/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
#ifndef PROJECTIONMODE_H
#define PROJECTIONMODE_H

#include <QObject>
#include <QVariant>
#include <QProcess>
#include <QtNetwork>
#include <QMediaPlayer>
#include "projectionmode_adaptor.h"

#define ANDROID_AUTO  0
#define APPLE_CARPLAY 1

class ProjectionModePrivate : public QObject
{
    Q_OBJECT
public:
    explicit ProjectionModePrivate(QObject *parent = 0);

Q_SIGNALS:
    void touchEvent(int id, int x, int y, int pressed);
    void keyEvent(int keycode, int pressed);
    void androidAutoVideoFocusEvent(bool acquired);
    void appleCarPlayVideoFocusEvent(bool acquired);
public Q_SLOTS:
    void returnToNative();
signals:
    void returnToNativeCalled();
};

class ProjectionStream : public QTcpSocket
{
    Q_OBJECT
public:
    explicit ProjectionStream(QMediaPlayer *player, QObject *parent = 0);

protected:
    bool atEnd() const;
    qint64 bytesAvailable() const;

private slots:
    void slotReadyRead();
    void slotDisconnected();
private:
    QMediaPlayer *mMediaPlayer;
    QBuffer mBuffer;
    bool mInit;
    qint64 mPrepareBufferLength;
};

class ProjectionMode : public QTcpServer
{
    Q_OBJECT
    Q_PROPERTY(QObject* mediaPlayer WRITE setMediaPlayer);
public:
    explicit ProjectionMode(QObject *parent = 0);
    Q_INVOKABLE void sendMousePressed(int id, int x, int y);
    Q_INVOKABLE void sendKeyPressed(int keycode);
    Q_INVOKABLE void sendMouseMove(int id, int x, int y);
    Q_INVOKABLE void sendMouseReleased(int id, int x, int y);
    Q_INVOKABLE void sendKeyReleased(int keycode);
    Q_INVOKABLE void sendVideoFocus(int who, bool acquired);
    void setMediaPlayer(QObject *obj);

signals:
    void returnToHomeRequested();

private:
    ProjectionModePrivate *mPM;
    QMediaPlayer *mMediaPlayer;

protected:
    void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;

private slots:
    void slotMediaStatusChanged(QMediaPlayer::MediaStatus);
    void slotReturnToHomeRequested();

};

#endif
