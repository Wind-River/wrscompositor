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

#ifndef SKOBBLERNAVI_H
#define SKOBBLERNAVI_H

#include <QtWidgets>
#include <QtNetwork>
#include <QtWebKitWidgets>
#include <QtGui/QGuiApplication>
#include <QtGui/private/qguiapplication_p.h>
#include <QtGui/qpa/qplatformnativeinterface.h>
#include <QtWaylandClient/private/qwaylandintegration_p.h>
#include <QtWaylandClient/private/qwaylandwindow_p.h>
#include <QtWaylandClient/private/qwaylanddisplay_p.h>
#include "qwayland-ivi-application.h"
#include "qwayland-ivi-controller.h"
#include "qwaylandivisurface.h"

#define WRS_IVI_ID_SURFACE_DEFAULT          0
#define WRS_IVI_ID_SURFACE_NAVIGATION       1
#define WRS_IVI_ID_SURFACE_DIALOG           3
#define WRS_IVI_ID_SURFACE_PHONE            4
#define WRS_IVI_ID_SURFACE_PROJECTION       5
#define WRS_IVI_ID_SURFACE_CAMERA           6

class SkobblerNavi : public QWebView 
{
    Q_OBJECT

public:
    SkobblerNavi(QWidget* parent = 0);
    ~SkobblerNavi();

protected:
   bool event(QEvent *event);

private:
	static void registryIvi(void *data, struct wl_registry *registry,
		uint32_t id, const QString &interface, uint32_t version);
    bool createSurface();
    
private:
    QtWayland::ivi_application *mIviApplication;
    QtWayland::ivi_controller *mIviController;
    QtWaylandClient::QWaylandIviSurface *mIviSurface;
    uint32_t mSurfaceId;
};

#endif
