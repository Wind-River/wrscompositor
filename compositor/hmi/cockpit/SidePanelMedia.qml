/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
import QtQuick 2.1
import QtMultimedia 5.0
import QtGraphicalEffects 1.0

Item {
    id: root
    property variant soundcast_playlist: null
    property int soundcast_index: -1
    property string soundcast_clientid: "c21ebb25eb644205d93434032620af47"

    FontLoader { id: tungsten; source: "fonts/Tungsten-Light.otf" }

    Component.onCompleted: {
        var doc = new XMLHttpRequest();
        doc.onreadystatechange = function() {
            if (doc.readyState == XMLHttpRequest.HEADERS_RECEIVED) {
                console.log('header received');
                console.log(doc.status);
                console.log(doc.readyState);
                console.log(doc.getAllResponseHeaders ());
                console.log(doc.getResponseHeader ("Last-Modified"));
            } else if (doc.readyState == XMLHttpRequest.DONE) {
                console.log('done')
                if(doc.status==200) {
                    console.log(doc.readyState);
                    root.soundcast_playlist = JSON.parse(doc.responseText);
                    mediaplayer.init();
                } else {
                    // error
                }
            }
        }
        doc.onerror = function() {
            // error
        }
        doc.open("GET", "https://api.soundcloud.com/tracks?client_id="+root.soundcast_clientid);
        doc.send();
    }
    Image {
        id: artwork
        width: root.width * 8 / 20
        height: width
        anchors.left: root.left
        anchors.top: root.top
        anchors.leftMargin: root.width / 30
        anchors.topMargin: root.width / 30
        smooth: true
    }
    Rectangle {
        id: waveformbg
        anchors.left: artwork.right
        anchors.leftMargin: root.width / 40
        anchors.bottom: artwork.bottom
        height: artwork.height / 6
        width: root.width / 2
        color: "#3BBDED"
    }
    Image {
        id: waveform
        anchors.fill: waveformbg
        opacity: 0.5
    }
    Rectangle {
        id: waveposition
        anchors.left: waveform.left
        anchors.top: waveform.top
        height: waveform.height
        width: 0
        color: "red"
        opacity: 0.5
    }
    /*
    LevelAdjust {
        anchors.fill: waveform
        source: waveform
    }
    */
    Timer {
        id: playbackTimer
        interval: 1000; running: false; repeat: true
        onTriggered: {
            console.log("playback: "+mediaplayer.position+"/"+mediaplayer.duration);
            var duration = mediaplayer.duration;
            var position = mediaplayer.position;
            if(position < 0 || duration < 0)
                return;
            waveposition.width = parseInt(position*waveformbg.width/duration);
        }
    }
    MediaPlayer {
        id: mediaplayer
        function init() {
            console.log('mediaplayer init');
            if(root.soundcast_index == -1) {
                for(var i=0; i<root.soundcast_playlist.length; i++) {
                    var info = root.soundcast_playlist[i];
                    if(!info.artwork_url)
                        continue;
                    root.soundcast_index = i;
                    break;
                }
            }
            console.log(root.soundcast_index);
            console.log('title: '+info.title);
            var streamurl = info.stream_url + '?client_id='+root.soundcast_clientid
            console.log('streamurl: '+streamurl);
            var info = root.soundcast_playlist[root.soundcast_index];
            mediaplayer.source = streamurl
            mediaplayer.play();
        }
        onError: {
            console.log('onerror '+error+' '+errorString);
        }
        onPaused: {
            console.log('paused');
        }
        onPlaying: {
            console.log('playing');
            playbackTimer.start();
            var info = root.soundcast_playlist[root.soundcast_index];
            artwork.source = info.artwork_url;
            waveform.source = info.waveform_url;
        }
        onStopped: {
            console.log('stopped');
            playbackTimer.stop();
        }
        onPlaybackStateChanged: {
            console.log('playback state changed '+mediaplayer.playbackState);
            if(mediaplayer.playbackState==MediaPlayer.StoppedState) {
                // goto next
                var previndex = root.soundcast_index;
                for(var i=root.soundcast_index+1; i<root.soundcast_playlist.length; i++) {
                    var info = root.soundcast_playlist[i];
                    if(!info.artwork_url)
                        continue;
                    root.soundcast_index = i;
                    break;
                }
                if(previndex == root.soundcast_index) { // rewind to first
                    for(var i=0; i<root.soundcast_playlist.length; i++) {
                        var info = root.soundcast_playlist[i];
                        if(!info.artwork_url)
                            continue;
                        root.soundcast_index = i;
                        break;
                    }
                }
                if(root.soundcast_index > 0) {
                    console.log('soundcast_index changed to '+root.soundcast_index);
                    var info = root.soundcast_playlist[root.soundcast_index];
                    var streamurl = info.stream_url + '?client_id='+root.soundcast_clientid
                    mediaplayer.source = streamurl
                    mediaplayer.play();
                }
            }
        }

    }
}


