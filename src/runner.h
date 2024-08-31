
#pragma once

#include <KRunner/AbstractRunner>

class KAudioSwiRunner final: public KRunner::AbstractRunner
{
    Q_OBJECT

  public:
    KAudioSwiRunner(QObject *parent, const KPluginMetaData &data);
    ~KAudioSwiRunner() override;

    void match(KRunner::RunnerContext &context) override;
    void run(const KRunner::RunnerContext &context, const KRunner::QueryMatch &match) override;

  protected:
    void init() override;

  private:
    QMap<QString, QByteArray> m_outputs;
};