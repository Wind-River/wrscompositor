/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
import QtQuick 2.1
import com.windriver.duduregi 1.0
import "config.js" as Conf

Item {
    id: mainMenu
    signal menuActivated(bool flag)
    property variant root: null
    property bool androidAutoProjectionMode: null;//coverFlow.androidAutoProjectionMode
    property bool androidAutoEnabled: falsed
    property variant androidAutoContainer: null;//coverFlow.androidAutoContainer
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
        menuActivated(true);
        focus = true
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
        id: nativeApps
        ListElement {
            label: "AM Monitor"
            description: "Audio Manager Monitor"
            //exec: "/home/jpark/demo/am-monitor-wayland.sh"
            exec: "AudioManagerMonitor"
            type: "capp"
            multiple: false
            taskmanage: true
            systemd: false
            iconPath: "icons/gdp-icon-ammonitor.png"
        }
        ListElement {
            label: "Application"
            description: "Application"
            exec: "/opt/windriver/bin/assistant"
            type: "capp"
            multiple: false
            taskmanage: true
            systemd: false
            iconPath: "icons/gdp-icon-app.png"
        }
        ListElement {
            label: "Fuel Stop Advisor"
            description: "Fuel Stop Advisor"
            //exec: "/opt/windriver/bin/assistant"
            exec: "qdbusviewer-qt5"
            type: "capp"
            multiple: false
            taskmanage: true
            systemd: false
            iconPath: "icons/gdp-icon-fuel.png"
        }
        ListElement {
            label: "Web Browsing"
            description: "Web Browsing"
            exec: "/opt/windriver/bin/assistant"
            type: "capp"
            multiple: false
            taskmanage: true
            systemd: false
            iconPath: "icons/gdp-icon-browser.png"
        }
        ListElement {
            label: "Navigation"
            description: "Navigation"
            exec: "mocknavi"
            type: "capp"
            multiple: false
            taskmanage: true
            systemd: false
            iconPath: "icons/gdp-icon-nav.png"
            unitFile: "mocknavi.service"
        }
        ListElement {
            label: "Media Player"
            description: "Media Player"
            //exec: "eyes"
            exec: "mediaplayer"
            type: "capp"
            multiple: false
            taskmanage: true
            systemd: false
            iconPath: "icons/gdp-icon-mediaplayer.png"
            unitFile: "eyes.service"
        }
    }

    GridView {
        id: grid
        model: nativeApps
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width*0.8
        height: parent.height*0.8
        cellWidth: ((width/4.0)|0)
        cellHeight: cellWidth
        delegate:  Item {
            property variant window: null
            property bool pressed: false
            width: ((grid.cellWidth * 0.8) | 0)
            height: ((grid.cellHeight * 0.8) | 0)
            //selected: GridView.isCurrentItem
            //onClicked: main.launch(name)
            function launch() {
                var pid = systemd ? systemd_unit.pid : process.pid;
                window = systemd ? systemd_unit.window : process.window;
                console.log('launch: '+ systemd ? unitFile : exec);
                console.log('pid: ' + pid);
                if (!multiple) {
                    console.log('no multiple');
                    if(pid != 0) {
                        console.log('no pid');
                        if(window != null) {
                            console.log('has window ' + window);
                            launcher.root.raiseWindow(window);
                        }
                        return;
                    }
                }
                if (systemd) systemd_dbusClient.startUnit(unitFile);
                else process.execute(exec);
            }

            function quit() {
                if (systemd) systemd_dbusClient.stopUnit(unitFile);
                else process.quit();
            }

            SystemdUnit {
                id: systemd_unit
                unitPath: systemd?unitFile:""
                property variant window: null
                property variant cmd: unitFile

                onPidChanged: {
                    console.log("program launched by systemd's dbus, pid = " + pid);
                    console.log(launcher.root);
                    launcher.root.waitProcess = systemd_unit;
                }
                Component.onCompleted: {
                    if (systemd)
                        systemd_dbusClient.registerUnit(systemd_unit);
                }
                function setWindow(window) {
                    console.log('setWindow '+ window);
                    systemd_unit.window = window;
                }
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
                font.pointSize: ((parent.height / 8.0)/Conf.densityAdjust|0)
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
