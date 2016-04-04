#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusReply>
#include "systemd-util.h"

#define SYSTEMD_SERVICE "org.freedesktop.systemd1"
#define SYSTEMD_OBJECT_PATH	"/org/freedesktop/systemd1"
#define SYSTEMD_MANAGER_IF "org.freedesktop.systemd1.Manager"
#define DBUS_PROPERTIES_IF "org.freedesktop.DBus.Properties"

SystemdUnit::SystemdUnit(QObject *parent) :
	QObject(parent), mPid(0) {
}

void SystemdUnit::setUnitPath(const QString &unitFile) {
	QDBusMessage ret = QDBusConnection::sessionBus().call(
		QDBusMessage::createMethodCall(SYSTEMD_SERVICE,
			SYSTEMD_OBJECT_PATH,
			SYSTEMD_MANAGER_IF,
			QStringLiteral("LoadUnit")) << unitFile);

	mUnitPath = ret.arguments()[0].value<QDBusObjectPath>().path();
}

void SystemdUnit::notifyPidChanged(uint pid) {
	qDebug() << __func__ << "pid = " << pid;
	mPid = pid;
	emit pidChanged(pid);
}

void SystemdUnit::notifyResultChanged(const QString &unitPath, const QString &result) {
	qDebug() << __func__ << "unitPath = " << unitPath;

	if (result == "success") {
		qDebug() << "a service succeed";
	} else if (result == "resources") {
		qDebug() << "not enough resources have been available to fork off and execute the service processes";
	} else if (result == "timeout") {
		qDebug() << "a time-out occurred while executing a service operation";
	} else if (result == "exit-code") {
		qDebug() << "a service process exited with an unclean exit code";
	} else if (result == "signal"){
		qDebug() << "a service process exited with an uncaught signal";
	} else if (result == "core-dump") {
		qDebug() << "a service process exited uncleanly and dumped core";
	} else if (result == "watchdog") {
		qDebug() << "a service did not send out watchdog ping messages often enough";
	} else if (result == "start-limit") {
		qDebug() << "a service has been started too frequently in a time frame";
	}
}

SystemdDbusClient::SystemdDbusClient(QObject *parent) :
	QObject(parent) {

	if (!QDBusConnection::sessionBus().isConnected()) {
		qWarning() << "Could not connect to the D-Bus session.\n";
		return;
	}
	QDBusConnection::sessionBus().call(
		QDBusMessage::createMethodCall(SYSTEMD_SERVICE,
		SYSTEMD_OBJECT_PATH,
		SYSTEMD_MANAGER_IF,
		QStringLiteral("Subscribe")));

	QDBusConnection::sessionBus().connect(SYSTEMD_SERVICE,
		SYSTEMD_OBJECT_PATH,
		SYSTEMD_MANAGER_IF,
		QStringLiteral("JobNew"), this,
		SLOT(slotJobNew(uint, QDBusObjectPath, QString)));

	QDBusConnection::sessionBus().connect(SYSTEMD_SERVICE,
		SYSTEMD_OBJECT_PATH,
		SYSTEMD_MANAGER_IF,
		QStringLiteral("JobRemove"), this,
		SLOT(slotJobRemove(uint, QDBusObjectPath, QString, QString)));
}

void SystemdDbusClient::slotJobNew(uint id, QDBusObjectPath job, QString unit) {
    (void)id;
    (void)job;
    (void)unit;
    qDebug() << __func__;
    // Nothing to do
}

void SystemdDbusClient::slotJobRemove(uint id, QDBusObjectPath job, QString unit, QString result) {
    (void)id;
    (void)job;
    (void)unit;
    (void)result;
    qDebug() << __func__;
    // Nothing to do
}

void SystemdDbusClient::slotPropertiesChanged(QString interface_name, QMap<QString,QVariant> changed_properties, QStringList invalidated_properties) {
	qDebug() << __func__ << "interface_name = " << interface_name;
	qDebug() << __func__ << "changed_properties = " << changed_properties;
	qDebug() << __func__ << "invalidated_properties = " << invalidated_properties;

	if (interface_name != "org.freedesktop.systemd1.Service")
		return;

	uint pid = changed_properties["ExecMainPID"].toUInt();
	QString unitPathByPID = getUnitByPID(pid);
	if (unitPathByPID.isEmpty() || unitPathByPID.isNull()) {
		qWarning() << "Invalid unitPath";
		return;
	}

	QString result = changed_properties["Result"].toString();
	if (result.isEmpty() || result.isNull()) {
		qWarning() << "Invalid result from systemd";
		return;
	}

	SystemdUnit* unit;
	foreach (unit, mUnitList) {
		if (unit != NULL) {
			if (unitPathByPID == unit->getUnitPath()) {
				qDebug() << "Found unitPath executed by Systemd-Dbus";
				unit->notifyPidChanged(pid);
				unit->notifyResultChanged(unitPathByPID, result);
			}
		}
	}
}

void SystemdDbusClient::startUnit(const QString &unitFile) {
	QDBusMessage ret;

	qDebug() << __func__;

	ret = QDBusConnection::sessionBus().call(
			QDBusMessage::createMethodCall(SYSTEMD_SERVICE,
			SYSTEMD_OBJECT_PATH,
			SYSTEMD_MANAGER_IF,
			QStringLiteral("LoadUnit")) << unitFile);

	QString unitPath = ret.arguments()[0].value<QDBusObjectPath>().path();
	qDebug() << unitPath;

	QDBusConnection::sessionBus().connect(
		SYSTEMD_SERVICE, unitPath,
		DBUS_PROPERTIES_IF, "PropertiesChanged",
		this, SLOT(slotPropertiesChanged(QString,QMap<QString,QVariant>,QStringList)));

	ret = QDBusConnection::sessionBus().call(
			QDBusMessage::createMethodCall(SYSTEMD_SERVICE,
			SYSTEMD_OBJECT_PATH,
			SYSTEMD_MANAGER_IF,
			QStringLiteral("StartUnit")) << unitFile << "replace");

	qWarning() << ret;
}

void SystemdDbusClient::stopUnit(const QString &unitFile) {
	QDBusMessage ret;

	qDebug() << __func__;

	ret = QDBusConnection::sessionBus().call(
			QDBusMessage::createMethodCall(SYSTEMD_SERVICE,
			SYSTEMD_OBJECT_PATH,
			SYSTEMD_MANAGER_IF,
			QStringLiteral("StopUnit")) << unitFile << "replace");

	qWarning() << ret;
}

QString SystemdDbusClient::getUnitByPID(uint pid) {
	QDBusMessage ret;

	qDebug() << __func__;

	ret = QDBusConnection::sessionBus().call(
		QDBusMessage::createMethodCall(SYSTEMD_SERVICE,
			SYSTEMD_OBJECT_PATH,
			SYSTEMD_MANAGER_IF,
			QStringLiteral("GetUnitByPID")) << pid);

	qWarning() << ret;

	QString unitPath = ret.arguments()[0].value<QDBusObjectPath>().path();

	qDebug() << __func__ << " unitPath by PID = " << unitPath;

	return unitPath;
}
