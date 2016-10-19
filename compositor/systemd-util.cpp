/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

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
    if(unitFile == "")
        return;

    mUnitFile = unitFile;
    QDBusMessage ret = QDBusConnection::sessionBus().call(
            QDBusMessage::createMethodCall(SYSTEMD_SERVICE,
                    SYSTEMD_OBJECT_PATH,
                    SYSTEMD_MANAGER_IF,
                    QStringLiteral("LoadUnit")) << mUnitFile);
    mUnitPath = ret.arguments()[0].value<QDBusObjectPath>().path();
}

void SystemdUnit::notifyPidChanged(uint pid) {
	qDebug() << __func__ << "pid = " << pid;
	mPid = pid;

	emit pidChanged(pid);
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

void SystemdDbusClient::checkResult(uint pid, const QString &result) {
	qDebug() << __func__ << "pid = " << pid;

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
	} else {
		qDebug() << "Invalid result";
	}
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
	if (interface_name != "org.freedesktop.systemd1.Service")
		return;

	//qDebug() << __func__;

	uint pid = changed_properties["ExecMainPID"].toUInt();
	checkResult(pid, 
		changed_properties["Result"].toString());

	SystemdUnit* unit;
	foreach (unit, mUnitList) {
		if (unit != NULL) {
			if (pid == unit->getPid()) {
				qDebug() << __func__ <<", Unit stopped by Systemd-Dbus, pid = 0";
				unit->notifyPidChanged(0);
				return;
			}

			if ( (0 == unit->getPid()) && 
							(getUnitByPID(pid) == unit->getUnitPath())) {
				qDebug() << __func__ << ", Unit is started by Systemd-Dbus, pid = " << pid;
				unit->notifyPidChanged(pid);
				return;
			}
		}
	}
}

int SystemdDbusClient::getPidByUnitFile(const QString &unitFile) {
	SystemdUnit* unit;
	foreach (unit, mUnitList) {
		if (unit != NULL) {
			if (unitFile == unit->getUnitFile()) {
				qDebug() << "get pid value from unitFile, " << unit->getUnitFile();
				return unit->getPid();
			}
		}
	}
	return -1;
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

	ret = QDBusConnection::sessionBus().call(
		QDBusMessage::createMethodCall(SYSTEMD_SERVICE,
			SYSTEMD_OBJECT_PATH,
			SYSTEMD_MANAGER_IF,
			QStringLiteral("GetUnitByPID")) << pid);


	QString unitPath = ret.arguments()[0].value<QDBusObjectPath>().path();

	//qDebug() << __func__ << " unitPath by PID = " << unitPath;

	return unitPath;
}
