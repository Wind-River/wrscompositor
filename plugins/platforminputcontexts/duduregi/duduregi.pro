QT += qml quick gui-private dbus widgets
CONFIG += plugin
TARGET = duduregiplatforminputcontextplugin
TEMPLATE = lib

PLUGIN_TYPE = platforminputcontexts
PLUGIN_EXTENDS = -
PLUGIN_CLASS_NAME = QDuduregiPlatformInputContextPlugin
target.path = $$[QT_INSTALL_PLUGINS]/platforminputcontexts
INSTALLS += target

SOURCES += $$PWD/qduduregiplatforminputcontext.cpp \
           $$PWD/main.cpp

HEADERS += $$PWD/qduduregiplatforminputcontext.h

OTHER_FILES += $$PWD/duduregi.json
