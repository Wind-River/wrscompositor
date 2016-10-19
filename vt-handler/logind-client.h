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
