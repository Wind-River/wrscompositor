DEFINES += QT_COMPOSITOR_QUICK

LIBS += -L ../lib

QT += quick qml
QT += quick-private
QT += webkitwidgets network
QT += compositor

#  if you want to compile QtCompositor as part of the application
#  instead of linking to it, remove the QT += compositor and uncomment
#  the following line
#include ../src/compositor/compositor.pri)

HEADERS += Process.h
SOURCES += main.cpp Process.cpp

OTHER_FILES = ContrastEffect.qml  main.qml  WindowChrome.qml  WindowContainer.qml alameda.jpg closebutton.png compositor.js
