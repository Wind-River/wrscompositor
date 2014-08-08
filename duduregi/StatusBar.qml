import QtQuick 2.0

Rectangle {
    id: statusBar
    color: "#121212"
    anchors.top: parent.top
    width: parent.width
    height: parent.height/12
    z: 50000

    signal closeWindow

    Image {
        id: tizen
        source: "images/tizen-on-dark-small.png"
        anchors.left: parent.left
        anchors.leftMargin: width/10
        anchors.verticalCenter: parent.verticalCenter
        width: (height*sourceSize.width)/sourceSize.height
        height: statusBar.height
        smooth: true
        /*
        RotationAnimation on rotation {
            loops: Animation.Infinite
            from: 360
            to: 0
            duration: 10000
        }
        */

    }
    Image {
        id: logo
        source: "images/wr.png"
        anchors.left: tizen.right
        anchors.verticalCenter: parent.verticalCenter
        width: (height*sourceSize.width)/sourceSize.height
        height: statusBar.height * 0.4
        smooth: true
    }
    Image {
        id: bluetooth
        source: "images/bt.png"
        anchors.right: dateTime.left
        anchors.rightMargin: width/20
        anchors.verticalCenter: parent.verticalCenter
        width: (height*sourceSize.width)/sourceSize.height
        height: statusBar.height * 0.75
        smooth: true
    }
    Text {
        id: dateTime
        anchors.right: closeButton.left
        anchors.rightMargin: width/20
        anchors.verticalCenter: parent.verticalCenter
        text: Qt.formatDateTime(new Date(), "yyyy/MM/dd hh:mm:ss")
        font.pointSize: statusBar.height/2
        color: "white"
        smooth: true
    }

    Timer {
        interval: 1000; running: true; repeat: true
        onTriggered: dateTime.text = Qt.formatDateTime(new Date(), "yyyy/MM/dd hh:mm:ss")
    } 

    /*
    Image {
        id: closeButton
	source: "images/close.png"
        anchors.verticalCenter: parent.verticalCenter
        width: (height*sourceSize.width)/sourceSize.height
        height: statusBar.height * 0.75
        smooth: true
    }
    */
    Rectangle {
        id: closeButton
        anchors.right: parent.right
        color: "#404040"
        Text {
            anchors.fill: parent
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: "X"
            color: "white"
            font.pointSize: parent.height
            font.bold: !buttonArea.pressed
            visible: parent.width>0
        }
        width: height
        height: parent.height
        MouseArea {
            id: buttonArea
            anchors.fill: parent
            onClicked: {
                closeWindow(); 
                closeButton.width = 0;
            }
        }
    }

    function showCloseButton(flag) {
        closeButton.width = flag?closeButton.height:0;
    }

}

