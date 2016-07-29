/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
import QtQuick 2.1
import QtMultimedia 5.0
import "compositor.js" as CompositorLogic
import "config.js" as Conf
import "sprintf.js" as SPrintf
import com.windriver.automotive 1.0
import com.windriver.genivi 1.0

Rectangle {
	width: parent.width
	height: parent.height
	anchors.top: parent.top
	color: "black"

	property variant projectionView: projectionView

	VideoOutput {
		id: projectionView
		source: mediaPlayer
		width: parent.width
		height: parent.height
		MediaPlayer {
			id: mediaPlayer
			autoLoad: false
			loops: Audio.Infinite
			onError: {
				if (MediaPlayer.NoError != error) {
					console.log("[41m[qmlvideo][0m VideoItem.onError error " + error + " errorString " + errorString)
				}
			}
		}
	}
}