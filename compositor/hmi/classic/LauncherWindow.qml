import QtQuick 2.1

Rectangle {
    id: launcherWindow
    color: Qt.rgba(0.6, 0.3, 0.3, 0.1)
    width: parent.width
    height: parent.height

    ListModel {
        id: appsx
        ListElement {
            name: ""
            icon: "icons/climate.png"
            status: "off"
            url: "http://www.gmail.com"
        }
        ListElement {
            name: ""
            icon: "icons/diagnostics.png"
            status: "off"
            url: "https://www.google.com/maps/"
        }
        ListElement {
            name: ""
            icon: "icons/settings.png"
            status: "off"
            url: "https://play.google.com/store/music"
        }
        ListElement {
            name: ""
            icon: "icons/climate.png"
            status: "off"
            url: "https://play.google.com/store/music"
        }
    }

    GridView {
        id: gridx
        model: appsx
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width*0.8
        height: parent.height*0.8
        cellWidth: ((width/4.0)|0)
        cellHeight: cellWidth
        delegate:  Rectangle {
            color: "transparent"
            property bool pressed: false
            width: ((gridx.cellWidth * 0.8) | 0)
            height: ((gridx.cellHeight * 0.8) | 0)
            Image {
                source: icon
            }
//            MouseArea {
//                anchors.fill: parent
//                onClicked: {
//                    if (status == "off") {
//                        appsx.setProperty(index, "status", "on");
//                        output.destoryCompositorElement("LauncherWindow");
//                        output.createWebViewToCompositorElement(url, name);
//                    } else {
//                        appsx.setProperty(index, "status", "off");
//                        output.destoryCompositorElement(name);
//                    }
//                }
//            }
        }
    }
}
