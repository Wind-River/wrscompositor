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

#ifndef SYSTEMD_UTIL_H
#define SYSTEMD_UTIL_H

#include <QtCore/QObject>
#include <QDBusInterface>
#include <QDebug>

class SystemdUnit: public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString unitPath WRITE setUnitPath);
	Q_PROPERTY(uint pid READ getPid NOTIFY pidChanged);
public:
	explicit SystemdUnit(QObject *parent = 0);
	uint getPid() { return mPid; }
	void setUnitPath(const QString &unitFile);
	QString getUnitPath() { return mUnitPath; }
	QString getUnitFile() { return mUnitFile; }
	void notifyPidChanged(uint pid);
signals:
	void pidChanged(uint pid);
private:
	uint mPid;
	QString mUnitPath;
	QString mUnitFile;
};

class SystemdDbusClient: public QObject
{
	Q_OBJECT
public:
	explicit SystemdDbusClient(QObject *parent = 0);
	Q_INVOKABLE void startUnit(const QString &unitFile);
	Q_INVOKABLE void stopUnit(const QString &unitFile);
	Q_INVOKABLE void registerUnit(SystemdUnit* unit) { mUnitList.append(unit); }
	Q_INVOKABLE int getPidByUnitFile(const QString &unitFile);
private slots:
	void slotJobNew(uint, QDBusObjectPath, QString);
	void slotJobRemove(uint, QDBusObjectPath, QString, QString);
	void slotPropertiesChanged(QString, QMap<QString,QVariant>, QStringList);
private:
	void checkResult(uint pid, const QString &result);
	QString getUnitByPID(uint pid);
	QVector<SystemdUnit*> mUnitList;
};
#endif
