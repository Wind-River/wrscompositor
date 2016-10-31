#include <QApplication>
#include <QPushButton>

#include <QWindow>

#include "qiviwindow.h"
#include "qtwaylandiviapplication.h"

#define IVI_ID 5

int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    QtWaylandClient::QtWaylandIviApplication iviApp(&app);

    QIVIWindow iviWindow(&iviApp, IVI_ID);

    QPushButton button ("! Hello world !", &iviWindow);

    //button.show();
    button.connect(&button, SIGNAL(pressed()), &iviApp, SLOT(pressed()));
    iviWindow.show();

    return app.exec();
}
//https://codereview.qt-project.org/#/c/121297/14
