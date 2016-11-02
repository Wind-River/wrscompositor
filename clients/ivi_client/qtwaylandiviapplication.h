#ifndef QTWAYLANDIVIAPPLICATION_H
#define QTWAYLANDIVIAPPLICATION_H

#include <QObject>
#include <QApplication>
#include <QWindow>
#include <QtWaylandClient>
#include <QtWaylandClient/private/qwaylandwindow_p.h>
#include <QtWaylandClient/private/qwaylanddisplay_p.h>
#include "qwayland-ivi-application.h"
#include "qtwaylandivisurface.h"

namespace QtWaylandClient {
    class QtWaylandIviApplication : public QObject, public QtWayland::ivi_application
    {
        Q_OBJECT
    private:
        QApplication *mApp;
    public:
        QtWaylandIviApplication(QApplication *app);

        ~QtWaylandIviApplication();

    public slots:
        void pressed();
    };
}


#endif // QTWAYLANDIVIAPPLICATION_H
