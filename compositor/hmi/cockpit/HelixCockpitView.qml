/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
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
    DockBar {
        id: dockBar
        onLaunched: {
            console.log('launched by Dock: '+appid);
            if(appid=='menu') {
                if(mainmenu.visible)
                    mainmenu.hide()
                else
                    mainmenu.show()
            } else if(!sidePanel.launchWidget(appid))
                console.log('no such widget or app');
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