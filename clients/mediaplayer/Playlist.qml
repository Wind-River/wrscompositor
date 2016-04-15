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
