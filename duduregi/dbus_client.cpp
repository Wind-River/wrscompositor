
#include <QDebug>
#include <QtDBus/QtDBus>

#include "dbus_client.h"


DBusClient::DBusClient() {
    qDebug() << "DBusClient!!!";

    if (!QDBusConnection::sessionBus().isConnected()) {
        qDebug() << "Cannot connect to th D-Bus session bus.\n";
    }

    QDBusConnection::sessionBus().registerObject("/", this);

    QDBusConnection::sessionBus().connect(QString(), QString(), "com.windriver.VehicleEventBroker", "message", this, SLOT(receiveMessage(QString)));
    QDBusConnection::sessionBus().connect(QString(), QString(), "com.windriver.VehicleEventBroker", "vehicleSpeedChanged", this, SLOT(receiveSpeed(unsigned int)));
}

QString DBusClient::getMessage() const {
    return mMessage;
}

void DBusClient::setMessage(const QString &message) {
    mMessage = message;
    qDebug() << "setMessage : " << message << "\n";
    emit messageReceived();
}

void DBusClient::receiveMessage(const QString &arg) {

    qDebug() << "message received : " << arg << "\n";
    setMessage(arg);
}

int DBusClient::getSpeed() {
    return mSpeed;
}

void DBusClient::setSpeed(int speed) {
    mSpeed = speed;
    qDebug() << "setSpeed : " << speed << "\n";
    emit speedChanged();
}

void DBusClient::receiveSpeed(unsigned int arg) {

    qDebug() << "speed changed : " << arg << "\n";
    setSpeed(arg);
}

