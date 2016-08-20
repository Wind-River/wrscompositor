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
    property int windowDefaultWidth: 1920
    property int windowDefaultHeight: 1080

    SystemdDbusClient {
        id: systemd_dbusClient
    }

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
    function launchNative(appid) {
        var unitFile = appid + ".service"
        var pid = systemd_dbusClient.getPidByUnitFile(unitFile);
        if (pid < 0)
            return;

        if (pid == 0) {
            console.log("launchNative, native app is launched by systemd. native app = " + appid);
            systemd_dbusClient.startUnit(unitFile);
        } else {
            console.log("launchNative, native app has already launched by systemd. native app = " + appid);
            root.raiseWindowByProcessId(pid);
        }

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
            exec: "AudioManagerMonitor"
            multiple: false
            systemd: true
            iconPath: "icons/gdp-icon-ammonitor.png"
            unitFile: "AudioManager_Monitor.service"
        }
        ListElement {
            label: "Application"
            description: "Application"
            exec: "skobblernavi"
            multiple: false
            systemd: true
            iconPath: "icons/gdp-icon-app.png"
            unitFile: "skobblernavi.service"
        }
        ListElement {
            label: "Fuel Stop Advisor"
            description: "Fuel Stop Advisor"
            //exec: "/opt/windriver/bin/assistant"
            exec: "qdbusviewer-qt5"
            multiple: false
            systemd: false
            iconPath: "icons/gdp-icon-fuel.png"
        }
        ListElement {
            label: "Web Browsing"
            description: "Web Browsing"
            exec: "/opt/demoui/bin/demoui"
            multiple: false
            systemd: true
            iconPath: "icons/gdp-icon-browser.png"
            unitFile: "demoui.service"
        }
        ListElement {
            label: "Navigation"
            description: "Navigation"
            exec: "mocknavi"
            multiple: false
            systemd: true
            iconPath: "icons/gdp-icon-nav.png"
            unitFile: "mocknavi.service"
        }
        ListElement {
            label: "Media Player"
            description: "Media Player"
            exec: "mediaplayer"
            multiple: false
            systemd: true
            iconPath: "icons/gdp-icon-mediaplayer.png"
            unitFile: "mediaplayer.service"
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
                            root.raiseWindow(window);
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
                    console.log("onPidChanged, pid = " + pid);
                    root.waitProcess = systemd_unit;
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
                windowDefaultWidth: mainMenu.windowDefaultWidth
                windowDefaultHeight: mainMenu.windowDefaultHeight
                onPidChanged: {
                    console.log('program launched');
                    root.waitProcess = process
                }
                function setWindow(window) {
                    console.log('setWindow '+window);
                    process.window = window
                }
            }
            Component.onCompleted: {
                // XXX auto launch at starting time
                if (unitFile == "skobblernavi.service") {
                    launch();
                }
            }
            Image {
                id: appIcon
                source: iconPath
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
                    visible: (systemd) ? systemd_unit.pid != 0 : process.pid != 0
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
                            console.log('quit ' + systemd ? systemd_unit.pid : process.pid);
                            if (systemd) systemd_dbusClient.stopUnit(unitFile);
                            else process.quit()
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
