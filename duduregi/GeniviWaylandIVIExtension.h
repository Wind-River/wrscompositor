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
    class IVIScreen;
    class IVIScene;
    class IVIRectangle : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
        Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
        Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
        Q_PROPERTY(int x READ x WRITE setX NOTIFY xChanged)
        Q_PROPERTY(int y READ y WRITE setY NOTIFY yChanged)
        public:
            IVIRectangle(int id, int w, int h, QObject *p=0) : QObject(p), mId(id), mX(0), mY(0), mWidth(w), mHeight(h) {};
            IVIRectangle(int id, int x, int y, int w, int h, QObject *p=0) : QObject(p), mId(id), mX(x), mY(y), mWidth(w), mHeight(h) {};
            int id() { return mId; };
            void setId(int id) { mId = id; emit idChanged(); };
            int width() { return mWidth; };
            void setWidth(int width) { mWidth = width; emit widthChanged(); };
            int height() { return mHeight; };
            void setHeight(int height) { mHeight = height; emit heightChanged(); };
            int x() { return mX; };
            void setX(int x) { mX = x; emit xChanged(); };
            int y() { return mY; };
            void setY(int y) { mY = y; emit yChanged(); };
        signals:
            void idChanged();
            void widthChanged();
            void heightChanged();
            void xChanged();
            void yChanged();
        private:
            int mId;
            int mX;
            int mY;
            int mWidth;
            int mHeight;
    };

    class IVISurface : public IVIRectangle
    {
    Q_OBJECT

    public:
        IVISurface(int id, int w, int h, QObject* parent=0);
        IVISurface(int id, int x, int y, int w, int h, QObject* parent=0);
    };

    class IVILayer : public IVIRectangle
    {
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<IVISurface> layers READ surfaces)

    public:
        IVILayer(int id, int w, int h, QObject* parent=0);
        IVILayer(int id, int x, int y, int w, int h, QObject* parent=0);

        QQmlListProperty<IVISurface> surfaces() {
            return QQmlListProperty<IVISurface>(this, mSurfaces);
        }
        int surfaceCount() const { return mSurfaces.count(); }
        IVISurface *surface(int i) const { return mSurfaces.at(i); }
    private:
        QList<IVISurface*> mSurfaces;
        IVIScreen  *mScreen;
    };

    class IVIScreen : public IVIRectangle
    {
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<IVILayer> layers READ layers)

    public:
        IVIScreen(int id, int w, int h, QObject* parent=0);
        Q_INVOKABLE void addLayer(int id);
        Q_INVOKABLE void addLayer(int id, int width, int height);

        QQmlListProperty<IVILayer> layers() {
            return QQmlListProperty<IVILayer>(this, mLayers);
        }
        int layerCount() const { return mLayers.count(); }
        IVILayer *layer(int i) const { return mLayers.at(i); }
    private:
        QList<IVILayer*> mLayers;
        IVIScreen  *mScene;
    };

    class IVIScene : public IVIRectangle
    {
    Q_OBJECT

    public:
        IVIScene(int w, int h, QObject *p=0);
    private:
        QList<IVIScreen*> mScreens;
        IVIScreen *mMainScreen;
    };
}

#endif // AUDIOMANAGERDBUSINTERFACE_H
