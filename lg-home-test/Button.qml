import QtQuick 2.0
Rectangle {
	property string text: "Button"
	signal clicked
	border.color: "red"
	border.width: 1
	color: buttonArea.pressed?"orange":"black"
	width: parent.width/3
	height: parent.height/4
	Text {
		color: "white"
		smooth: true
		font.bold: true
		font.pointSize: parent.height / 3
		style: Text.Outline
		anchors.centerIn: parent
		text: parent.text
	}
	MouseArea {
		id: buttonArea
		anchors.top: parent.top
		anchors.left: parent.left
		width: parent.width
		height: parent.height
		onClicked: parent.clicked()
	}
}
