import QtQuick 2.0
import com.windriver.home 1.0

Rectangle {
	color: "black"
	Rectangle {
		id: statusBar
		color: "black"
		width: parent.width
		height: parent.height/10
		anchors.left: parent.left
		anchors.top: parent.top
		Text {
			color: "white"
			smooth: true
			font.bold: true
			font.pointSize: parent.height / 2
			style: Text.Outline
			anchors.left: parent.left
			anchors.top: parent.top
			text: "<Status Bar>"
		}
		Rectangle {
			color: "red"
			width: parent.width
			height: 1
			anchors.left: parent.left
			anchors.bottom: statusBar.bottom
		}
	}
	Button {
		id: feature0
		text: "Feature 0"
		anchors.left: parent.left
		anchors.leftMargin: parent.width/3/3
		anchors.top: statusBar.bottom
		anchors.topMargin: parent.height/6/4
		onClicked: program.visible = true;
	}

	Button {
		id: feature1
		anchors.left: feature0.right
		anchors.leftMargin: parent.width/3/3
		anchors.top: statusBar.bottom
		anchors.topMargin: parent.height/6/4
		text: "Feature 1"
		onClicked: program.visible = true;
	}

	Button {
		id: feature2
		anchors.left: parent.left
		anchors.leftMargin: parent.width/3/3
		anchors.top: feature0.bottom
		anchors.topMargin: parent.height/6/4
		text: "Feature 2"
		onClicked: program.visible = true;
	}

	Button {
		id: feature3
		anchors.left: feature0.right
		anchors.leftMargin: parent.width/3/3
		anchors.top: feature1.bottom
		anchors.topMargin: parent.height/6/4
		text: "Feature 3"
		onClicked: program.visible = true;
	}

	Button {
		id: feature4
		anchors.left: parent.left
		anchors.leftMargin: parent.width/3/3
		anchors.top: feature2.bottom
		anchors.topMargin: parent.height/6/4
		text: "Feature 4"
		onClicked: program.visible = true;
	}

	MenuHandler {
		id: menuHandler
	}
	Button {
		id: feature5
		anchors.left: feature0.right
		anchors.leftMargin: parent.width/3/3
		anchors.top: feature3.bottom
		anchors.topMargin: parent.height/6/4
		text: "iPod"
		onClicked: {
			menuHandler.launch('ipod');
		}
	}

	Rectangle {
		id: program
		width: parent.width
		height: parent.height-statusBar.height-1
		anchors.left: parent.left
		anchors.top: statusBar.bottom
		visible: false
		Button {
			id: backButton
			width: parent.width/8
			height: parent.width/10
			anchors.right: parent.right
			anchors.rightMargin: parent.width/20
			anchors.top: parent.top
			anchors.topMargin: parent.height/20
			text: "Back"
			onClicked: parent.visible = false
		}
		Text {
			color: "white"
			smooth: true
			font.bold: true
			font.pointSize: parent.height / 5
			style: Text.Outline
			anchors.centerIn: parent
			text: "Program"
		}
	}
}
