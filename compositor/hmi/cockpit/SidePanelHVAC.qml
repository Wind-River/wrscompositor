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
    Component.onCompleted: {
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


