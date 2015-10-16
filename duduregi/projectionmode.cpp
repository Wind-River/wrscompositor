#include "duduregiconfig.h"
#include "projectionmode.h"
#include <QVariant>
#include <QMetaType>
#include <QTime>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <stdlib.h>
#include <QHostAddress>

ProjectionModePrivate::ProjectionModePrivate(QObject *parent) :
    QObject(parent)
{

}

ProjectionMode::ProjectionMode(QObject *parent) :
    QTcpServer(parent)
{
    mPM = new ProjectionModePrivate();
    new ProjectionModeAdaptor(mPM);
    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerObject("/AndroidAuto", mPM);
    connection.registerService("com.windriver.automotive.ProjectionMode1");

    listen(QHostAddress("/tmp/duduregi-projection-androidauto-video"));;
}
void ProjectionMode::sendMousePressed(int x, int y) {
    emit mPM->touchEvent(x, y, 1);
};
void ProjectionMode::sendKeyPressed(int keycode) {
    emit mPM->keyEvent(keycode, true);
}
void ProjectionMode::sendMouseReleased(int x, int y) {
    emit mPM->touchEvent(x, y, 0);
};
void ProjectionMode::sendKeyReleased(int keycode) {
    emit mPM->keyEvent(keycode, false);
}
void ProjectionMode::incomingConnection(qintptr socketDescriptor) {
    ProjectionStream *stream = new ProjectionStream(this);
    stream->setSocketDescriptor(socketDescriptor);
    qDebug() << "---";
    qDebug() << findChild<QObject*>("mediaPlayer");
    qDebug() << "---";
}

ProjectionStream::ProjectionStream(QObject *parent) :
    QTcpSocket(parent)
{
}
