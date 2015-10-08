#ifndef PROCESS_H
#define PROCESS_H

#include <QObject>
#include <QVariant>
#include <QProcess>

class Process : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int pid READ getPID NOTIFY pidChanged);
    Q_PROPERTY(QString cmd READ getCmd NOTIFY cmdChanged);
public:
    explicit Process(QObject *parent = 0);
    int getPID();
    QString getCmd() const;
    Q_INVOKABLE bool execute(const QString &cmd);
    Q_INVOKABLE bool quit();

signals:
    void pidChanged(int pid);
    void cmdChanged(const QString &cmd);

private slots:
    void slotStarted();
    void slotCompleted(int exitStatus);
    void slotStandardOutput();
    void slotStandardError();
private:
    QProcess *mProcess;
    QString mCmd;
};

#endif
