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
    id: root

    height: windowHeight
    width: windowWidth

    property variant currentWindow: null
    property variant waitProcess: null

    property variant selectedWindow: null
    property bool hasFullscreenWindow: typeof compositor != "undefined" && compositor.fullscreenSurface !== null
    property int surfaceWidth: root.width - sidePanel.width
    property int surfaceHeight: root.height - statusBar.height - dockBar.height

    signal swapWindowRequested(var anObject)
    signal cloneWindowRequested(var anObject)
    signal closeClonedWindowRequested(var anObject)
    signal flipWindowFrameRequested

    onHasFullscreenWindowChanged: {
        console.log("has fullscreen window: " + hasFullscreenWindow);
    }

    ProjectionMode {
        id: projectionModeAndroidAuto

        property bool androidAutoEnabled: false
        property bool androidAutoProjectionMode: null
        property variant androidAutoContainer: null

        onReturnToHomeRequested: {
            console.log('return to home !!!');
            root.flipWindowFrameRequested();
        }
    }

    /*
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
    */

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

    Flipable {
        id: windowFrameFlip
        width: parent.width
        height: parent.height
        property bool flipped: false

        front: Rectangle { 
            id: helixCockpit
            width: parent.width
            height: parent.height
            visible: !projectionModeAndroidAuto.androidAutoProjectionMode
            //z: projectionModeAndroidAuto.androidAutoProjectionMode?-1:200
            StatusBar {
                id: statusBar
                androidAutoEnabled: projectionModeAndroidAuto.androidAutoEnabled
                androidAutoProjectionMode: projectionModeAndroidAuto.androidAutoProjectionMode
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

                source: "resources/background.jpg"

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
                    root: root
                    visible: false
                    Component.onCompleted: {
                        statusBar.closeWindow.connect(function() {
                            console.log('close clicked');
                            hide();
                            })
                    }
                    onMenuActivated: {
                        statusBar.showCloseButton(flag);
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
        back: Rectangle {
            id: fullscreenProjection
            width: parent.width
            height: parent.height
            anchors.top: parent.top
            color: "black"
            VideoOutput {
                id: projectionView
                source: mediaPlayer
                width: parent.width
                height: parent.height
                property string projectionStatus: "none"
                onProjectionStatusChanged: {
                    if (projectionView.projectionStatus == "disconnected" && projectionView.androidAutoProjectionMode) {
                        console.log("try to flip helix-cockpit");
                        root.flipWindowFrameRequested();
                    }
                }
                MediaPlayer {
                    id: mediaPlayer
                    autoLoad: false
                    loops: Audio.Infinite
                    onError: {
                        if (MediaPlayer.NoError != error) {
                            console.log("[41m[qmlvideo][0m VideoItem.onError error " + error + " errorString " + errorString)
                        }
                    }
                }

                MultiPointTouchArea {
                    id: projectionViewTouchArea
                    anchors.fill: parent
                    mouseEnabled: true
                    minimumTouchPoints: 1
                    maximumTouchPoints: 4

                    onPressed: {
                        for (var touch in touchPoints) {
                            projectionModeAndroidAuto.sendMousePressed(touchPoints[touch].pointId, touchPoints[touch].x, touchPoints[touch].y);
                        }
                    }
                    onReleased: {
                        for (var touch in touchPoints) {
                            projectionModeAndroidAuto.sendMouseReleased(touchPoints[touch].pointId, touchPoints[touch].x, touchPoints[touch].y);
                        }
                    }

                    onTouchUpdated: {
                        for (var touch in touchPoints) {
                            projectionModeAndroidAuto.sendMouseMove(touchPoints[touch].pointId, touchPoints[touch].x, touchPoints[touch].y);
                        }
                    }
                }
                Keys.onPressed: {
                    console.log('key pressed on projection: '+event.key);
                    projectionModeAndroidAuto.sendKeyPressed(event.key);
                }
                Keys.onReleased: {
                    console.log('key released on projection: '+event.key);
                    projectionModeAndroidAuto.sendKeyReleased(event.key);
                }
                Component.onCompleted: {
                    projectionModeAndroidAuto.mediaPlayer = mediaPlayer
                    projectionModeAndroidAuto.androidAutoContainer = projectionView
                }
            }
        }
        transform: Rotation {
            id: rotation
            origin.x: windowFrameFlip.width/2
            origin.y: windowFrameFlip.height/2
            axis.x: 0; axis.y: 1; axis.z: 0     // set axis.y to 1 to rotate around y-axis
            angle: 0    // the default angle
        }
        states: State {
            name: "back"
            PropertyChanges { target: rotation; angle: 180 } 
            when: windowFrameFlip.flipped
        }
        transitions: Transition {
            NumberAnimation { target: rotation; property: "angle"; duration: 500 }
        }

        onSideChanged: {
            if(side==Flipable.Front) {
                console.log('onSideChanged(front), focused window is helix-cockpit');
                projectionModeAndroidAuto.androidAutoProjectionMode = false;
                projectionModeAndroidAuto.sendVideoFocus(false);
            } else {
                console.log('onSideChanged(back), focused window is projectionView');
                projectionModeAndroidAuto.androidAutoProjectionMode = true;
                projectionModeAndroidAuto.sendVideoFocus(true);
            }
        }
        Component.onCompleted: {
            root.flipWindowFrameRequested.connect(function() {
                console.log("Recevied flipWindowFrameRequested signal");
                windowFrameFlip.flipped = !windowFrameFlip.flipped
            })
        }
    }

    function raiseWindow(window) {
        if(root.currentWindow != null)
            root.currentWindow.visible = false
        root.currentWindow = window
        root.currentWindow.visible = true
        if(mainmenu.visible)
            mainmenu.hide();
    }

    function swappedWindowRestored(surfaceItem) {
        if(!Conf.useMultiWaylandDisplayFeature)
            return;
        console.log("swappedWindowRestored: "+surfaceItem);

        var windowFrame = CompositorLogic.findBySurface(surfaceItem.surface);
        console.log(windowFrame);
        root.raiseWindow(windowFrame);
    }
    function windowDestroyed(surface) {
        console.log('window destroyed '+surface);
        var windowFrame = CompositorLogic.findBySurface(surface);
        if(!windowFrame)
            return;
        if(root.currentWindow == windowFrame)
            root.currentWindow = null;

        if(windowFrame.androidAutoProjection) {
            projectionModeAndroidAuto.androidAutoEnabled = false;
            projectionModeAndroidAuto.androidAutoContainer.projectionStatus = "disconnected";
        }

        var layer = geniviExt.mainScreen.layerById(1000); // application layer
        layer.removeSurface(windowFrame.ivi_surface);
        console.log('position '+windowFrame.position);
        if(Conf.useMultiWaylandDisplayFeature && (windowFrame.cloned || windowFrame.position != 'main')) {
            root.closeClonedWindowRequested(windowFrame.surfaceItem);
        }
        windowFrame.destroy();
        CompositorLogic.removeWindow(windowFrame);
        if(Conf.useMultiWindowFeature)
            CompositorLogic.relayoutForMultiWindow(background.width, background.height);

    }

    function windowAdded(surface) {
        console.log('surface added '+surface);
        console.log('surface added title:'+surface.title);
        console.log('surface added className:'+surface.className);
        console.log('surface added client: '+surface.client);
        console.log('surface added pid: '+surface.client.processId);
        console.log(geniviExt.mainScreen);
        console.log(geniviExt.mainScreen.layerCount());
        console.log(geniviExt.mainScreen.layer(0));
        console.log(geniviExt.mainScreen.layer(0).visibility);
        console.log(currentApp.width+' '+ currentApp.height);

        var layer = geniviExt.mainScreen.layerById(1000); // application layer
        var windowContainerComponent = Qt.createComponent("WindowFrame.qml");
        var windowFrame;
        if(surface.title == 'gsteglgles' || surface.title == 'OpenGL Renderer') {
            // XXX surface from android on Minnow Max target
            console.log('wayland android auto');
            windowFrame = windowContainerComponent.createObject(projectionModeAndroidAuto.androidAutoContainer);
            windowFrame.androidAutoProjection = true
            projectionModeAndroidAuto.androidAutoEnabled = true;
            projectionModeAndroidAuto.androidAutoContainer.projectionStatus = "connected";
        } else
            windowFrame = windowContainerComponent.createObject(background);

        windowFrame.rootBackground = background
        windowFrame.z = 50
        windowFrame.width = surface.size.width;
        windowFrame.height = surface.size.height;
        windowFrame.surface = surface;
        windowFrame.surfaceItem = compositor.item(surface);
        windowFrame.surfaceItem.parent = windowFrame;
        windowFrame.surfaceItem.touchEventsEnabled = true;
        windowFrame.ivi_surface = layer.addSurface(0, 0, surface.size.width, surface.size.height, windowFrame);
        windowFrame.ivi_surface.id = surface.client.processId;

        windowFrame.targetX = 0;
        windowFrame.targetY = 0;
        windowFrame.targetWidth = surface.size.width;
        windowFrame.targetHeight = surface.size.height;
        if(windowFrame.androidAutoProjection) {
            windowFrame.z = -1
            windowFrame.targetX = 0;
            windowFrame.targetY = 0;
            windowFrame.scaledWidth = Conf.displayWidth/surface.size.width;
            windowFrame.scaledHeight = Conf.displayHeight/surface.size.height;
        }

        if(root.waitProcess && root.waitProcess.pid == surface.client.processId)
        {
            root.waitProcess.setWindow(windowFrame);
            root.waitProcess = null;
        }

        if(!Conf.useMultiWindowFeature) {
            // XXX scale to fit into main area
            if (!windowFrame.androidAutoProjection) {
                console.log("background.width: "+background.width);
                windowFrame.scaledWidth = background.width/surface.size.width;
                windowFrame.scaledHeight = background.height/surface.size.height;
            }
            CompositorLogic.addWindow(windowFrame);
        } else { // for multi surface feature enabled mode
            // stretch to maximum size as default
            windowFrame.scaledWidth = background.width/surface.size.width;
            windowFrame.scaledHeight = background.height/surface.size.height;
            console.log("oscaleds "+background.height/surface.size.height);

            // add surface and relayout for multi surface feature
            CompositorLogic.addMultiWindow(windowFrame,
                                    background.width, background.height);
        }

        windowFrame.opacity = 1

        if(!windowFrame.androidAutoProjection) {
            if(!Conf.useMultiWindowFeature)
                CompositorLogic.hideWithout(windowFrame);
            root.currentWindow = windowFrame

            if(mainmenu.visible)
                mainmenu.hide();
        }
    }

    function windowResized(surface) {
        console.log('surface resized '+surface);
        surface.width = surface.surface.size.width;
        surface.height = surface.surface.size.height;
    }

    Keys.onPressed: {
        console.log('key on main: '+event.key);
        if (event.key == Qt.Key_F1) {
            if(mainmenu.visible)
                mainmenu.hide()
            else
                mainmenu.show()
        } else if (event.key == Qt.Key_Backspace) {
            console.log('backspace');
            if(mainmenu.visible)
                mainmenu.hide();
        }
    }
    onWidthChanged: {
        Conf.displayWidth = width;
    }
    onHeightChanged: {
        Conf.displayHeight = height;
    }
    Component.onCompleted: {
        if(!Conf.useMultiWaylandDisplayFeature)
            return;
        statusBar.swapWindow.connect(function() {
            console.log("swap button clicked");
            console.log(root.currentWindow);
            if(root.currentWindow.cloned)
                return;
            root.currentWindow.position = "rear";
            root.swapWindowRequested(root.currentWindow);
            root.currentWindow.visible = false;
        });
        statusBar.cloneWindow.connect(function() {
            console.log("clone button clicked");
            if(root.currentWindow.cloned) {
                root.closeClonedWindowRequested(root.currentWindow.surfaceItem);
                root.currentWindow.cloned = false;
            } else {
                root.cloneWindowRequested(root.currentWindow);
                root.currentWindow.cloned = true;
            }
        });
    }
}