import QtQuick 1.0

Item {
    id: chrome
    anchors.fill: parent

    property variant window: parent;
    property bool selected: root.selectedWindow === window

    MouseArea {
        anchors.fill: parent
        enabled: !window.focus
        hoverEnabled: !window.focus
        onClicked: {
            if (selected) {
                window.takeFocus();
            } else {
                root.selectedWindow = window
                root.focus = true
            }
        }
    }
}
