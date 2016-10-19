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

#ifndef PROCESS_H
#define PROCESS_H

#include <QObject>
#include <QVariant>
#include <QProcess>

class Process : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int pid READ getPID NOTIFY pidChanged);
    Q_PROPERTY(int windowDefaultWidth READ getWDW WRITE setWDW NOTIFY wdwChanged);
    Q_PROPERTY(int windowDefaultHeight READ getWDH WRITE setWDH NOTIFY wdhChanged);
    Q_PROPERTY(QString cmd READ getCmd NOTIFY cmdChanged);
public:
    explicit Process(QObject *parent = 0);
    int getPID();
    int getWDW() {
        return mWDW;
    };
    void setWDW(int width) {
        mWDW = width;
    };
    int getWDH() {
        return mWDH;
    };
    void setWDH(int height) {
        mWDH = height;
    };
    QString getCmd() const;
    Q_INVOKABLE bool execute(const QString &cmd);
    Q_INVOKABLE bool quit();

public:
    static QString WAYLAND_DISPlAY;

signals:
    void pidChanged(int pid);
    void cmdChanged(const QString &cmd);
    void wdwChanged(int width);
    void wdhChanged(int height);

private slots:
    void slotStarted();
    void slotCompleted(int exitStatus);
    void slotStandardOutput();
    void slotStandardError();
private:
    QProcess *mProcess;
    QString mCmd;
    int mWDW;
    int mWDH;
};

#endif
