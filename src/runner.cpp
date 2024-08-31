#include "runner.h"

#include <KNotification>
#include <QAudioDevice>
#include <QLoggingCategory>
#include <QMediaDevices>
#include <QProcess>

Q_LOGGING_CATEGORY(RUNNER, "org.kde.krunner.kaudioswi");

namespace
{
const auto kIconNameAudio = QStringLiteral("audio-symbolic");
const auto kIconNameAudioOff = QStringLiteral("audio-off-symbolic");
const auto kIconNameAudioOn = QStringLiteral("audio-on-symbolic");

const auto kKeyWord = QStringLiteral("to ");
const auto kMatchFormat = QStringLiteral("%1 %2").arg(kKeyWord);

const auto kComponentName = QStringLiteral("Switch Audio");
} // namespace

namespace
{
void showNotification(const QString &title, const QString &body, const bool isWarning)
{
    auto *notif = new KNotification(QStringLiteral("kAudioSwiNotification"), KNotification::CloseOnTimeout);
    notif->setComponentName(kComponentName);
    notif->setTitle(kComponentName);
    notif->setTitle(title);
    notif->setText(body);
    notif->setIconName(isWarning ? kIconNameAudioOff : kIconNameAudioOn);
    notif->sendEvent();
}
} // namespace

KAudioSwiRunner::KAudioSwiRunner(QObject *parent, const KPluginMetaData &data)
    : AbstractRunner(parent, data)
{
    qCDebug(RUNNER) << "ctor";
    setObjectName(QStringLiteral("kaudioswi"));
    addSyntax(KRunner::RunnerSyntax(QLatin1String("to <output name>"),
                                    QLatin1String("Change the default audio output to a selected sink")));
}

KAudioSwiRunner::~KAudioSwiRunner()
{
    qCDebug(RUNNER) << "dtor";
}

void KAudioSwiRunner::init()
{
    connect(this, &AbstractRunner::prepare, this, [this]() {
        // main thread

        m_outputs.clear();

        const auto &outputs = QMediaDevices::audioOutputs();
        for (const auto &o: outputs) {
            m_outputs[o.description()] = o.id();
            qCDebug(RUNNER) << "found output: " << o.description() << " [" << o.id() << "]";
        }
    });
}

void KAudioSwiRunner::match(KRunner::RunnerContext &context)
{
    auto query = context.query();

    if (!query.length() || query.startsWith(QLatin1Char('?'))) {
        return;
    }

    bool hasKeyWord = false;
    if (query.startsWith(kKeyWord)) {
        hasKeyWord = true;
        query = query.mid(kKeyWord.length());
    }

    QList<KRunner::QueryMatch> matches;
    for (const auto [name, id]: m_outputs.asKeyValueRange()) {
        if (query.isEmpty() || name.contains(query, Qt::CaseInsensitive)) {
            KRunner::QueryMatch match(this);
            match.setText(kMatchFormat.arg(name));
            match.setData(QStringList() << QLatin1String(id) << name);
            match.setRelevance(hasKeyWord ? 100.f : 80.f);

            {
                using Relevance = KRunner::QueryMatch::CategoryRelevance;
                match.setCategoryRelevance(hasKeyWord ? Relevance::Highest : Relevance::Moderate);
            }

            match.setIconName(kIconNameAudio);
            matches.append(match);
        }
    }

    context.addMatches(matches);
}

void KAudioSwiRunner::run(const KRunner::RunnerContext &context, const KRunner::QueryMatch &match)
{
    Q_UNUSED(context);

    if (!match.isValid()) {
        return;
    }

    const auto &items = match.data().toStringList();
    if (items.length() != 2) {
        qCWarning(RUNNER) << "invalid match data, length [" << items.length() << "]";
    }

    const auto &id = items[0];
    const auto &name = items[1];
    qCInfo(RUNNER) << "switching audio output to [" << id << ", " << name << "]";

    QProcess p;
    p.start(QStringLiteral("pactl"), QStringList() << QStringLiteral("set-default-sink") << id);
    if (!p.waitForFinished(1000)) {
        p.kill();
    }
    if (p.exitStatus() != QProcess::NormalExit || p.exitCode() != 0) {
        qCWarning(RUNNER) << "failed to change default sink with error [" << p.exitCode() << "]";
        showNotification(name, QStringLiteral("Failed to change default audio output"), true);
    }
}

K_PLUGIN_CLASS_WITH_JSON(KAudioSwiRunner, "kaudioswi.json")

#include "runner.moc"