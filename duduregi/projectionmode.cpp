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

ProjectionModePrivate::ProjectionModePrivate(QObject *parent) :
    QObject(parent)
{
    qDebug() << "[41m haha [0m";
}
void ProjectionModePrivate::sendMousePressed(int x, int y) {
    emit touchEvent(x, y, 0);
};
void ProjectionModePrivate::sendKeyPressed(int keycode) {
    emit keyEvent(keycode, true);
}
void ProjectionModePrivate::sendMouseReleased(int x, int y) {
    emit touchEvent(x, y, 0);
};
void ProjectionModePrivate::sendKeyReleased(int keycode) {
    emit keyEvent(keycode, false);
}

ProjectionMode::ProjectionMode(QObject *parent) :
    QObject(parent)
{
    mPM = new ProjectionModePrivate();
    new ProjectionModeAdaptor(mPM);
    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerObject("/AndroidAuto", mPM);
    connection.registerService("com.windriver.automotive.ProjectionMode1");
}
