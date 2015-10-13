#ifndef GENIVI_WAYLAND_IVI_EXTENSION_H
#define GENIVI_WAYLAND_IVI_EXTENSION_H

#include <iostream>

#include <QDebug>
#include <QString>
#include <QObject>
#include <QQuickView>
#include <QQuickItem>
#include <QMap>
#include <QMetaType>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusVariant>
#include <QtDBus/QDBusArgument>
#include <QtDBus/QDBusMetaType>
#include <QtDBus/QDBusReply>
#include <QFile>


namespace GeniviWaylandIVIExtension {

    class IVIScene : public QObject
    {
    Q_OBJECT

    public:
        IVIScene(QObject* parent=0);
    };

    class IVIScreen : public QObject
    {
    Q_OBJECT

    public:
        IVIScreen(QObject* parent=0);
    };

    class IVILayer : public QObject
    {
    Q_OBJECT

    public:
        IVILayer(QObject* parent=0);
    };

    class IVISurface : public QObject
    {
    Q_OBJECT

    public:
        IVISurface(QObject* parent=0);
    };
}

#endif // AUDIOMANAGERDBUSINTERFACE_H
