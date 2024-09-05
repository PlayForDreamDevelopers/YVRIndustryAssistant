QT += quick winextras  websockets
CONFIG += c++11
TRANSLATIONS = chinese.ts
QMAKE_CXXFLAGS += /source-charset:utf-8 /execution-charset:utf-8

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

TARGET = "PFDMIndustryAssistant"

win32 {
    CONFIG(release, debug|release) {
        DESTDIR = ../bin/Release
    } else {
        DESTDIR = ../bin/Debug
    }
RC_ICONS = $$PWD/res/image/yvr.ico
}



# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ResourceAddListModel.cpp \
    VRDiscoverListModel.cpp \
    VRPlayListModel.cpp \
    network/ConnectManager.cpp \
    plugins/FramelessQuickWindow.cpp \
    plugins/JsonFileParse.cpp \
    ResourceListModel.cpp \
    Settings.cpp \
    VRListModel.cpp \
    VRMonitor.cpp \
    YVRManager.cpp \
    plugins/YVRUpdate.cpp \
    network/YVRWebServer.cpp \
    main.cpp \
    plugins/utils.cpp

RESOURCES += qml.qrc


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    DefineData.h \
    ResourceAddListModel.h \
    VRDiscoverListModel.h \
    VRPlayListModel.h \
    network/ConnectManager.h \
    plugins/FramelessQuickWindow.h \
    plugins/JsonFileParse.h \
    ResourceListModel.h \
    Settings.h \
    VRListModel.h \
    VRMonitor.h \
    YVRManager.h \
    plugins/YVRUpdate.h \
    network/YVRWebServer.h \
    plugins/utils.h \
    version.h

#requireAdministrator
#QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"


