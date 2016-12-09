QT += waylandclient-private
CONFIG += wayland-scanner

!contains(QT_CONFIG, no-pkg-config) {
    PKGCONFIG += wayland-client wayland-cursor
    CONFIG += link_pkgconfig
} else {
    LIBS += -lwayland-client -lwayland-cursor
}

config_xkbcommon {
    !contains(QT_CONFIG, no-pkg-config) {
        PKGCONFIG += xkbcommon
    } else {
        LIBS += -lxkbcommon
    }
} else {
    DEFINES += QT_NO_WAYLAND_XKB
}

WAYLANDCLIENTSOURCES += \
    ../protocol/ivi-application.xml \
    ../protocol/ivi-controller.xml

HEADERS += \
    qtwaylandivisurface.h \
    qtwaylandiviapplication.h \
    qiviwindow.h


#   qwaylandivishellintegration.h \
#    qwaylandivisurface_p.h

SOURCES += \
    main.cpp \
    qtwaylandivisurface.cpp \
    qtwaylandiviapplication.cpp \
    qiviwindow.cpp

#qwaylandivishellintegration.cpp \
#    qwaylandivisurface.cpp

QT += core
QT += gui widgets

CONFIG += c++11

TARGET = ivi_client
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app
