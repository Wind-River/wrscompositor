#include "duduregiconfig.h"
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

ProjectionModePrivate::ProjectionModePrivate(QObject *parent) :
    QObject(parent)
{

}

ProjectionMode::ProjectionMode(QObject *parent) :
    QTcpServer(parent), mMediaPlayer(0)
{
    mPM = new ProjectionModePrivate();
    new ProjectionModeAdaptor(mPM);
    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerObject("/AndroidAuto", mPM);
    connection.registerService("com.windriver.automotive.ProjectionMode");

    listen(QHostAddress(QHostAddress::LocalHost), 32323);
}
void ProjectionMode::sendMousePressed(int x, int y) {
    emit mPM->touchEvent(x, y, 1);
};
void ProjectionMode::sendKeyPressed(int keycode) {
    emit mPM->keyEvent(keycode, true);
}
void ProjectionMode::sendMouseReleased(int x, int y) {
    emit mPM->touchEvent(x, y, 0);
};
void ProjectionMode::sendKeyReleased(int keycode) {
    emit mPM->keyEvent(keycode, false);
}

void ProjectionMode::setMediaPlayer(QObject *obj) {
    qDebug() << "[42m haha [0m" << obj->property("source");;
    mMediaPlayer = qvariant_cast<QMediaPlayer *>(obj->property("mediaObject"));
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



ProjectionStream::ProjectionStream(QMediaPlayer *player, QObject *parent) :
    QTcpSocket(parent), mMediaPlayer(player), mInit(false), mPrepareBufferLength(0)
{
    mBuffer.open(QBuffer::ReadWrite);
    QObject::connect(this, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    QObject::connect(this, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
}

void ProjectionStream::slotDisconnected() {
    qDebug() << "disconnected";
    mMediaPlayer->stop();
    mMediaPlayer->setMedia(0, 0);
    qDebug() << "mediaStatus" << mMediaPlayer->mediaStatus();
}

void ProjectionStream::slotReadyRead() {
    qint64 r = bytesAvailable();
    if(mPrepareBufferLength<2000000)
        mPrepareBufferLength += r;
    qDebug() << "r" << r;
    if(mPrepareBufferLength > 2000000 && QMediaPlayer::PlayingState != mMediaPlayer->state() && r>0) {
        qDebug() << "mediaStatus" << mMediaPlayer->mediaStatus();
        if(QMediaPlayer::NoMedia == mMediaPlayer->mediaStatus())
            mMediaPlayer->setMedia(0, this);

        if(QMediaPlayer::InvalidMedia != mMediaPlayer->mediaStatus())
            mMediaPlayer->play();
        qDebug() << "mMediaPlayer->play()";
    }
}
