#include "duduregiconfig.h"
#include "projectionmode.h"
#include <QVariant>
#include <QMetaType>
#include <QTime>
#include <QDebug>
#include <QCoreApplication>
#include <QDBusConnection>
#include <QDir>
#include <QFileInfo>
#include <stdlib.h>
#include "projectionmode_adaptor.h"

ProjectionModePrivate::ProjectionModePrivate(QObject *parent) :
    QObject(parent)
{
}

ProjectionMode::ProjectionMode(QObject *parent) :
    QObject(parent)
{
    ProjectionModePrivate *pm = new ProjectionModePrivate();
    new ProjectionModeAdaptor(pm);
    //QDBusConnection connection = QDBusConnection::systemBus();
    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerObject("/AndroidAuto", pm);
    connection.registerService("com.windriver.automotive.ProjectionMode");
}
