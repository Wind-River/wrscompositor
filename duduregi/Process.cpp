#include "Process.h"
#include <QVariant>
#include <QMetaType>
#include <QTime>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <stdlib.h>

//Q_DECLARE_METATYPE(QList<QObject*>)

Process::Process(QObject *parent) :
    QObject(parent), mProcess(0)
{
}
bool Process::execute(const QString &cmd)
{
    qDebug() << "Process execute: " << cmd;
    if(mProcess)
        delete mProcess;
    mProcess = new QProcess();
    mProcess->setWorkingDirectory(getenv("HOME"));
    connect(mProcess, SIGNAL(started()), this, SLOT(slotStarted()));
    connect(mProcess, SIGNAL(finished(int)), this, SLOT(slotCompleted(int)));
    connect(mProcess, SIGNAL(readyReadStandardError()), this, SLOT(slotStandardError()));
    connect(mProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(slotStandardOutput()));
    QStringList args;
    mProcess->start(cmd, args);
    return true;
}
int Process::getPID() {
    if(mProcess)
        return mProcess->pid();
    return 0;
}
bool Process::quit()
{
    if(mProcess)
        mProcess->terminate();
    return true;
}
void Process::slotStarted()
{
    emit pidChanged(mProcess->pid());
}
void Process::slotCompleted(int exitStatus)
{
    if(exitStatus==0) {
        emit pidChanged(0);
    }
}
void Process::slotStandardOutput()
{
}
void Process::slotStandardError()
{
    qDebug() << mProcess->readAllStandardError();
}
