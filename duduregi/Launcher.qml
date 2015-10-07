import QtQuick 2.1
import com.windriver.duduregi 1.0

Item {
    id: launcher
    property variant root: null
    property string modelType: "tizen"
    anchors.fill: parent
    // 
    ListModel {
        id: tizenApps
        ListElement {
            appid: "org.tizen.dialer"
            label: "Dialer"
            description: "Handsfree Dialer Application"
            exec: "/usr/bin/dialer"
            type: "capp"
            multiple: false
            taskmanage: true
	    iconPath: "/usr/share/icons/default/small/org.tizen.dialer.png"
        }
        ListElement {
            appid: "MiniBrowser"
            label: "MiniBrowser"
            description: "Sample Webkit Browser"
            exec: "/usr/bin/browser"
            type: "capp"
            multiple: false
            taskmanage: true
            iconPath: "/usr/share/icons/default/small/browser.png"
        }
        ListElement {
            appid: "terminal"
            label: "terminal"
            description: "weston terminal"
            exec: "/usr/bin/weston-terminal"
            type: "capp"
            multiple: false
            taskmanage: true
            iconPath: "/usr/share/icons/default/small/terminal.png"
        }
        ListElement {
            appid: "ODBQpKvkS1.Settings"
            label: "Settings"
            exec: "ODBQpKvkS1"
            type: "wgt"
            multiple: false
            taskmanage: true
        }
        ListElement {
            appid: "GV3ySIINq7.GhostCluster"
            label: "GhostCluster"
            exec: "GV3ySIINq7"
            type: "wgt"
            multiple: false
            taskmanage: true
        }
        ListElement {
            appid: "t8j6HTRpuz.MediaPlayer"
            label: "MediaPlayer"
            exec: "t8j6HTRpuz"
            type: "wgt"
            multiple: false
            taskmanage: true
        }
        ListElement {
            appid: "lYjFlj49Q4.saythis"
            label: "saythis"
            exec: "lYjFlj49Q4"
            type: "wgt"
            multiple: false
            taskmanage: true
        }
    }
    /*
    weston-calibrator    weston-fullscreen    weston-simple-shm
    weston-clickdot      weston-image         weston-simple-touch
    weston-cliptest      weston-info          weston-smoke
    weston-dnd           weston-launch        weston-terminal
    weston-eventdemo     weston-resizor       weston-transformed
    weston-flower        weston-simple-egl   
    */
    ListModel {
        id: waylandApps
        ListElement {
            label: "terminal"
            exec: "/usr/bin/weston-terminal"
            type: "capp"
            multiple: false
            taskmanage: true
	    iconPath: "icons/wayland.png"
        }
        ListElement {
            label: "flower"
            exec: "/usr/bin/weston-flower"
            type: "capp"
            multiple: false
            taskmanage: true
	    iconPath: "icons/wayland.png"
        }
        ListElement {
            label: "smoke"
            exec: "/usr/bin/weston-smoke"
            type: "capp"
            multiple: false
            taskmanage: true
	    iconPath: "icons/wayland.png"
        }
        /*
        ListElement {
            label: "simple-egl"
            exec: "/usr/bin/weston-simple-egl"
            type: "capp"
            multiple: false
            taskmanage: true
	    iconPath: "icons/wayland.png"
        }
        */
        ListElement {
            label: "simple-shm"
            exec: "/usr/bin/weston-simple-shm"
            type: "capp"
            multiple: false
            taskmanage: true
	    iconPath: "icons/wayland.png"
        }
        ListElement {
            label: "clickdot"
            exec: "/usr/bin/weston-calibrator"
            type: "capp"
            multiple: false
            taskmanage: true
	    iconPath: "icons/wayland.png"
        }
    }
    GridView {
        id: grid
        model: modelType=="tizen"?tizenApps:waylandApps
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width*0.8
        height: parent.height*0.8
        cellWidth: ((width/4.0)|0)
        cellHeight: cellWidth
        delegate:  Item {
            property bool pressed: false
            width: ((grid.cellWidth * 0.8) | 0)
            height: ((grid.cellHeight * 0.8) | 0)
            //selected: GridView.isCurrentItem
            //onClicked: main.launch(name)
            function launch() {
                console.log('launch: '+exec);
                console.log('pid: '+process.pid);
                if(!multiple) {
                    console.log('no multiple');
                    if(process.pid != 0) {
                        console.log('no pid');
                        if(process.window != null) {
                            console.log('haswindow'+process.window);
                            launcher.root.raiseWindow(process.window);
                        }
                        return;
                    }
                }
                process.execute(exec);
            }
            function quit() {
                process.quit();
            }
            Process {
                id: process
                property variant window: null
                onPidChanged: {
                    console.log('program launched');
                    console.log(launcher.root);
                    launcher.root.waitProcess = process
                }
                function setWindow(window) {
                    console.log('setWindow '+window);
                    process.window = window
                }
            }
            Image {
                id: appIcon
                source: type=="wgt"?"/opt/usr/apps/"+exec+"/res/wgt/"+label+".png":iconPath
		anchors.centerIn: parent
                scale: (pressed||iconMouseArea.pressed)?(parent.height/height):(parent.height/height)*0.9
                MouseArea {
                    id: iconMouseArea
                    anchors.fill: parent
                    onClicked: {
                        grid.currentIndex = index;
                        launch();
                    }
                }
                Image {
                    id: quitButton
                    visible: process.pid != 0
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
                            console.log('quit '+process.pid);
                            process.quit()
                        }
                    }
                }
            }
            Text {
                id: myText
                anchors.top: parent.bottom
                text: label
                font.pointSize: ((parent.height / 4)|0)
                anchors.horizontalCenter: parent.horizontalCenter
                color: "black"
                style: Text.Outline
                styleColor: "white"
                smooth: true
            }
        }
        highlight: Rectangle {
            color: "lightsteelblue";
            radius: 5
        }

    }
    Keys.onLeftPressed: { grid.moveCurrentIndexLeft(); event.accepted = true}
    Keys.onRightPressed: { grid.moveCurrentIndexRight(); event.accepted = true}
    Keys.onUpPressed: { grid.moveCurrentIndexUp(); event.accepted = true}
    Keys.onDownPressed: { grid.moveCurrentIndexDown(); event.accepted = true}
    Keys.onReturnPressed: { grid.currentItem.pressed = true; grid.currentItem.launch(); event.accepted = true}
    Keys.onReleased: { grid.currentItem.pressed = false; if(event.key == Qt.Key_F2) { grid.currentItem.quit(); event.accepted = true }}
}
