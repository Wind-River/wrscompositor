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
import "sprintf.js" as SPrintf

Item {
    id: root
    property variant playlist: null
    property int track_index: -1
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
                    var tracklist = JSON.parse(doc.responseText);
                    root.playlist = new Array();
                    for(var i=0; i<tracklist.length; i++) {
                        var info = tracklist[i];
                        if(!info.artwork_url)
                            continue;
                        playlist.push(info);
                    }
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
        source: "resources/album-cover.png"
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
        MouseArea {
            id: seekArea
            anchors.fill: parent
            onClicked: {
                var offset = parseInt(mouseX*mediaplayer.duration/width);
                mediaplayer.seek(offset);
                console.log("seek to "+offset);
            }
        }
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
    Text {
        id: tracktime
        text: "00:00 / 00:00"
        anchors.verticalCenter: waveform.verticalCenter
        anchors.horizontalCenter: waveform.horizontalCenter
        font.pointSize: waveform.height/2
        font.family: tungsten.name
        font.bold: true
        color: "white"
        smooth: true
    }
    /*
    LevelAdjust {
        anchors.fill: waveform
        source: waveform
    }
    */


    Item {
        id: playbuttonpanel
        anchors.left: artwork.right
        anchors.bottom: playinfopanel.top
        width: waveformbg.width
        height: (artwork.height - waveformbg.height)/2

        RingIcon {
            id: playbutton
            icon: mediaplayer.playbackState==MediaPlayer.PlayingState?"resources/pause.svg":"resources/play.svg"
            width: playArea.pressed?root.width/7:root.width / 8
            height: width
            anchors.verticalCenter: playbuttonpanel.verticalCenter
            anchors.horizontalCenter: playbuttonpanel.horizontalCenter
            MouseArea {
                id: playArea
                anchors.fill: parent
                onClicked: {
                    if(mediaplayer.playbackState==MediaPlayer.PlayingState)
                        mediaplayer.pause()
                    else
                        mediaplayer.play()
                }
            }
        }
        Image {
            id: backward
            source: "resources/backward.svg"
            width: backwardArea.pressed?root.width/14:root.width/16
            height: width
            anchors.right: playbutton.left
            anchors.rightMargin: width
            anchors.verticalCenter: playbuttonpanel.verticalCenter
            MouseArea {
                id: backwardArea
                anchors.fill: parent
                onClicked: {
                    waveposition.width = 0;
                    root.track_index = root.track_index - 2;
                    if(root.track_index < -1)
                        root.track_index = -1;
                    mediaplayer.stop(); // to play previous track
                }
            }
        }
        Image {
            id: forward
            source: "resources/forward.svg"
            width: forwardArea.pressed?root.width/14:root.width/16
            height: width
            anchors.left: playbutton.right
            anchors.leftMargin: width
            anchors.verticalCenter: playbuttonpanel.verticalCenter
            MouseArea {
                id: forwardArea
                anchors.fill: parent
                onClicked: {
                    waveposition.width = 0;
                    mediaplayer.stop(); // to play next track
                }
            }
        }
    }
    Item {
        id: playinfopanel
        anchors.left: artwork.right
        anchors.bottom: waveformbg.top
        width: waveformbg.width
        height: (artwork.height - waveformbg.height)/2

        Text {
            id: username
            text: "Annonymous"
            anchors.bottom: title.top
            anchors.bottomMargin: parent.height/16
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: parent.height / 5
            font.family: tungsten.name
            font.bold: true
            color: "white"
            smooth: true
        }
        Text {
            id: title
            text: "No title"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.height/4
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: parent.height / 6
            font.family: tungsten.name
            font.bold: true
            color: "white"
            smooth: true
        }
    }
    Timer {
        id: playbackTimer
        interval: 1000
        running: mediaplayer.playbackState==MediaPlayer.PlayingState
        repeat: true
        onTriggered: {
            console.log("playback: "+mediaplayer.position+"/"+mediaplayer.duration+" "+mediaplayer.status);
            var duration = mediaplayer.duration;
            var position = mediaplayer.position;
            if(position < 0 || duration < 0)
                return;
            waveposition.width = parseInt(position*waveformbg.width/duration);

            if(mediaplayer.status == MediaPlayer.Buffering)
                tracktime.text = "Buffering ...";
            else if(mediaplayer.status == MediaPlayer.Stalled)
                tracktime.text = "Buffering ...";
            else
                tracktime.text = SPrintf.sprintf('%02d:%02d / %02d:%02d', parseInt(position/1000/60), parseInt((position/1000)%60), parseInt(duration/1000/60), parseInt((duration/1000)%60));
        }
    }
    MediaPlayer {
        id: mediaplayer
        function init() {
            console.log('mediaplayer init');
            if(root.playlist.length == 0)
                return;
            var track_index = 0;
            var info = root.playlist[track_index];
            console.log(track_index);
            console.log('title: '+info.title);
            var streamurl = info.stream_url + '?client_id='+root.soundcast_clientid
            console.log('streamurl: '+streamurl);
            mediaplayer.source = streamurl
            mediaplayer.play();
            root.track_index = -1;
        }
        onError: {
            console.log('onerror '+error+' '+errorString);
            mediaplayer.stop(); // to skip track
        }
        onPaused: {
            console.log('paused');
        }
        onPlaying: {
            console.log('playing');
            var track_index = 0;
            if(root.track_index >= 0)
                track_index = root.track_index;
            var info = root.playlist[track_index];
            artwork.source = info.artwork_url;
            waveform.source = info.waveform_url;
            var _username = info.user.username;
            var _title = info.title;
            //_username ='ABCDEFGHIJILMNOPQRSTUVWXYZABCDEFGHIJILMNOPQRSTUVWXYZ';
            //_title ='ABCDEFGHIJILMNOPQRSTUVWXYZABCDEFGHIJILMNOPQRSTUVWXYZ';
            if(_username && _username.length > 28)
                _username = _username.slice(0, 28)+'...';
            if(_title.length > 36)
                _title = _title.slice(0, 36)+'...';
            title.text = _title
            username.text = _username

        }
        onStopped: {
            console.log('stopped');
        }
        onPlaybackStateChanged: {
            console.log('playback state changed '+mediaplayer.playbackState);
            if(mediaplayer.playbackState==MediaPlayer.StoppedState) {
                // auto play the next track
                if((root.track_index+1) == root.playlist.length)
                    root.track_index = 0;
                else
                    root.track_index += 1;

                var info = root.playlist[root.track_index];
                var streamurl = info.stream_url + '?client_id='+root.soundcast_clientid
                mediaplayer.source = streamurl
                mediaplayer.play();
            }
        }

    }
}


