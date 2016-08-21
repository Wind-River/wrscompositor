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
    property variant colorArray: new Array('#43bbe3', '#4db8d8', '#57b6ce', '#61b4c3', '#6ab2b8', '#75b0ad', '#7eaea3', '#88ab98', '#92a98e', '#9ca783', '#a5a579', '#afa36d', '#b9a163', '#c39e58', '#d79a43', '#e19838', '#e58e26', '#f39424');

    width : parent.width
    height : parent.height

    FontLoader { id: tungsten; source: "fonts/Tungsten-Light.otf" }
    Rectangle {
        color: "#222222";
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 3
    }
    /*
    Rectangle {
        color: "black";
        anchors.fill: parent
        border.width: width/20
        border.color: "red"
    }
    */

    function hide() {
        root.state = 'hide';
    }

    function show() {
        root.state = 'show';
    }

    states: [
        State {
            name: "show"
            PropertyChanges { target: root; opacity: 1}
        },
        State {
            name: "hide"
            PropertyChanges { target: root; opacity: 0;}
        }
    ]

    transitions: [
        Transition {
            from: "show"
            to: "hide"
            ParallelAnimation {
                NumberAnimation {
                    target: root
                    properties: "opacity"
                    easing.type: Easing.InCubic
                    duration: 300
                }
            }
            onRunningChanged: {
                if ((state == "hide") && (!running)) {
                    console.log("onRunningChanged, finish hiding hvac's widget");
                    root.visible = false;
                }
            }
        },
        Transition {
            from: "hide"
            to: "show"
            ParallelAnimation {
                NumberAnimation {
                    target: root
                    properties: "x"
                    easing.type: Easing.InCubic
                    to: 0
                    duration: 500
                }
                NumberAnimation {
                    target: root
                    properties: "opacity"
                    easing.type: Easing.InCubic
                    duration: 500
                }
            }
            onRunningChanged: {
                if ((state == "show") && (running)) {
                    console.log("onRunningChanged, starting showing hvac's widget");
                    root.x = 700; // swipe in from right to left
                    root.visible = true;
                }
            }
        }
    ]

    Component.onCompleted: {
        root.show();
    }

    Text {
        id: label
        text: "TEMPERATURE"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: root.top
        anchors.topMargin: root.width / 20
        font.pointSize: root.width / 30
        font.family: tungsten.name
        font.bold: true
        color: "white"
        smooth: true
    }

    Item {
        id: volumePanel
        width: root.width
        height: root.width / 10
        anchors.top: label.bottom
        anchors.topMargin: label.height
        anchors.right: parent.right
        anchors.left: parent.left

        property int value: 20
        property int minimumValue: 0
        property int maximumValue: 40

        Canvas {
            id: "volumeSlider"
            width: volumePanel.width
            height: volumePanel.height
            anchors.top: volumePanel.top
            anchors.bottom: volumePanel.bottom
            anchors.right: volumePanel.right
            anchors.rightMargin: parent.width / 8
            anchors.left: parent.left
            anchors.leftMargin: parent.width / 8
            anchors.topMargin: parent.height*2/5
            anchors.bottomMargin: parent.height*2/5
            onPaint: {
                var ctx = getContext("2d");
                for(var i=0; i<colorArray.length; i++) {
                    var x = i*(width/colorArray.length);
                    ctx.fillStyle = colorArray[i];
                    ctx.fillRect(x, 0, 1, height);
                }
            }
            Rectangle {
                id: "volumeTip"
                color: colorArray[colorArray.length/2];
                width: volumePanel.height*1.5
                height: width
                x: (parent.width/2)-width/2
                anchors.verticalCenter: parent.verticalCenter
                radius: 100
                Text {
                    id: volumeText
                    text: "20°"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pointSize: parent.height/3
                    font.family: tungsten.name
                    font.bold: true
                    color: "white"
                    smooth: true
                }
            }
        }
        MouseArea {
            anchors.fill: parent
            function change(mouse) {
                var x = mouse.x - volumeSlider.x
                var pos = x / volumeSlider.width * (volumePanel.maximumValue - volumePanel.minimumValue) + volumePanel.minimumValue;
                volumePanel.value = pos
                volumeTip.x = x - volumeTip.width/2;
                volumeText.text = parseInt(pos)+"°";
                var colorIndex = parseInt(pos/2);
                if(colorIndex >= colorArray.length)
                    colorIndex = colorArray.length - 1;
                volumeTip.color = colorArray[colorIndex];
            }
            onClicked: {
                if(mouse.x >= volumeSlider.x && mouse.x <= (parent.width-(parent.width-(volumeSlider.x+volumeSlider.width))))
                    change(mouse);
            }
            onPositionChanged: {
                if(mouse.x >= volumeSlider.x && mouse.x <= (parent.width-(parent.width-(volumeSlider.x+volumeSlider.width))))
                    change(mouse);
            }
        }
    }

    Rectangle {
        color: "#222222";
        anchors.topMargin: volumePanel.height
        anchors.top: volumePanel.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 3
    }

    Item {
        id: heatedSeatsPanel
        anchors.top: volumePanel.bottom
        anchors.topMargin: volumePanel.height
        anchors.left: parent.left
        anchors.right: parent.right
        RingIcon {
            id: driverSeat
            icon: heated?"resources/seat-heat.svg":"resources/seat-normal.svg"
            innerScale: 0.65
            width: driverSeatArea.pressed?volumeTip.width*1.2:volumeTip.width
            height: width
            anchors.left: parent.left
            anchors.leftMargin: parent.width/10
            anchors.top: parent.top
            anchors.topMargin: volumePanel.height*2/3
            property bool heated: false
            MouseArea {
                id: driverSeatArea
                anchors.fill: parent
                onClicked: {
                    parent.heated = !parent.heated;
                }
            }
        }
        Text {
            id: seatlabel
            text: "HEATED SEATS"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: height*3
            font.pointSize: root.width / 30
            font.family: tungsten.name
            font.bold: true
            color: "white"
            smooth: true
        }

        RingIcon {
            id: passengerSeat
            icon: heated?"resources/seat-heat.svg":"resources/seat-normal.svg"
            innerScale: 0.65
            width: passengerSeatArea.pressed?volumeTip.width*1.2:volumeTip.width
            height: width
            anchors.right: parent.right
            anchors.rightMargin: parent.width/10
            anchors.top: parent.top
            anchors.topMargin: volumePanel.height*2/3
            property bool heated: true
            MouseArea {
                id: passengerSeatArea
                anchors.fill: parent
                onClicked: {
                    parent.heated = !parent.heated;
                }
            }
        }
        Text {
            id: label1
            text: "DRIVER"
            anchors.horizontalCenter: driverSeat.horizontalCenter
            anchors.top: driverSeat.bottom
            anchors.topMargin: height*2/3
            font.pointSize: root.width / 40
            font.family: tungsten.name
            color: "white"
            smooth: true
        }
        Text {
            id: label2
            text: "PASSENGER"
            anchors.horizontalCenter: passengerSeat.horizontalCenter
            anchors.top: passengerSeat.bottom
            anchors.topMargin: height*2/3
            font.pointSize: root.width / 40
            font.family: tungsten.name
            color: "white"
            smooth: true
        }
    }

}

