import QtQuick 2.1

Rectangle {
    id: statusBar
    width: parent.width
    height: parent.height
    color: Qt.rgba(0.3, 0.3, 0.3, 0.3)

    Rectangle {
        width: parent.width
        height: parent.height
        color: Qt.rgba(0.3, 0.3, 0.3, 0.3)
        Text {
            x: parent.width / 10;
            anchors.verticalCenter: parent.verticalCenter
            id: timerText;
            color: "white"
            text: Qt.formatDateTime(new Date(),"dd, dddd, MMMM  hh:mm:ss")
        }
        Image {
            id: logo
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            //width: parent.width
            //height: parent.height
            source: "icons/wr-red.png"
        }
    }

    Timer {
        id: timer
        interval: 1000
        repeat: true
        running: true
        onTriggered: {
            timerText.text =  Qt.formatDateTime(new Date(),"dd, dddd, MMMM  hh:mm:ss")
        }
    }
}
