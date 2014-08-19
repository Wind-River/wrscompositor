QT += quick qml
QT += quick-private
#QT += webkitwidgets network
QT += webenginewidgets network


#QMAKE_CXXFLAGS += -DWLCOMPOSITOR
#QT += compositor
#DEFINES += QT_COMPOSITOR_QUICK
#LIBS += -L ../lib

HEADERS += Process.h
SOURCES += main.cpp Process.cpp

OTHER_FILES = ContrastEffect.qml  main.qml  WindowChrome.qml  WindowContainer.qml alameda.jpg closebutton.png compositor.js
