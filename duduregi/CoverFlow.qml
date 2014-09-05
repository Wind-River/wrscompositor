import QtQuick 2.0
//import QtWebEngine 0.9
import QtWebKit 3.0

Item {
    id: coverFlow
    property variant root: null

    property int itemWidth: 100
    property int itemHeight: 100

    property ListModel listModel

    signal indexChanged(int index)
    signal menuShowRequested;
    signal menuShow(int index);

    function show() {
        myPathView.focus = true;
    }

    /*
    Keys.onPressed: {
        console.log('key on cf: '+event.key+' '+Qt.Key_Return);
        if (event.key == Qt.Key_Backspace) {
            parent.hide();
        } else
            return;
        event.accepted = true
    }
    */

    Component {
	id: appDelegate

	Flipable {
	    id: myFlipable

	    property bool flipped: false

	    width: itemWidth; height: itemHeight
	    z: PathView.z
	    scale: PathView.iconScale
            opacity: PathView.opacity

	    function itemClicked()
	    {
		console.log('icon clicked path='+path);
		console.log('icon clicked type='+type);
		if(PathView.isCurrentItem) {
                   if(type == "quick-html5app" && !myFlipable.flipped) {
                       myFlipable.width = myPathView.width
                       myFlipable.height = myPathView.height
                   }
		   myFlipable.flipped = !myFlipable.flipped
		   myPathView.interactive = !myFlipable.flipped
                   /*
                   if(type == "quick-html5app" && myFlipable.flipped) {
                        var view = Qt.createQmlObject('import QtQuick 2.0; import QtWebKit 3.0; WebView { anchors.fill: parent;}', back);
                        view.url = path;
                   }
                   */
                   if(type == "quick-html5app" && myFlipable.flipped) {
                        webview.focus = true
			console.log('icon clicked webview.url='+webview.url);
                        if(webview.url == "about:blank") {
			    console.log('icon clicked webview.url='+webview.url);
                            webview.url = path
                        }
                            /*
                        else {
                            var view = front.children[front.children.length-1]
                            view.parent = back;
                        }
                            */
                   } 
                   else if(type == "launcher" && myFlipable.flipped) {
                        subLauncher.focus = true
                   }
		}
		else if(myPathView.interactive) {
		    myPathView.currentIndex = index
		}
	    }
            onSideChanged: {
                if(side==Flipable.Front) {
                    myFlipable.width = itemWidth
                    myFlipable.height = itemHeight
                } else {
                }
            }
			Component.onCompleted: {
				coverFlow.menuShow.connect(function(i) {
					if(index == i) {
						webview.focus = false;
						itemClicked();
						myFlipable.focus = true;
					}
				})
			}

            Keys.onPressed: {
                console.log('key on flipable: '+event.key);
                if (event.key == Qt.Key_Return) {
                    itemClicked();
                } else if (event.key == Qt.Key_F2) {
                    webview.url = "about:blank";
                } else if (event.key == Qt.Key_F1) {
                    webview.focus = false;
                    itemClicked();
                    myFlipable.focus = true;
                    /*
                } else if (event.key == Qt.Key_Backspace) {
                    itemClicked();
                    coverFlow.parent.hide();
                    */
                } else
                    return;
                event.accepted = true;
            }

	    transform: Rotation {
		 id: rotation
		 origin.x: myFlipable.width/2
		 origin.y: myFlipable.height/2
		 axis.x: 0; axis.y: 1; axis.z: 0
		 angle: PathView.angle
	     }

	    states: [
                State {
                     name: "back"
                     PropertyChanges { target: rotation; angle: 180 }
                     PropertyChanges {target: myFlipable; width: myPathView.width; height: myPathView.height }
                     when: myFlipable.flipped
                 },
                 State{
                     name: "front"
                     PropertyChanges { target: rotation; angle: 0 }
                     PropertyChanges {target: myFlipable; width: itemWidth; height: itemHeight }
                     /*
                     PropertyChanges {target: myFlipable; width: myPathView.width; height: myPathView.height }
                     */
                     when: !myFlipable.flipped
                }
            ]

	    transitions: Transition {
		 ParallelAnimation {
		    NumberAnimation { target: rotation; property: "angle"; duration: 500 }
		    NumberAnimation {target: myFlipable; properties: "height,width"; duration: 500}
		 }
	    }

	    front: Item {
                id: frontItem
		smooth: true
		width: itemWidth; height: itemHeight
		anchors.horizontalCenter: parent.horizontalCenter
		anchors.verticalCenter: parent.verticalCenter
		//color: "black"
		//border.color: "white"
		//border.width: 3

		Image {
		    id: myIcon
		    anchors.centerIn: parent
		    source: icon
		    smooth: true
            scale: itemHeight/height
            RotationAnimation on rotation {
                loops: Animation.Infinite
                from: 360
                to: 0
                duration: 10000
                running: iconRotate == true && myPathView.currentIndex == index
            }
            MouseArea {
                id: mouseArea
                enabled: side==Flipable.Front
                anchors.fill: parent
                onClicked: {
                    console.log('icon clicked '+name);
                    itemClicked()
                }
            }
		}
                Image {
                    id: quitButton
                    visible: webview.url != "about:blank" && myPathView.currentIndex == index
                    enabled: visible
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenterOffset: parent.width*0.4
                    anchors.verticalCenterOffset: -parent.width*0.4
                    source: "images/close.png"
                    smooth: true
                    width: height
                    height: quitArea.pressed?((parent.height/4.5)|0):((parent.height/4)|0)
                    MouseArea {
                        id: quitArea
                        anchors.fill: parent
                        onClicked: {
                            console.log('quit '+name);
                            webview.url = "about:blank";
                        }
                    }
                }
                Text {
                    id: myText
                    anchors.top: parent.bottom
                    anchors.topMargin: ((parent.height / 20)|0)
                    text: name
                    font.pointSize: ((parent.height / 8)|0)
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "black"
                    style: Text.Outline
                    styleColor: "white"
                    smooth: true
                    anchors.bottomMargin: ((parent.height / 14)|0)
                }
	    }
	    back: Rectangle {
                id: backItem
		anchors.fill: parent
                color: "black"

                Image {
		    anchors.fill: parent
                    visible: background != null
                    source: background
                    fillMode: Image.PreserveAspectCrop
                }
                Image {
                    anchors.centerIn: parent
                    source: icon
                    smooth: true
                    width: parent.height*itemWidth/itemHeight
                    height: parent.height
                    visible: background == null
                }
                //WebEngineView  {
                WebView  {
                    id: webview
                    url: "about:blank"
                    visible: type == "quick-html5app"
                    anchors.fill: parent
                    Keys.onPressed: {
                        console.log('key on webview '+event.key);
                        if (event.key == Qt.Key_Backspace || event.key == Qt.Key_F1) {
                            webview.focus = false;
                            itemClicked();
                            myFlipable.focus = true;
                        } 
                    }
                    onUrlChanged: {
                        if(url == "about:blank" && webview.focus) {
                            webview.focus = false;
                            itemClicked()
                            myFlipable.focus = true;
                        };
                    }
                }
                Launcher {
                    id: subLauncher
                    root: coverFlow.root
                    visible: type == "launcher"
                    modelType: launcherType
                    //focus: visible
                }
	    }
	}
    }

    PathView {
	id: myPathView

	Keys.onRightPressed: { goRight(); event.accepted = true; }
	Keys.onLeftPressed: { goLeft(); event.accepted = true; }
        function goRight() {
            if (!moving && interactive) incrementCurrentIndex()
        }
        function goLeft() {
            if (!moving && interactive) decrementCurrentIndex()
        }

	anchors.fill: parent
	preferredHighlightBegin: 0.5
	preferredHighlightEnd: 0.5
	focus: true
	interactive: true
	model: listModel
	delegate: appDelegate
	path: Path {
	    startX: 0
	    startY: coverFlow.height / 2
	    PathAttribute { name: "z"; value: 0 }
	    PathAttribute { name: "angle"; value: 60 }
	    PathAttribute { name: "iconScale"; value: 0.4 }
	    PathAttribute { name: "opacity"; value: 0.1 }
	    PathLine { x: coverFlow.width / 2; y: coverFlow.height / 2;  }
	    PathAttribute { name: "z"; value: 100 }
	    PathAttribute { name: "angle"; value: 0 }
	    PathAttribute { name: "iconScale"; value: 1.0 }
	    PathAttribute { name: "opacity"; value: 1.0 }
	    PathLine { x: coverFlow.width; y: coverFlow.height / 2; }
	    PathAttribute { name: "z"; value: 0 }
	    PathAttribute { name: "angle"; value: -60 }
	    PathAttribute { name: "iconScale"; value: 0.4 }
	    PathAttribute { name: "opacity"; value: 0.1 }
	}
    }

    Component.onCompleted: {
        indexChanged(myPathView.currentIndex);
	myPathView.currentIndexChanged.connect(function(){
	    indexChanged(myPathView.currentIndex);
	})
		coverFlow.menuShowRequested.connect(function() {
			menuShow(myPathView.currentIndex);
		})
    }
}
