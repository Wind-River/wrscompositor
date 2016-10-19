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

import QtQuick 2.1
import QtMultimedia 5.0
import Qt.labs.folderlistmodel 2.1

Item {
    id: playlist

    property int index: 0
    property MediaPlayer mediaPlayer
    property FolderListModel fm

    function setIndex(i) {
        index = i;
        console.log("setting index to: " + i);
        index %= fm.count;
        mediaPlayer.source = "file://" + fm.get(index, "filePath");
        console.log("setting source to: " + mediaPlayer.source);
    }

    function next() {
        setIndex(index + 1);
    }

    Connections {
        target: playlist.mediaPlayer

        onStopped: {
            if (playlist.mediaPlayer.status == MediaPlayer.Buffered) {
                console.log("onStopped, Try to play next-media");
                playlist.next();
                playlist.mediaPlayer.play();
            } else {
                console.log("unchecked status " + playlist.mediaPlayer.status);
            }
        }

        onError: {
            if (MediaPlayer.NoError != error) {
                console.warn("error " + error);
                console.warn("errorString " + errorString)
            }
        }
    }
}
