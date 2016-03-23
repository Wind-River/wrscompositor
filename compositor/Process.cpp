/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
#include "duduregiconfig.h"
#include "Process.h"
#include <QVariant>
#include <QMetaType>
#include <QTime>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <stdlib.h>

//Q_DECLARE_METATYPE(QList<QObject*>)

Process::Process(QObject *parent) :
    QObject(parent), mProcess(0)
{
}
bool Process::execute(const QString &cmd)
{
    mCmd = cmd;
    emit cmdChanged(mCmd);
    qDebug() << "Process execute: " << cmd;
    if(mProcess)
        delete mProcess;
    mProcess = new QProcess();
#if DUDUREGI_WAYLAND_COMPOSITOR
    QStringList env = QProcess::systemEnvironment();
    env << "QT_QPA_PLATFORM=wayland";
    //env << "LD_LIBRARY_PATH=/opt/workspaces/wl/out/lib";
    //env << "QT_XCB_GL_INTEGRATION=xcb_egl";
    mProcess->setEnvironment(env);
#endif
    mProcess->setWorkingDirectory(getenv("HOME"));
    connect(mProcess, SIGNAL(started()), this, SLOT(slotStarted()));
    connect(mProcess, SIGNAL(finished(int)), this, SLOT(slotCompleted(int)));
    connect(mProcess, SIGNAL(readyReadStandardError()), this, SLOT(slotStandardError()));
    connect(mProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(slotStandardOutput()));
    QStringList args;
    QStringList paths;
    paths << "/opt/windriver/bin";
    paths << "/usr/local/bin";
    paths << "/usr/bin";
    if(!cmd.startsWith("/")) {
        Q_FOREACH (const QString &dirpath, paths) {
            if(QFileInfo::exists(dirpath+"/"+cmd.split(" ")[0])) {
                qDebug() << dirpath+"/"+cmd;
                mProcess->start(dirpath+"/"+cmd, args);
                break;
            }
        }
        return false;
    } else
        mProcess->start(cmd, args);
    return true;
}
QString Process::getCmd() const {
    return mCmd;
}
int Process::getPID() {
    if(mProcess)
        return mProcess->pid();
    return 0;
}
bool Process::quit()
{
    if(mProcess)
        //mProcess->terminate(); // try to terminate
        mProcess->kill(); // just kill (XXX term does not work on minnow)
    return true;
}
void Process::slotStarted()
{
    emit pidChanged(mProcess->pid());
}
void Process::slotCompleted(int)
{
    emit pidChanged(0);
}
void Process::slotStandardOutput()
{
}
void Process::slotStandardError()
{
    qDebug() << mProcess->readAllStandardError();
}
