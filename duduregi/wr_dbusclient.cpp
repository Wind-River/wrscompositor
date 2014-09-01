#include <QDebug>
#include <QtDBus/QtDBus>

#include "wr_dbusclient.h"


WRDBusClient::WRDBusClient() {
    if (!QDBusConnection::sessionBus().isConnected()) {
        qDebug() << "Could not connect to the D-Bus session.\n";
        return;
    }
    mConnected = 0;
    mRepeatState = 0;
    mShuffleState = 0;
    mNumberOfTracks = 0;
    mTrackTimeStamp = 0;
    mTrackLength = 0;
    mPlayState = 0;
    mHasArtwork = 0;
    mTitle = "";
    mChapter = "";
    mArtist = "";
    mAlbum = "";
    mArtwork = "";
    mTrackPosition = 0;

    //QDBusConnection::sessionBus().registerService("com.windriver.duduregi");
    //QDBusConnection::sessionBus().registerObject("/", this);
    QDBusConnection::sessionBus().connect(QString(), QString(), "com.windriver.iAP1", "track_info", this, SLOT(slotTrackInfoChanged(int, int, uint, uint, uint, uint, uint, const QString &, const QString &, const QString &, const QString &)));
    QDBusConnection::sessionBus().connect(QString(), QString(), "com.windriver.iAP1", "playstate_changed", this, SLOT(slotPlayStateChanged(uint)));
    QDBusConnection::sessionBus().connect(QString(), QString(), "com.windriver.iAP1", "track_position_changed", this, SLOT(slotTrackPositionChanged(uint)));
    //QDBusConnection::sessionBus().connect(QString(), QString(), "com.windriver.iAP1", "artwork_info", this, SLOT(slotArtworkChanged(uint, uint, const QString &)));
    QDBusConnection::sessionBus().connect(QString(), QString(), "com.windriver.iAP1", "artwork_info", this, SLOT(slotArtworkChanged(uint, uint)));
    QDBusConnection::sessionBus().connect(QString(), QString(), "com.windriver.iAP1", "ipod_connected", this, SLOT(slotConnected()));
    QDBusConnection::sessionBus().connect(QString(), QString(), "com.windriver.iAP1", "ipod_disconnected", this, SLOT(slotDisconnected()));
}

void WRDBusClient::play() {
    QDBusConnection::sessionBus().call(QDBusMessage::createMethodCall ("com.windriver.automotive", "/iPod", "com.windriver.iAP1", "play"));
}

void WRDBusClient::pause() {
    QDBusConnection::sessionBus().call(QDBusMessage::createMethodCall ("com.windriver.automotive", "/iPod", "com.windriver.iAP1", "pause"));
}

void WRDBusClient::slotTrackPositionChanged(uint position) {
    if(mConnected != 1)
        slotConnected();
    mTrackPosition = position;
    emit trackPositionChanged();
}
void WRDBusClient::slotPlayStateChanged(uint playState) {
    if(mConnected != 1)
        slotConnected();
    mPlayState = playState;
    emit trackInfoChanged();
}

//void WRDBusClient::slotArtworkChanged(uint w, uint h, const QString &image) {
void WRDBusClient::slotArtworkChanged(uint w, uint h) {
    if(mConnected != 1)
        slotConnected();

    qDebug() << "artwork info" << w << h;

    QDBusReply<QString> reply = QDBusConnection::sessionBus().call(QDBusMessage::createMethodCall ("com.windriver.automotive", "/iPod", "com.windriver.iAP1", "current_artwork"));
    if(reply.isValid()) {
        qDebug() << "artwork valid";
        mArtwork = reply.value();
        emit artworkChanged();
    }
    else
        qDebug() << "artwork invalid";
}

void WRDBusClient::slotTrackInfoChanged(int repeatState, int shuffleState, uint numberOfTracks, uint trackTimeStamp, uint trackLength, uint playState, uint hasArtwork, const QString &title, const QString &chapter, const QString &artist, const QString &album) {
    if(mConnected != 1)
        slotConnected();
    mRepeatState =    repeatState;
    mShuffleState =   shuffleState;
    mNumberOfTracks = numberOfTracks;
    mTrackTimeStamp = trackTimeStamp;
    mTrackLength =    trackLength;
    mPlayState =      playState;
    mHasArtwork =     hasArtwork;
    mTitle =          title;
    mChapter =        chapter;
    mArtist =         artist;
    mAlbum =          album;
    emit trackInfoChanged();
}

void WRDBusClient::slotConnected() {
    mConnected = 1;
    emit connectionChanged();
}

void WRDBusClient::slotDisconnected() {
    mConnected = 0;
    emit connectionChanged();
}
