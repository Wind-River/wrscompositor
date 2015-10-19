import QtQuick 2.1

Item {
    id: statusBar
    anchors.top: parent.top
    width: parent.width
    height: parent.height/12
    z: 50000
    property string wmi: ""
    property bool androidAutoEnabled: false

    signal closeWindow
    signal logoClicked

    Rectangle {
        id: statusbarBackground
        color: "#2e2e2e"
        anchors.fill: parent
    }

    /*
    Image {
        id: logo
        //source: "images/tizen-on-dark-small.png"
        source: "icons/genivi-notext.png"
        anchors.left: parent.left
        anchors.leftMargin: width/10
        anchors.verticalCenter: parent.verticalCenter
        width: (height*sourceSize.width)/sourceSize.height
        height: statusBar.height
        opacity: (logoButtonArea.pressed?0.8:1.0)
        smooth: true
        MouseArea {
            id: logoButtonArea
            anchors.fill: parent
            onClicked: {
                logoClicked();
            }
        }
    }
    */

    Image {
        id: logo_title
        //source: "images/wr.png"
        //height: statusBar.height * 0.4
        //anchors.left: logo.right
        source: "images/wr-red.png"
        height: statusBar.height * 0.7
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        width: (height*sourceSize.width)/sourceSize.height
        smooth: true
        //visible: statusBar.wmi == ""
        opacity: (logoButtonArea.pressed?0.8:1.0)
        MouseArea {
            id: logoButtonArea
            anchors.fill: parent
            onClicked: {
                logoClicked();
            }
        }
    }
    Text {
        id: wmi_title
        anchors.left: logo_title.right
        anchors.verticalCenter: parent.verticalCenter
        text: statusBar.wmi
        visible: statusBar.wmi != ""
        font.pointSize: statusBar.height*0.4
        color: "white"
        smooth: true
        font.bold: true
    }

    Image {
        id: androidAuto
        source: "icons/android-logo.png"
        anchors.right: bluetooth.left
        anchors.rightMargin: width/20
        anchors.verticalCenter: parent.verticalCenter
        width: (height*sourceSize.width)/sourceSize.height
        height: statusBar.height * 0.75
        smooth: true
		visible: statusBar.androidAutoEnabled
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
        font.pointSize: statusBar.height*0.4
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
    Image {
        id: closeButton
        source: "icons/menu-close.png"
        anchors.right: parent.right
        anchors.rightMargin: width/8
        width: height
        height: parent.height * 3 / 4
        y: (parent.height - height)/2
        MouseArea {
            id: buttonArea
            anchors.fill: parent
            onClicked: {
                closeWindow();
                closeButton.width = 0;
            }
        }
        opacity: (buttonArea.pressed?0.8:1.0)
        scale: (buttonArea.pressed?0.9:1.0)
        smooth: true
    }

    function showCloseButton(flag) {
        closeButton.width = flag?closeButton.height:0;
    }

    function setWMI(wmi) {
        statusBar.wmi = wmi
    }

}

