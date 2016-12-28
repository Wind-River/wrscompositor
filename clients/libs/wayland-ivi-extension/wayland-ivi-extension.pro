QT += gui-private quick qml
QT += waylandclient-private
CONFIG += wayland-scanner staticlib
TARGET = WrsQtWaylandIviExtension
TEMPLATE = lib

!contains(QT_CONFIG, no-pkg-config) {
    PKGCONFIG += wayland-client wayland-cursor
    CONFIG += link_pkgconfig
} else {
    LIBS += -lwayland-client -lwayland-cursor
}

WAYLANDCLIENTSOURCES += \
    ../protocol/ivi-application.xml \
    ../protocol/ivi-controller.xml

LOCAL_SOURCES = qwaylandiviextension.cpp qwaylandivisurface.cpp
LOCAL_HEADERS = qwaylandiviextension.h qwaylandivisurface.h

SOURCES += $$LOCAL_SOURCES
HEADERS += $$LOCAL_HEADERS
