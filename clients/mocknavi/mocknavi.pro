QT       += core gui widgets

TARGET = mocknavi
TEMPLATE = app

SOURCES += main.cpp

SOURCES += \
    mainwidget.cpp \
    Camera.cpp \
    Car.cpp \
    Argument.cpp \
    Ground.cpp \
    House.cpp \
    Street.cpp \
    ShaderLighting.cpp \
    WaylandServerinfoProtocol.cpp \
    IlmMatrix.cpp \
    geometryengine.cpp

HEADERS += \
    mainwidget.h \
    geometryengine.h \
    Camera.h \
    Car.h \
    Argument.h \
    Ground.h \
    House.h \
    IRenderable.h \
    IUpdateable.h \
    Street.h \
    ShaderLighting.h \

RESOURCES += \
    shaders.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/opengl/cube
INSTALLS += target
