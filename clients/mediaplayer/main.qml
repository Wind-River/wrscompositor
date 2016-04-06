/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
import QtQuick 2.0
import QtMultimedia 5.0

Item {
    anchors.fill: parent
    MediaPlayer {
        id: mediaplayer
        source: "file:///home/windriver/sample.ogv"
        autoLoad: true
        loops: Audio.Infinite
        onError: {
            if (MediaPlayer.NoError != error) {
                console.warn("error " + error);
                console.warn("errorString " + errorString)
            }
        }
    }
    VideoOutput {
        source: mediaplayer
        anchors.fill: parent
        visible: true;
    }
    Component.onCompleted: {
        mediaplayer.play();
        console.warn('souce: '+mediaplayer.source);
        console.warn('status: '+mediaplayer.playbackState);
    }
}
