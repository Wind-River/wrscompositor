
#ifndef WR_DBUSCLIENT_H
#define WR_DBUSCLIENT_H

#include <QtCore/QObject>

class WRDBusClient: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int repeatState READ repeatState NOTIFY trackInfoChanged)
    Q_PROPERTY(int shuffleState READ shuffleState NOTIFY trackInfoChanged)
    Q_PROPERTY(unsigned int numberOfTracks READ numberOfTracks NOTIFY trackInfoChanged)
    Q_PROPERTY(unsigned int trackTimestamp READ trackTimeStamp NOTIFY trackInfoChanged)
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

    int repeatState() { return mRepeatState; };
    int shuffleState() { return mShuffleState; };
    unsigned int numberOfTracks() { return mNumberOfTracks; };
    unsigned int trackTimeStamp() { return mTrackTimeStamp; };
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
    void trackInfoChanged();
    void trackPositionChanged();
    void artworkChanged();

public slots:
    void slotTrackInfoChanged(int, int, uint, uint, uint, uint, uint, const QString &, const QString &, const QString &, const QString &);
    void slotPlayStateChanged(uint);
    void slotTrackPositionChanged(uint);
    void slotArtworkChanged(uint, uint, const QString &);

private:
    int mRepeatState;
    int mShuffleState;
    unsigned int mNumberOfTracks;
    unsigned int mTrackTimeStamp;
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
