
#ifndef DBUS_CLIENT_H
#define DBUS_CLIENT_H

#include <QtCore/QObject>

class DBusClient: public QObject
{
    Q_OBJECT
    //Q_CLASSINFO("D-Bus Interface", "com.windriver.VehicleEventBroker")
    Q_PROPERTY(QString message READ getMessage WRITE setMessage NOTIFY messageReceived)
    Q_PROPERTY(int speed READ getSpeed WRITE setSpeed NOTIFY speedChanged)

public:
    DBusClient();
    QString getMessage() const;
    void setMessage(const QString &message);
    int getSpeed();
    void setSpeed(int speed);

    //Q_INVOKABLE void receiveMessage(const QString &message);

signals:
    void messageReceived();
    void speedChanged();

public slots:
    void receiveMessage(const QString &arg);
    void receiveSpeed(unsigned int arg);

private:
    QString mMessage;
    int mSpeed;
};

#endif
