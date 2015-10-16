#ifndef PROJECTIONMODE_H
#define PROJECTIONMODE_H

#include <QObject>
#include <QVariant>
#include <QProcess>
#include <QtNetwork>
#include "projectionmode_adaptor.h"

class ProjectionModePrivate : public QObject
{
    Q_OBJECT
public:
    explicit ProjectionModePrivate(QObject *parent = 0);

Q_SIGNALS:
    void touchEvent(int x, int y, int button);
    void keyEvent(int keycode, int pressed);
    void focusEvent(bool acquired);

};

class ProjectionStream : public QTcpSocket
{
    Q_OBJECT
public:
    explicit ProjectionStream(QObject *parent = 0);
};

class ProjectionMode : public QTcpServer
{
    Q_OBJECT
public:
    explicit ProjectionMode(QObject *parent = 0);
    Q_INVOKABLE void sendMousePressed(int x, int y);
    Q_INVOKABLE void sendKeyPressed(int keycode);
    Q_INVOKABLE void sendMouseReleased(int x, int y);
    Q_INVOKABLE void sendKeyReleased(int keycode);
private:
    ProjectionModePrivate *mPM;

protected:
    void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;

};

#endif
