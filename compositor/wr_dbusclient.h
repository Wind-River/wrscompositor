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

#ifndef WR_DBUSCLIENT_H
#define WR_DBUSCLIENT_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>

class WRDBusClient: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int connected READ connected NOTIFY connectionChanged)
    Q_PROPERTY(int repeatState READ repeatState NOTIFY trackInfoChanged)
    Q_PROPERTY(int shuffleState READ shuffleState NOTIFY trackInfoChanged)
    Q_PROPERTY(unsigned int numberOfTracks READ numberOfTracks NOTIFY trackInfoChanged)
    Q_PROPERTY(unsigned int trackIndex READ trackIndex NOTIFY trackInfoChanged)
    Q_PROPERTY(unsigned int trackLength READ trackLength NOTIFY trackInfoChanged)
    Q_PROPERTY(unsigned int playState READ playState NOTIFY trackInfoChanged)
    Q_PROPERTY(unsigned int hasArtwork READ hasArtwork NOTIFY trackInfoChanged)
    Q_PROPERTY(QString title READ title NOTIFY trackInfoChanged)
    Q_PROPERTY(QString chapter READ chapter NOTIFY trackInfoChanged)
    Q_PROPERTY(QString artist READ artist NOTIFY trackInfoChanged)
    Q_PROPERTY(QString album READ album NOTIFY trackInfoChanged)

    Q_PROPERTY(QString artwork READ artwork NOTIFY artworkChanged)
    Q_PROPERTY(unsigned int trackPosition READ trackPosition NOTIFY trackPositionChanged)

public:
    WRDBusClient();

    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();

    int connected() { return mConnected; };
    int repeatState() { return mRepeatState; };
    int shuffleState() { return mShuffleState; };
    unsigned int numberOfTracks() { return mNumberOfTracks; };
    int trackIndex() { return mTrackIndex; };
    unsigned int trackLength() { return mTrackLength; };
    unsigned int playState() { return mPlayState; };
    unsigned int hasArtwork() { return mHasArtwork; };
    const QString title() { return mTitle; };
    const QString chapter() { return mChapter; };
    const QString artist() { return mArtist; };
    const QString album() { return mAlbum; };

    const QString artwork() { return mArtwork; };
    unsigned int trackPosition() { return mTrackPosition; };

signals:
    void connectionChanged();
    void trackInfoChanged();
    void trackPositionChanged();
    void artworkChanged();

public slots:
    void slotTrackInfoChanged(int, int, uint, int, uint, uint, uint, const QString &, const QString &, const QString &, const QString &);
    void slotPlayStateChanged(uint);
    void slotTrackPositionChanged(uint);
    //void slotArtworkChanged(uint, uint, const QString &);
    void slotArtworkChanged(uint, uint);
    void slotConnected();
    void slotDisconnected();
    void slotTrackChanged(int);

private:
    int mConnected;

    int mRepeatState;
    int mShuffleState;
    unsigned int mNumberOfTracks;
    unsigned int mTrackIndex;
    unsigned int mTrackLength;
    unsigned int mPlayState;
    unsigned int mHasArtwork;
    QString mTitle;
    QString mChapter;
    QString mArtist;
    QString mAlbum;

    QString mArtwork;
    unsigned int mTrackPosition;
};

#endif
