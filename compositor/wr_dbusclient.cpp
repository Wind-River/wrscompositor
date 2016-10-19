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

#include <QDebug>
#include <QtDBus/QtDBus>

#include "wr_dbusclient.h"


WRDBusClient::WRDBusClient() {
    if (!QDBusConnection::systemBus().isConnected()) {
        qDebug() << "Could not connect to the D-Bus session.\n";
        return;
    }
    mConnected = 0;
    mRepeatState = 0;
    mShuffleState = 0;
    mNumberOfTracks = 0;
    mTrackIndex = 0;
    mTrackLength = 0;
    mPlayState = 0;
    mHasArtwork = 0;
    mTitle = "";
    mChapter = "";
    mArtist = "";
    mAlbum = "";
    mArtwork = "";
    mTrackPosition = 0;

    //QDBusConnection::systemBus().registerService("com.windriver.wrscompositor");
    //QDBusConnection::systemBus().registerObject("/", this);
    QDBusConnection::systemBus().connect(QString(), QString(), "com.windriver.iAP1", "track_info", this, SLOT(slotTrackInfoChanged(int, int, uint, int, uint, uint, uint, const QString &, const QString &, const QString &, const QString &)));
    QDBusConnection::systemBus().connect(QString(), QString(), "com.windriver.iAP1", "playstate_changed", this, SLOT(slotPlayStateChanged(uint)));
    QDBusConnection::systemBus().connect(QString(), QString(), "com.windriver.iAP1", "track_position_changed", this, SLOT(slotTrackPositionChanged(uint)));
    //QDBusConnection::systemBus().connect(QString(), QString(), "com.windriver.iAP1", "artwork_info", this, SLOT(slotArtworkChanged(uint, uint, const QString &)));
    QDBusConnection::systemBus().connect(QString(), QString(), "com.windriver.iAP1", "artwork_info", this, SLOT(slotArtworkChanged(uint, uint)));
    QDBusConnection::systemBus().connect(QString(), QString(), "com.windriver.iAP1", "ipod_connected", this, SLOT(slotConnected()));
    QDBusConnection::systemBus().connect(QString(), QString(), "com.windriver.iAP1", "ipod_disconnected", this, SLOT(slotDisconnected()));
    QDBusConnection::systemBus().connect(QString(), QString(), "com.windriver.iAP1", "track_changed", this, SLOT(slotTrackChanged(int)));
}

void WRDBusClient::play() {
    QDBusConnection::systemBus().call(QDBusMessage::createMethodCall ("com.windriver.automotive", "/iPod", "com.windriver.iAP1", "play"));
}

void WRDBusClient::pause() {
    QDBusConnection::systemBus().call(QDBusMessage::createMethodCall ("com.windriver.automotive", "/iPod", "com.windriver.iAP1", "pause"));
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

    QDBusReply<QString> reply = QDBusConnection::systemBus().call(QDBusMessage::createMethodCall ("com.windriver.automotive", "/iPod", "com.windriver.iAP1", "current_artwork"));
    if(reply.isValid()) {
        qDebug() << "artwork valid";
        mArtwork = reply.value();
        emit artworkChanged();
    }
    else
        qDebug() << "artwork invalid";
}

void WRDBusClient::slotTrackInfoChanged(int repeatState, int shuffleState, uint numberOfTracks, int trackIndex, uint trackLength, uint playState, uint hasArtwork, const QString &title, const QString &chapter, const QString &artist, const QString &album) {
    if(mConnected != 1)
        slotConnected();
    mRepeatState =    repeatState;
    mShuffleState =   shuffleState;
    mNumberOfTracks = numberOfTracks;
    mTrackIndex =     trackIndex;
    mTrackLength =    trackLength;
    mPlayState =      playState;
    mHasArtwork =     hasArtwork;
    mTitle =          title;
    mChapter =        chapter;
    mArtist =         artist;
    mAlbum =          album;
    emit trackInfoChanged();
    QDBusMessage message = QDBusMessage::createMethodCall ("com.windriver.automotive", "/iPod", "com.windriver.iAP1", "get_artwork");
    message << trackIndex;
    QDBusConnection::systemBus().call(message);
}

void WRDBusClient::slotConnected() {
    mConnected = 1;
    QDBusConnection::systemBus().call(QDBusMessage::createMethodCall ("com.windriver.automotive", "/iPod", "com.windriver.iAP1", "get_trackinfo"));
    emit connectionChanged();
}

void WRDBusClient::slotDisconnected() {
    mConnected = 0;
    emit connectionChanged();
}

void WRDBusClient::slotTrackChanged(int index) {
    qDebug() << "track changed" << index;
    if(index >= 0)
        QDBusConnection::systemBus().call(QDBusMessage::createMethodCall ("com.windriver.automotive", "/iPod", "com.windriver.iAP1", "get_trackinfo"));
}
