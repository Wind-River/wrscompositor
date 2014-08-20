
#ifndef VNA_DBUSCLIENT_H
#define VNA_DBUSCLIENT_H

#include <QtCore/QObject>

class VNADBusClient: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString vehicleInfoWMI READ vehicleInfoWMI NOTIFY vehicleInfoChanged)
public:
    VNADBusClient();
    QString vehicleInfoWMI() const { return mVehicleInfoWMI; };

signals:
    void vehicleInfoChanged();

public slots:
    void slotVehicleInfoWMI(const QString&);

private:
    QString mVehicleInfoWMI;
};

#endif
