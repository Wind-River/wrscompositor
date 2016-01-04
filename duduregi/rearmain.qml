import QtQuick 2.1
import "config.js" as Conf
import com.windriver.automotive 1.0
import com.windriver.genivi 1.0

Rectangle {
    id: rearRoot
    color: "#ff0000"
    anchors.fill: parent
    property variant currentWindow: null
    property var cloneSurfaceItemList: null;

    signal swappedWindowRestoreRequested(var anObject)
    signal clonedSurfaceItemDestroyed(var anObject)

    Rectangle {
        id: statusBar
        anchors.top: parent.top
        width: parent.width
        height: parent.height/12
        z: 50000
        property bool currentWindowExposed: rearRoot.currentWindow && rearRoot.currentWindow.visible

        Rectangle {
            id: statusbarBackground
            color: "#2e2e2e"
            anchors.fill: parent
        }

        Image {
            id: logo_title
            source: "images/wr-red.png"
            height: statusBar.height * 0.7
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            width: (height*sourceSize.width)/sourceSize.height
            smooth: true
        }
        Text {
            id: wmi_title
            anchors.left: logo_title.right
            anchors.verticalCenter: parent.verticalCenter
            text: "Rear Monitor"
            font.pointSize: ((statusBar.height*0.4/Conf.densityAdjust)|0)
            color: "white"
            smooth: true
            font.bold: true
        }



        Image {
            id: swapWindowButton
            source: "icons/restore-swapped-window.svg"
            anchors.right: dateTime.left
            anchors.rightMargin: width
            anchors.verticalCenter: parent.verticalCenter
            visible: Conf.useMultiWaylandDisplayFeature && statusBar.currentWindowExposed
            width: height
            height: parent.height * 3 / 5
            MouseArea {
                id: swapButtonArea
                anchors.fill: parent
                onClicked: {
                    if(!rearRoot.currentWindow)
                        return;
                    rearRoot.currentWindow.position = "main";
                    rearRoot.swappedWindowRestoreRequested(rearRoot.currentWindow);
                    rearRoot.currentWindow = null;
                }
            }
            opacity: (swapButtonArea.pressed?0.8:1.0)
            scale: (swapButtonArea.pressed?0.9:1.0)
            smooth: true
        }

        Text {
            id: dateTime
            anchors.right: parent.right
            anchors.rightMargin: width/20
            anchors.verticalCenter: parent.verticalCenter
            text: Qt.formatDateTime(new Date(), "yyyy/MM/dd hh:mm:ss")
            font.pointSize: ((statusBar.height*0.4/Conf.densityAdjust)|0)
            color: "white"
            smooth: true
        }

        Timer {
            interval: 1000; running: true; repeat: true
            onTriggered: dateTime.text = Qt.formatDateTime(new Date(), "yyyy/MM/dd hh:mm:ss")
        }

    }

    Image {
        id: background
        anchors.top: statusBar.bottom
        width: parent.width
        height: parent.height - statusBar.height
        Behavior on opacity {
            NumberAnimation { easing.type: Easing.InCubic; duration: 400; }
        }
        fillMode: Image.PreserveAspectCrop
        source: "alameda.jpg"
        smooth: true
    }

    function windowSwapped(windowFrame) {
        console.log("swapped window added: "+windowFrame);
        windowFrame.parent = background
        rearRoot.currentWindow = windowFrame
    }
    function windowCloned(surfaceItem) {
        console.log("cloned: "+surfaceItem);
        surfaceItem.parent = background
        if (rearRoot.cloneSurfaceItemList == null)
            rearRoot.cloneSurfaceItemList = new Array(0);
        rearRoot.cloneSurfaceItemList.push(surfaceItem);
    }
    function windowCloneClosed(surface) {
        console.log("cloned closed: "+surface);
        var i;
        for (i = 0; i < rearRoot.cloneSurfaceItemList.length; i++) {
            var surfaceItem = rearRoot.cloneSurfaceItemList[i];
            if(surfaceItem.surface == surface) {
                rearRoot.cloneSurfaceItemList.splice(i, 1);
                rearRoot.clonedSurfaceItemDestroyed(surfaceItem);
                break;
            }
        }
    }
    Component.onCompleted: {
    }
}
