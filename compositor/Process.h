/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
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
