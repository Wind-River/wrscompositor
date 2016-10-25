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

Item {
    id: launcher
    property variant root: null
    //property string modelType: "tizen"
    //property string modelType: "waland"
    property string modelType: "genivi"
    anchors.fill: parent


    //
    ListModel {
        id: geniviApps
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
            exec: "/usr/bin/qt5/designer"
            type: "capp"
            multiple: false
            taskmanage: true
            systemd: false
            iconPath: "icons/gdp-icon-app.png"
        }
        ListElement {
            label: "Fuel Stop Advisor"
            description: "Fuel Stop Advisor"
            exec: "/usr/bin/qt5/qdbusviewer"
            type: "capp"
            multiple: false
            taskmanage: true
            systemd: false
            iconPath: "icons/gdp-icon-fuel.png"
        }
        ListElement {
            label: "Web Browsing"
            description: "Web Browsing"
            exec: "/usr/share/qt5/examples/webkitwidgets/browser/browser"
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
        model: modelType=="genivi"?geniviApps:waylandApps
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
