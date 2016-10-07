/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */

#include <QDebug>
#include <QtDBus/QtDBus>

#include "vna_dbusclient.h"


VNADBusClient::VNADBusClient() {
    if (!QDBusConnection::sessionBus().isConnected()) {
        qDebug() << "Could not connect to the D-Bus session.\n";
        return;
    }
    mSpeedometer = 0;

    //QDBusConnection::sessionBus().registerService("com.windriver.wrscompositor");
    //QDBusConnection::sessionBus().registerObject("/", this);
    QDBusConnection::sessionBus().connect(QString(), QString(), "org.genivi.vna", "get_vehicleinfo_wmi", this, SLOT(slotVehicleInfoWMI(const QString&)));
    QDBusConnection::sessionBus().connect(QString(), QString(), "org.genivi.vna", "get_runningstatus_speedometer", this, SLOT(slotSpeedometer(ushort)));
}




void VNADBusClient::slotVehicleInfoWMI(const QString &name) {
    mVehicleInfoWMI = name;
    emit vehicleInfoChanged();
}

void VNADBusClient::slotSpeedometer(ushort speed) {
    mSpeedometer = speed;
    emit vehicleInfoChanged();
}
