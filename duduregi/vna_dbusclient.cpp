
#include <QDebug>
#include <QtDBus/QtDBus>

#include "vna_dbusclient.h"


VNADBusClient::VNADBusClient() {
    if (!QDBusConnection::sessionBus().isConnected()) {
        qDebug() << "Could not connect to the D-Bus session.\n";
        return;
    }

    //QDBusConnection::sessionBus().registerService("com.windriver.duduregi");
    //QDBusConnection::sessionBus().registerObject("/", this);
    QDBusConnection::sessionBus().connect(QString(), QString(), "org.genivi.vna", "get_vehicleinfo_wmi", this, SLOT(slotVehicleInfoWMI(const QString&)));
}


void VNADBusClient::slotVehicleInfoWMI(const QString &name) {
    mVehicleInfoWMI = name;
    emit vehicleInfoChanged();
}
