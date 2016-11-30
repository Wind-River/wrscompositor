/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

import QtQuick 2.1
import com.windriver.wrscompositor 1.0
import "config.js" as Conf
import "compositor.js" as Logic

Item {
    id: mainMenu
    width: parent.width
    height: parent.height
    anchors.fill: parent

    SystemdDbusClient {
        id: systemd_dbusClient
    }

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.8
    }

    Behavior on opacity {
        NumberAnimation { easing.type: Easing.InCubic; duration: 400; }
    }

    function show() {
        mainMenu.visible = true
        mainMenu.focus = true
    }

    function hide() {
        mainMenu.visible = false
        mainMenu.focus = true
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

    ListModel {
        id: nativeApps
        ListElement {
            label: "Web Browsing"
            description: "Web Browsing"
            exec: "/usr/share/qt5/examples/webkitwidgets/browser/browser"
            multiple: false
            systemd: false
            iconPath: "icons/native-web.png"
        }
        ListElement {
            label: "Navigation"
            description: "Navigation"
            exec: "/usr/bin/skobblernavi"
            multiple: false
            systemd: false
            iconPath: "icons/native-map-location.png"
            unitFile: "skobblernavi.service"
        }
        ListElement {
            label: "Video"
            description: "Video"
            exec: "/usr/bin/mediaplayer"
            multiple: false
            systemd: false
            iconPath: "icons/native-video.png"
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
            function launch() {
                var pid = systemd ? systemd_unit.pid : process.pid;
                window = systemd ? systemd_unit.window : process.window;
                console.log('launch: '+ systemd ? unitFile : exec);
                console.log('pid: ' + pid);
                if (!multiple) {
                    console.log('no multiple');
                    if (pid != 0) {
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
                onPidChanged: {
                    console.log('program launched');
                    root.waitProcess = process
                }
                function setWindow(window) {
                    console.log('setWindow '+window);
                    process.window = window
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
                    source: "icons/close.png"
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

    Component.onCompleted: {
        Logic.registerObjectItem("MainMenu", mainMenu);
    }

    Keys.onLeftPressed: { grid.moveCurrentIndexLeft(); event.accepted = true}
    Keys.onRightPressed: { grid.moveCurrentIndexRight(); event.accepted = true}
    Keys.onUpPressed: { grid.moveCurrentIndexUp(); event.accepted = true}
    Keys.onDownPressed: { grid.moveCurrentIndexDown(); event.accepted = true}
    Keys.onReturnPressed: { grid.currentItem.pressed = true; grid.currentItem.launch(); event.accepted = true}
    Keys.onReleased: { grid.currentItem.pressed = false; if(event.key == Qt.Key_F2) { grid.currentItem.quit(); event.accepted = true }}
}