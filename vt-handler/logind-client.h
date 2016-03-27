/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
#ifndef WR_LOGIND_CLIENT_H
#define WR_LOGIND_CLIENT_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include <QSocketNotifier>

class LogindClient: public QObject
{
    Q_OBJECT

public:
    LogindClient();

public:
    static void signalHandler(int unused);
    void systemdConnect();
    void restoreTTY();
    void setDRMFd(int fd) { mDRMFd = fd; };
signals:
    void paused();
    void resumed();
private:
    int setupTTY();
private slots:
    void slotSigNotifier(int);
    void slotPauseDevice(uint, uint, const QString&);
private:
    QString mSession;
    QString mSeat;
    int mSigFD[2];
    QSocketNotifier *mSNSig;
    int mDRMFd;

};

#endif
