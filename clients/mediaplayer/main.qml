/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
import QtQuick 2.0
import QtMultimedia 5.0
import Qt.labs.folderlistmodel 2.1

Item {
    id: root
    anchors.fill: parent

    Playlist {
        id: playlist
        mediaPlayer: player

        fm: FolderListModel {
            id: fm
            folder: "file:///home/windriver/Videos"
            showDirs: false
            showDotAndDotDot: false
            nameFilters: ["*.ogv"]
            property bool ready: count > 0
            // startup initialization;
            onReadyChanged: {
                if (player.status == MediaPlayer.NoMedia) {
                    playlist.setIndex(0);
                    player.play();
                }
            }
        }
    }

    MediaPlayer {
        id: player
    }

    VideoOutput {
        source: player
        anchors.fill: parent
        fillMode: VideoOutput.Stretch
        visible: true;
    }
}
