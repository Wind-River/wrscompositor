QT += qml quick gui-private dbus widgets
CONFIG += plugin
TARGET = wrscompositorplatforminputcontextplugin
TEMPLATE = lib

PLUGIN_TYPE = platforminputcontexts
PLUGIN_EXTENDS = -
PLUGIN_CLASS_NAME = QWrsCompositorPlatformInputContextPlugin
target.path = $$[QT_INSTALL_PLUGINS]/platforminputcontexts
INSTALLS += target

SOURCES += $$PWD/qwrscompositorplatforminputcontext.cpp \
           $$PWD/main.cpp

HEADERS += $$PWD/qwrscompositorplatforminputcontext.h

OTHER_FILES += $$PWD/wrscompositor.json
