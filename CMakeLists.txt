cmake_minimum_required(VERSION 3.28)
project(kaudioswi)

set(CMAKE_VERBOSE_MAKEFILE, ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(KF6_MIN_VERSION "6.0")

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)

find_package(Qt6 "6.0" CONFIG REQUIRED COMPONENTS Multimedia)
find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS Runner Notifications)

add_library(kaudioswi MODULE src/runner.h src/runner.cpp)
target_link_libraries(kaudioswi
        KF6::Runner
        KF6::Notifications
        Qt6::Multimedia
)

install(TARGETS kaudioswi DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf6/krunner)
install(FILES kaudioswi.notifyrc DESTINATION ${KDE_INSTALL_KNOTIFYRCDIR})
