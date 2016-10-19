/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef GENIVI_WAYLAND_IVI_EXTENSION_H
#define GENIVI_WAYLAND_IVI_EXTENSION_H

#include <iostream>

#include <QtCompositor/QWaylandOutput>
#include <QtCompositor/QWaylandCompositor>
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
    class IVIScene;
    class IVIScreen;
    class IVILayer;

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
    Q_PROPERTY(double opacity READ opacity WRITE setOpacity NOTIFY propertyChanged)
    Q_PROPERTY(int orientation READ orientation WRITE setOrientation NOTIFY propertyChanged)
    Q_PROPERTY(int visibility READ visibility WRITE setVisibility NOTIFY propertyChanged)

    public:
        IVISurface(QObject *parent=0);
        IVISurface(int id, int w, int h, IVILayer* parent=0);
        IVISurface(int id, int x, int y, int w, int h, IVILayer* parent=0);

        double opacity() const { return mOpacity; }
        void setOpacity(double o) { mOpacity = o; emit propertyChanged(); };
        int orientation() const { return mOrientation; }
        void setOrientation(int o) { mOrientation = o; emit propertyChanged(); };
        int visibility() const { return mVisibility; }
        void setVisibility(int o) { mVisibility = o; emit propertyChanged(); };

        QObject *qmlWindowFrame() { return mQmlWindowFrame; };
        void setQmlWindowFrame(QObject *obj) { mQmlWindowFrame = obj; };

        struct wl_resource* waylandResource() { return mWaylandResource; }
        void setWaylandResource(struct wl_resource *r) { mWaylandResource = r; }
    signals:
        void propertyChanged();
    private:
        IVILayer *mLayer;
        double mOpacity;
        int mOrientation;
        int mVisibility;
        struct wl_resource *mWaylandResource;
        QObject *mQmlWindowFrame;
    };

    class IVILayer : public IVIRectangle
    {
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<IVISurface> layers READ surfaces)
    Q_PROPERTY(double opacity READ opacity WRITE setOpacity NOTIFY propertyChanged)
    Q_PROPERTY(int orientation READ orientation WRITE setOrientation NOTIFY propertyChanged)
    Q_PROPERTY(int visibility READ visibility WRITE setVisibility NOTIFY propertyChanged)

    public:
        IVILayer(QObject* parent=0);
        IVILayer(int id, int w, int h, IVIScreen* parent=0);
        IVILayer(int id, int x, int y, int w, int h, IVIScreen* parent=0);

        Q_INVOKABLE IVISurface* addSurface(int x, int y, int width, int height, QObject *qmlWindowFrame);
        Q_INVOKABLE void removeSurface(IVISurface*);

        QQmlListProperty<IVISurface> surfaces() {
            return QQmlListProperty<IVISurface>(this, mSurfaces);
        }
        Q_INVOKABLE int surfaceCount() const { return mSurfaces.count(); }
        Q_INVOKABLE IVISurface *surface(int i) const { return mSurfaces.at(i); }

        double opacity() const { return mOpacity; }
        void setOpacity(double o) { mOpacity = o; emit propertyChanged(); };
        int orientation() const { return mOrientation; }
        void setOrientation(int o) { mOrientation = o; emit propertyChanged(); };
        int visibility() const { return mVisibility; }
        void setVisibility(int o) { mVisibility = o; emit propertyChanged(); };

        IVIScreen* screen() {return mScreen;};
        void setWaylandResource(struct wl_resource *r) { mWaylandResource = r; }
        struct wl_resource* waylandResource() { return mWaylandResource; }

    signals:
        void propertyChanged();
    private:
        QList<IVISurface*> mSurfaces;
        IVIScreen  *mScreen;
        double mOpacity;
        int mOrientation;
        int mVisibility;
        struct wl_resource *mWaylandResource;
    };

    class IVIScreen : public IVIRectangle
    {
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<IVILayer> layers READ layers)

    public:
        IVIScreen(QObject* parent=0);
        IVIScreen(int id, int w, int h, IVIScene* parent=0);
        Q_INVOKABLE void addLayer(int id);
        Q_INVOKABLE void addLayer(int id, int width, int height);

        QWaylandOutput * waylandOutput() { return mWaylandOutput; };
        void setWaylandOutput(QWaylandOutput *output) { mWaylandOutput = output; };

        QQmlListProperty<IVILayer> layers() {
            return QQmlListProperty<IVILayer>(this, mLayers);
        }
        Q_INVOKABLE int layerCount() const { return mLayers.count(); }
        Q_INVOKABLE IVILayer *layer(int i) const { return mLayers.at(i); }
        Q_INVOKABLE IVILayer *layerById(int id);

    private:
        QList<IVILayer*> mLayers;
        IVIScene  *mScene;
        QWaylandOutput *mWaylandOutput;
    };

    class IVIScene : public IVIRectangle
    {
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<IVIScreen> screens READ screens)
    Q_PROPERTY(IVIScreen* mainScreen READ mainScreen)

    public:
        IVIScene(QObject *p=0);
        IVIScene(QWaylandCompositor *compositor, int w, int h, QObject *p=0);
        IVIScreen* mainScreen() { return mMainScreen; };

        QQmlListProperty<IVIScreen> screens() {
            return QQmlListProperty<IVIScreen>(this, mScreens);
        }
        Q_INVOKABLE int screenCount() const { return mScreens.count(); }
        Q_INVOKABLE IVIScreen *screen(int i) const { return mScreens.at(i); }
    private:
        QList<IVIScreen*> mScreens;
        IVIScreen *mMainScreen;
        QWaylandCompositor *mCompositor;
    };

}

#endif // AUDIOMANAGERDBUSINTERFACE_H
