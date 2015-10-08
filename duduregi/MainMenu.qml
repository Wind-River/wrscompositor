import QtQuick 2.1

Item {
    id: mainMenu
    signal menuShowRequested;
    signal menuActivated(bool flag)
    property variant root: null
    Rectangle {
        id: background
        anchors.fill: parent
        color: "black"
        opacity: 0.8
    }
    Behavior on opacity {
        NumberAnimation { easing.type: Easing.InCubic; duration: 400; }
    }
    anchors.fill: parent
    visible: true
    function show() {
        visible = true
        coverFlow.show();
        menuActivated(true);
    }
    function hide() {
        visible = false
        parent.focus = true
        menuActivated(false);
    }
    /*
    Keys.onPressed: {
        console.log('key on menu: '+event.key);
        if (event.key == Qt.Key_Backspace) {
            hide();
        }
    }
    */

    ListModel {
		id: appModel
		ListElement {
			name: "GENIVI Demo Platform";
			icon: "icons/genivi.png";
			type: "launcher"
			launcherType: "genivi"
			iconRotate: false
			background: "images/xdg-background.png"
		}
		ListElement {
			name: "Navigation";
			icon: "icons/navi.png"
			type: "quick-html5app"
			iconRotate: false
			//path: 'file:///opt/windriver/wr-amb/www/html5apps/WRNavigation/index.html';
			//path: 'file:///home/sryu/projects/lg_demo/hello/www/index.html';
			path: 'file:///opt/windriver/share/html5apps/WRNavigation/index.html';
		}
		ListElement {
			name: "HVAC";
			icon: "icons/hvac.png"
			type: "quick-html5app"
			iconRotate: false
			path: 'file:///opt/windriver/share/html5apps/WRHVAC/index.html';
		}
		ListElement {
			name: "Water-Depth View";
			icon: "icons/waterdepth-icon-small.png"
			type: "quick-html5app"
			path: 'file:///opt/windriver/share/html5apps/WRWaterDepth/index.html';
		}
		/*
		ListElement {
			name: "Settings";
			icon: "icons/settings.png"
			type: "quick-html5app"
			//path: 'file:///opt/usr/apps/GV3ySIINq7/res/wgt/index.html';
			path: 'file:///opt/usr/apps/ODBQpKvkS1/res/wgt/index.html';
		}
		ListElement {
			name: "Smart Device Link HMI Sample";
			icon: "icons/genivi.png";
			type: "quick-html5app"
			path: 'file:///usr/share/smartdevicelink/HMI/index.html';
		}
		ListElement {
			name: "Tizen IVI 3.0 Apps";
			icon: "icons/tizen-on-dark.png";
			type: "launcher"
			launcherType: "tizen"
			iconRotate: true
			background: "images/tizen-background.png"
		}
		ListElement {
			name: "Wayland Samples";
			icon: "icons/wayland.png";
			type: "launcher"
			launcherType: "wayland"
			background: "images/xdg-background.png"
		}
		*/
	/*ListElement { name: "CSS3 Demo"; icon: "icons/angrybird.png" }*/

		ListElement {
			name: "iPod";
			icon: "icons/ipod-shuffle-4g.png"
			type: "quick-html5app"
			iconRotate: false
			path: 'file:///opt/windriver/share/html5apps/WRIPodPlayer/index.html';
		}
    }


    CoverFlow {
        id: coverFlow
        root: mainMenu.root
		listModel: appModel

		width: parent.width
		anchors.fill: parent


		itemWidth: ((height / 1.8)|0)
		itemHeight: itemWidth

		Component.onCompleted: {
			mainMenu.menuShowRequested.connect(function() {
    			menuShowRequested()
			})
		}
    }

    /*
    Image {
	source: "images/close.png"
	anchors.right: parent.right
	anchors.top: parent.top
	anchors.margins: buttonArea.pressed?((parent.height/24)|0):((parent.height/20)|0)
        width: buttonArea.pressed?((parent.height/7)|0):((parent.height/8)|0)
        height: buttonArea.pressed?((parent.height/7)|0):((parent.height/8)|0)
        smooth: true

	MouseArea {
            id: buttonArea
	    anchors.fill: parent
	    onClicked: hide()
	}
    }
    */
}
