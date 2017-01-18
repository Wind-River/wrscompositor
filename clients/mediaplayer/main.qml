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

import QtQuick 2.2
import QtQuick.Window 2.2
import QtMultimedia 5.0
import Qt.labs.folderlistmodel 2.1

Item {
    id: root
    width: Screen.width
    height: Screen.height

    Playlist {
        id: playlist
        mediaPlayer: player

        fm: FolderListModel {
            id: fm
            folder: "file:///usr/share/wrscompositor"
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
