
#ifndef VNA_DBUSCLIENT_H
#define VNA_DBUSCLIENT_H

#include <QtCore/QObject>

class VNADBusClient: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString vehicleInfoWMI READ vehicleInfoWMI NOTIFY vehicleInfoChanged)
    Q_PROPERTY(int speedometer READ speedometer NOTIFY vehicleInfoChanged)
public:
    VNADBusClient();
    QString vehicleInfoWMI() const { return mVehicleInfoWMI; };
    int speedometer() const { return mSpeedometer; };

signals:
    void vehicleInfoChanged();

public slots:
    void slotVehicleInfoWMI(const QString&);
    void slotSpeedometer(ushort);

private:
    QString mVehicleInfoWMI;
    ushort mSpeedometer;
};

#endif
