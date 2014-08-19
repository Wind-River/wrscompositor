QT += quick qml
QT += quick-private
#QT += webkitwidgets network
QT += webenginewidgets network
QT += dbus


#QMAKE_CXXFLAGS += -DWLCOMPOSITOR
#QT += compositor
#DEFINES += QT_COMPOSITOR_QUICK
#LIBS += -L ../lib

HEADERS += Process.h dbus_client.h
SOURCES += main.cpp Process.cpp dbus_client.cpp

OTHER_FILES = ContrastEffect.qml  main.qml  WindowChrome.qml  WindowContainer.qml alameda.jpg closebutton.png compositor.js
