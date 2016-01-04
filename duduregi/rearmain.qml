import QtQuick 2.1
import "config.js" as Conf
import com.windriver.automotive 1.0
import com.windriver.genivi 1.0

Rectangle {
    color: "#ff0000"
    anchors.fill: parent
    Image {
        id: background
        anchors.fill: parent
        Behavior on opacity {
            NumberAnimation { easing.type: Easing.InCubic; duration: 400; }
        }
        fillMode: Image.PreserveAspectCrop
        source: "alameda.jpg"
        smooth: true
    }
    Text {
        anchors.fill: parent
        text: "Rear Display"
        font.pointSize: parent.height / 20
        color: "white"
        smooth: true
        font.bold: true
        style: Text.Outline
    }
}
