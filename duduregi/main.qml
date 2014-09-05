import QtQuick 2.0
import "compositor.js" as CompositorLogic
import "sprintf.js" as SPrintf
import com.windriver.automotive 1.0

Item {
    id: root

    property variant currentWindow: null
    property variant waitProcess: null

    property variant selectedWindow: null
    property bool hasFullscreenWindow: typeof compositor != "undefined" && compositor.fullscreenSurface !== null

    onHasFullscreenWindowChanged: {
        console.log("has fullscreen window: " + hasFullscreenWindow);
    }

    VNADBusClient {
        id: vna_dbusClient

        onVehicleInfoChanged: {
			if(false) {
				console.log('vehicleInfoChanged : '+vehicleInfoWMI);
				console.log('vehicleInfoChanged : '+speedometer);
			}
			statusBar.setWMI(vehicleInfoWMI)
        }
    }

    WRDBusClient {
        id: wr_dbusClient

        onTrackInfoChanged: {
			console.log('title : '+title);
			console.log('playState : '+playState);
			if(playState == 0) // stop
				iPodArtwork.source = 'images/artwork.jpg';
        }
		onArtworkChanged: {
			iPodArtwork.source = 'data:image/png;base64,'+wr_dbusClient.artwork;
		}
    }

    StatusBar {
        id: statusBar
    }
    Image {
        id: background
        Behavior on opacity {
            NumberAnimation { easing.type: Easing.InCubic; duration: 400; }
        }
        anchors.top: statusBar.bottom
        width: parent.width
        height: parent.height - statusBar.height

        fillMode: Image.PreserveAspectCrop
        source: "alameda.jpg"
        smooth: true

        Item {
            id: currentApp
            anchors.fill: parent
        }

		Rectangle {
			id: ipodWidget
			clip: true
			anchors.left: parent.left
			anchors.leftMargin: background.width / 20
			anchors.top: parent.top
			anchors.topMargin: background.height / 20
			width: parent.width/2
			height: parent.height/3
			color: "transparent"
			Rectangle {
				id: iPodWidgetBG
				anchors.top: parent.top
				anchors.left: parent.left
				width: parent.width
				height: parent.height
				color: iPodMouseArea.pressed?"blue":"black"
				opacity: 0.5
				radius: 20

				MouseArea {
					id: iPodMouseArea
					anchors.top: parent.top
					anchors.left: parent.left
					width: parent.width
					height: parent.height
					onClicked: {
						if(wr_dbusClient.playState == 4)
							wr_dbusClient.pause()
						else if(wr_dbusClient.playState == 5)
							wr_dbusClient.play()
					}
				}

			}

    		Image {
				id: iPodArtwork
				source: 'images/artwork.jpg'
				anchors.top: parent.top
				anchors.left: top.left
				anchors.leftMargin: parent.height / 20
				width: parent.height
				height: parent.height
			}
			Text {
				id: iPodMedia
				anchors.top: parent.top
				anchors.topMargin: parent.height / 20
				anchors.left: iPodArtwork.right
				anchors.leftMargin: parent.height / 20
				text: "iPod"
				font.pointSize: parent.height / 10
				color: "white"
				smooth: true
				font.bold: true
				style: Text.Outline
			}
			Text {
				id: iPodArtist
				elide: Text.ElideRight
				anchors.top: iPodMedia.bottom
				anchors.left: iPodMedia.left
				anchors.right: parent.right
				anchors.rightMargin: parent.height / 20
				text: wr_dbusClient.artist
				font.pointSize: parent.height / 10
				color: "white"
				smooth: true
				font.bold: true
				style: Text.Outline
			}
			Text {
				id: iPodAlbum
				elide: Text.ElideRight
				anchors.top: iPodArtist.bottom
				anchors.left: iPodArtist.left
				anchors.right: parent.right
				anchors.rightMargin: parent.height / 20
				text: wr_dbusClient.album
				font.pointSize: parent.height / 10
				color: "white"
				smooth: true
				font.bold: true
				style: Text.Outline
			}
			Text {
				id: iPodTitle
				elide: Text.ElideRight
				anchors.top: iPodAlbum.bottom
				anchors.left: iPodArtist.left
				anchors.right: parent.right
				anchors.rightMargin: parent.height / 20
				text: wr_dbusClient.title
				font.pointSize: parent.height / 10
				color: "white"
				smooth: true
				font.bold: true
				style: Text.Outline
			}
			Text {
				id: iPodStatus
				anchors.bottom: parent.bottom
				anchors.bottomMargin: parent.height / 20
				anchors.left: iPodArtist.left
				text: (wr_dbusClient.playState == 4)?"Playing...":((wr_dbusClient.playState == 5)?"Paused":"")
				font.pointSize: parent.height / 6
				color: "white"
				smooth: true
				font.bold: true
				style: Text.Outline
			}
			Text {
				id: iPodTrackPosition
				anchors.bottom: parent.bottom
				anchors.bottomMargin: parent.height / 20
				anchors.right: parent.right
				anchors.rightMargin: parent.height / 10
				text: SPrintf.sprintf('%02d', parseInt(wr_dbusClient.trackPosition/1000/60))+':'+SPrintf.sprintf('%02d', parseInt((wr_dbusClient.trackPosition/1000)%60))
				font.pointSize: parent.height / 6
				color: "white"
				smooth: true
				font.bold: true
				style: Text.Outline
			}
			//visible: wr_dbusClient.connected > 0 && wr_dbusClient.title != ""
			visible: wr_dbusClient.connected == 1
		}

		Text {
			id: speed
			anchors.right: parent.right
			anchors.rightMargin: background.width / 20
			anchors.bottom: parent.bottom
			text: vna_dbusClient.speedometer+" Km"
			font.pointSize: background.height/4
			color: "white"
			smooth: true
			font.bold: true
			style: Text.Outline
			visible: vna_dbusClient.speedometer
		}

        MainMenu {
            id: mainmenu 
            root: root
            Component.onCompleted: {
                statusBar.closeWindow.connect(function() {
                    console.log('close clicked');
                    hide();
                })
                statusBar.logoClicked.connect(function() {
					if(!mainmenu.visible)
            			mainmenu.show()
					else
						menuShowRequested()
                })
                statusBar.closeWindow.connect(function() {
                    console.log('close clicked');
                    hide();
                })
            }
            onMenuActivated: {
                statusBar.showCloseButton(flag);
            }
        }
    }
    /*
    WebView  {
        id: launcher
        //url: "http://0.0.0.0:7070/html5apps/WRLauncher/"
        url: "file:///opt/windriver/wr-amb/www/html5apps/WRLauncher/index.html"
        anchors.fill: parent
        focus: true
        Keys.onPressed: {
            console.log('key '+event.key);
            if (event.key == Qt.Key_F1) {
                console.log('menu');
                launcher.url = "file:///opt/windriver/wr-amb/www/html5apps/WRLauncher/index.html";
            } else if (event.key == Qt.Key_Backspace) {
                console.log('back');
                launcher.goBack();
            }
        }
    }
    */

    /*
    MouseArea {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        width: 2
        height: 2
        hoverEnabled: true
        onEntered: {
            console.log('entered');
            root.selectedWindow = null
            root.focus = true
        }
        z: 10
    }
    */
    function raiseWindow(window) {
        if(root.currentWindow != null)
            root.currentWindow.visible = false
        root.currentWindow = window
        root.currentWindow.visible = true
        if(mainmenu.visible)
            mainmenu.hide();
    }

    function windowAdded(window) {
        console.log('window added '+window);
        console.log(currentApp.width+' '+ currentApp.height);
        compositor.configure(window.surface, currentApp.width, currentApp.height);
        var windowContainerComponent = Qt.createComponent("WindowFrame.qml");
        var windowContainer = windowContainerComponent.createObject(root);

        window.parent = windowContainer;
        windowContainer.child = window;
        windowContainer.width = window.width;
        windowContainer.height = window.height;
        windowContainer.parent = currentApp
        if(root.currentWindow != null)
            root.currentWindow.visible = false
        root.currentWindow = windowContainer

        if(mainmenu.visible)
            mainmenu.hide();

        if(root.waitProcess) {
            root.waitProcess.setWindow(windowContainer);
            root.waitProcess = null;
        }
    }

    function windowDestroyed(window) {
        console.log('window destroyed '+window);
        if (window && window.parent) {
            //root.currentWindow.runDestroyAnimation();
            window.parent.child = null
            window.parent.destroy();
        }
    }

    function windowResized(window) {
        console.log('window resized '+window);
    }
    /*
    function windowAdded(window) {
        console.log('window added '+window);
        var windowContainerComponent = Qt.createComponent("WindowContainer.qml");
        var windowContainer = windowContainerComponent.createObject(root);

        window.parent = windowContainer;

        windowContainer.targetWidth = window.width;
        windowContainer.targetHeight = window.height;
        windowContainer.child = window;

        var windowChromeComponent = Qt.createComponent("WindowChrome.qml");
        var windowChrome = windowChromeComponent.createObject(window);

        CompositorLogic.addWindow(windowContainer);

        windowContainer.opacity = 1
        windowContainer.animationsEnabled = true;
        windowContainer.chrome = windowChrome;
    }

    function windowResized(window) {
        console.log('window resized '+window);
        var windowContainer = window.parent;
        windowContainer.width = window.width;
        windowContainer.height = window.height;

        CompositorLogic.relayout();
    }

    function windowDestroyed(window) {
        console.log('window destroyed '+window);
        return;
        var windowContainer = window.parent;
        if (windowContainer.runDestroyAnimation)
            windowContainer.runDestroyAnimation();
    }

    function removeWindow(window) {
        console.log('window remove '+window);
        var windowContainer = window.parent;
        CompositorLogic.removeWindow(windowContainer);
        windowContainer.chrome.destroy();
        windowContainer.destroy();
        compositor.destroyWindow(window);
    }


    onHeightChanged: CompositorLogic.relayout();
    onWidthChanged: CompositorLogic.relayout();
    Component.onCompleted: {
    }
    */

    Keys.onPressed: {
        console.log('key on main: '+event.key);
        if (event.key == Qt.Key_F1) {
			console.log('[41mF1 - 3 !!!![0m\n');
            mainmenu.show()
        } else if (event.key == Qt.Key_Backspace) {
            console.log('backspace');
            if(mainmenu.visible)
                mainmenu.hide();
        }
    }
}
