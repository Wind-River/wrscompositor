import QtQuick 2.1

Rectangle {
    id: launcherBar
    color: Qt.rgba(0.3, 0.3, 0.3, 0.1)
    width: parent.width
    height: parent.height


    ListModel {
        id: apps
        ListElement {
            icon: "icons/phone.png"
            name: "Email"
            status: "off"
            url: "http://www.gmail.com"
            type: "web"
        }
        ListElement {
            icon: "icons/navigation.png"
            name: "Maps"
            status: "off"
            url: "https://www.google.ro/maps/@45.4386315,28.0529669,15z"
            type: "web"
        }
        ListElement {
            icon: "icons/media.png"
            name: "Music"
            status: "off"
            url: "http://www.iheart.com/"
            type: "web"
        }
        ListElement {
            icon: "icons/applications.png"
            name: "LauncherWindow"
            status: "off"
            url: "LauncherWindow.qml"
            type: "qml"
        }
    }

    GridView {
        id: grid
        model: apps
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width*0.8
        height: parent.height*0.8
        cellWidth: ((width/4.0)|0)
        cellHeight: cellWidth
        delegate:  Rectangle {
            color: "transparent"
            property bool pressed: false
            width: parent.parent.height
            height: parent.parent.height
            Rectangle {
                id: iconxx
                anchors.fill: parent
                smooth: true;
                color: "transparent"
            }
            Image {
                id: iconx
                anchors.fill: parent
                source: icon
                fillMode: Image.PreserveAspectFit
                smooth: true;
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (status == "off") {
                        apps.setProperty(index, "status", "on");
                        iconxx.color = Qt.rgba(0.5, 0.5, 0.5, 0.5);
                        if (type == "web") {
                            output.createWebViewToCompositorElement(url, name);
                        } else if (type == "qml") {
                            output.createQmlComponentToCompositorElement(url, name);
                        }
                    } else {
                        apps.setProperty(index, "status", "off");
                        iconxx.color = Qt.rgba(0, 0, 0, 0);
                        output.destoryCompositorElement(name);
                    }
                }
            }
        }
    }
}
