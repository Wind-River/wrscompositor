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
import QtMultimedia 5.0
import "compositor.js" as CompositorLogic
import "config.js" as Conf
import "sprintf.js" as SPrintf
import com.windriver.automotive 1.0
import com.windriver.genivi 1.0

Item {
    id: helixCockpitView

    property variant root: null
    property variant background: background
    property variant mainmenu: mainmenu
    property variant statusBar: statusBar
    property variant dockBar: dockBar
    property variant sidePanel: sidePanel
    property variant inputPanel: inputPanel

    property int fullScreenWidth: parent.width
    property int fullScreenHeight: parent.height - statusBar.height
    property int defaultScreenWidth: parent.width - sidePanel.width
    property int defaultScreenHeight: parent.height - statusBar.height - dockBar.height

    width: parent.width
    height: parent.height

    StatusBar {
        id: statusBar
        onHeightChanged: {
            Conf.statusBarHeight = statusBar.height
        }
        currentWindowExposed: root.currentWindow && root.currentWindow.visible && !mainmenu.visible
        cloneAvailable: root.currentWindow && root.currentWindow.cloned == false
    }
    SidePanel {
        id: sidePanel
        anchors.top: statusBar.bottom
        anchors.right: parent.right
        anchors.bottom: dockBar.top
        width: parent.width * 0.34
    }

    InputPanel {
        id: inputPanel
        z: 50000
        y: parent.height
        width: parent.width
        height: parent.height/3
        property variant statusBar: statusBar
        property variant dockBar: dockBar
        property variant virutalKeyboard: virutalKeyboard
        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges {
                target: inputPanel
                y: parent.height - inputPanel.height
            }
        }
        transitions: Transition {
            from: ""
            to: "visible"
            reversible: true
            ParallelAnimation {
                NumberAnimation {
                    properties: "y"
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }

    VirtualKeyboard {
        id: virutalKeyboard
        onShowKeyboardRequested: {
            console.log("received onShowKeyboardRequested signal");
            inputPanel.active = true;
            if(!statusBar.fullscreenViewed)
                dockBar.hide();

        }
        onHideKeyboardRequested: {
            console.log("received onHideKeyboardRequested signal");
            inputPanel.active = false;
            if (!statusBar.fullscreenViewed)
                dockBar.show();
        }
        onKeyboardVisibleRequested: {
            console.log("received onKeyboardVisibleRequested signal");
            virutalKeyboard.sendActiveState(inputPanel.active);

        }
    }
    
    DockBar {
        id: dockBar
        onLaunched: {
            console.log('launched by Dock: ' + appid);
            switch (apptype) {
                case "widget":
                {
                    console.log("apptype is widget");
                    if (appid == 'menu') {
                        if (mainmenu.visible) mainmenu.hide()
                        else mainmenu.show()
                    } else if (!sidePanel.launchWidget(appid)) {
                        console.log('no such widget or app');
                    }
                    break;
                }
                case "native":
                {
                    console.log("apptype is native");
                    mainmenu.launchNative(appid);
                    break;
                }
                default:
                    console.log("invalid app type");
            }
        }
    }
    Image {
        id: background
        anchors.top: statusBar.bottom
        width: parent.width - sidePanel.width
        height: parent.height - statusBar.height - dockBar.height

        source: "resources/helix-cockpit.jpg"
        Item {
            id: currentApp
            anchors.fill: parent
        }
        MainMenu {
            id: mainmenu
            height: parent.height
            width: parent.width
            windowDefaultWidth: background.width
            windowDefaultHeight: background.height
            z: 100
            root: helixCockpitView.root
            visible: false
            Component.onCompleted: {
                statusBar.closeWindow.connect(function() {
                    console.log('close clicked');
                    hide();
                })
            }
            onMenuActivated: {
                statusBar.notifyMainMenuStatus(flag);
            }
        }
        /*
        BuiltinNavigation {
        	id: navi
        	anchors.fill: parent
        }
        */
    }
}