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

#include "config.h"
#include "projectionmode.h"
#include <QVariant>
#include <QMetaType>
#include <QTime>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <stdlib.h>
#include <QHostAddress>
#include <unistd.h>
#include <sys/types.h>


ProjectionModePrivate::ProjectionModePrivate(QObject *parent) :
    QObject(parent)
{

}

void ProjectionModePrivate::returnToNative() {
    qDebug() << __func__;
    emit returnToNativeCalled();
}

ProjectionMode::ProjectionMode(QObject *parent) :
    QTcpServer(parent), mMediaPlayer(0)
{
    mPM = new ProjectionModePrivate();
    connect(mPM, SIGNAL(returnToNativeCalled()), this, SLOT(slotReturnToHomeRequested()));
    new ProjectionModeAdaptor(mPM);
    QDBusConnection connection = QDBusConnection::sessionBus();
    if(qApp->arguments().contains("--dbus-systembus")) {
        qDebug() << "connection to systemBus";
        connection = QDBusConnection::systemBus();
    }
    qDebug() << "register" << connection.registerObject("/ConnectivityProjection", mPM);
    qDebug() << "register" << connection.registerService("com.windriver.automotive.ProjectionMode");

    listen(QHostAddress(QHostAddress::LocalHost), 32323);
}
void ProjectionMode::slotReturnToHomeRequested() {
    emit returnToHomeRequested();
}
void ProjectionMode::sendMouseMove(int id, int x, int y) {
    emit mPM->touchEvent(id, x, y, 2);
};
void ProjectionMode::sendMousePressed(int id, int x, int y) {
    emit mPM->touchEvent(id, x, y, 1);
};
void ProjectionMode::sendKeyPressed(int keycode) {
    emit mPM->keyEvent(keycode, true);
}
void ProjectionMode::sendMouseReleased(int id, int x, int y) {
    emit mPM->touchEvent(id, x, y, 0);
};
void ProjectionMode::sendKeyReleased(int keycode) {
    emit mPM->keyEvent(keycode, false);
}
void ProjectionMode::sendVideoFocus(int who, bool acquired) {
    if (who == ANDROID_AUTO)
        emit mPM->androidAutoVideoFocusEvent(acquired);
    else if (who == APPLE_CARPLAY)
        emit mPM->appleCarPlayVideoFocusEvent(acquired);
}

void ProjectionMode::setMediaPlayer(QObject *obj) {
    mMediaPlayer = qvariant_cast<QMediaPlayer *>(obj->property("mediaObject"));
    connect(mMediaPlayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(slotMediaStatusChanged(QMediaPlayer::MediaStatus)));
};
void ProjectionMode::incomingConnection(qintptr socketDescriptor) {
    /*
    QFile *video = new QFile("data/androidauto.h264");
    video->open(QIODevice::ReadOnly);
    mMediaPlayer->setMedia(0, video);
    mMediaPlayer->play();
    */
    qDebug() << "incomming";
    ProjectionStream *stream = new ProjectionStream(mMediaPlayer, this);
    stream->setSocketDescriptor(socketDescriptor);
    qDebug() << "mediaStatus" << mMediaPlayer->mediaStatus();
}

void ProjectionMode::slotMediaStatusChanged(QMediaPlayer::MediaStatus status) {
    qDebug() << "mediaStatus changed to " << status;

    if(status == QMediaPlayer::LoadedMedia && QMediaPlayer::PlayingState != mMediaPlayer->state()) {
        mMediaPlayer->play();
        qDebug() << "play!";
    }
}


ProjectionStream::ProjectionStream(QMediaPlayer *player, QObject *parent) :
    QTcpSocket(parent), mMediaPlayer(player), mInit(false), mPrepareBufferLength(0)
{
    mBuffer.open(QBuffer::ReadWrite);
    QObject::connect(this, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    QObject::connect(this, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
}

qint64 ProjectionStream::bytesAvailable() const {
    //qDebug() << "bytesAvailable ??" << QTcpSocket::bytesAvailable();
    qint64 r = QTcpSocket::bytesAvailable();
    if( r == 0 ) {
        return 1;
    }
    return r;
}

bool ProjectionStream::atEnd() const {
    return false;
}
void ProjectionStream::slotDisconnected() {
    qDebug() << "disconnected";
    mMediaPlayer->stop();
    mMediaPlayer->setMedia(0, 0);
    mPrepareBufferLength = 0;
    qDebug() << "mediaStatus" << mMediaPlayer->mediaStatus();
}

void ProjectionStream::slotReadyRead() {
    qint64 r = bytesAvailable();
    if(mPrepareBufferLength<100000)
        mPrepareBufferLength += r;
    //qDebug() << "r" << r;
    if(mPrepareBufferLength > 100000 && QMediaPlayer::PlayingState != mMediaPlayer->state() && r>0) {
        qDebug() << "mediaStatus" << mMediaPlayer->mediaStatus();
        if(QMediaPlayer::NoMedia == mMediaPlayer->mediaStatus())
            mMediaPlayer->setMedia(QMediaContent(QUrl("udp://")), this);

    }
}
