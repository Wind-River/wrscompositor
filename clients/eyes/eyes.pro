QT       += core gui widgets

TARGET = eyes
TEMPLATE = app

SOURCES += main.cpp

SOURCES += \
    WLEyes.cpp \


HEADERS += \
    WLEyes.h \

# install
target.path = $$[QT_INSTALL_EXAMPLES]/opengl/cube
INSTALLS += target
