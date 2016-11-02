#include "qtwaylandiviapplication.h"

namespace QtWaylandClient {


    QtWaylandIviApplication::QtWaylandIviApplication(QApplication *app) {
        mApp = app;
    }


    QtWaylandIviApplication::~QtWaylandIviApplication() {

    }


    void QtWaylandIviApplication::pressed() {
        qDebug() << "Button pressed";
    }
}
