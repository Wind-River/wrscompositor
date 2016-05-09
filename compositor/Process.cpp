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
#include <QDirIterator>

QString Process::WAYLAND_DISPlAY = "wayland-0";
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
    env << QString("WAYLAND_DISPLAY=%1").arg(Process::WAYLAND_DISPlAY);
    //env << "LD_LIBRARY_PATH=/opt/workspaces/wl/out/lib";
    //env << "QT_XCB_GL_INTEGRATION=xcb_egl";
    mProcess->setEnvironment(env);
#endif
    mProcess->setWorkingDirectory(getenv("HOME"));
    connect(mProcess, SIGNAL(started()), this, SLOT(slotStarted()));
    connect(mProcess, SIGNAL(finished(int)), this, SLOT(slotCompleted(int)));
    connect(mProcess, SIGNAL(readyReadStandardError()), this, SLOT(slotStandardError()));
    connect(mProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(slotStandardOutput()));
    QStringList args = cmd.split(" ");
    QStringList paths;
    if(QFileInfo::exists("../clients")) {
        QDirIterator it("../clients", QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString subpath = it.next();
            QFileInfo fi(subpath);
            if(subpath != "." && subpath != ".." && !subpath.endsWith(".") && fi.isDir())
                paths << subpath;
        }
    }
    paths << "/opt/windriver/bin";
    paths << "/usr/local/bin";
    paths << "/usr/bin";
    if(cmd.startsWith("..")) {
        QFileInfo fi(args[0]);
        mProcess->setWorkingDirectory(fi.dir().absolutePath());
        mProcess->start(args[0], args.mid(1));
    } else if(!cmd.startsWith("/")) {
        Q_FOREACH (const QString &dirpath, paths) {
            if(QFileInfo::exists(dirpath+"/"+args[0])) {
                if(dirpath.startsWith("..")) {
                    QFileInfo fi(dirpath);
                    mProcess->setWorkingDirectory(fi.dir().absolutePath());
                }
                mProcess->start(dirpath+"/"+args[0], args.mid(1));
                return true;
            }
        }
        return false;
    } else
        mProcess->start(args[0], args.mid(1));
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
