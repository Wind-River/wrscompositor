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
    signal swapWindowRequested(var anObject)
    signal cloneWindowRequested(var anObject)
    signal closeClonedWindowRequested(var anObject)

    onHasFullscreenWindowChanged: {
        console.log("has fullscreen window: " + hasFullscreenWindow);
    }

    ProjectionMode {
        id: projectionMode

        signal flipHelixCockpitSurface(var identity)
        signal flipProjectionViewSurface(var identity)

        property int androidAuto: 0
        property int appleCarPlay: 1
        property string androidAutoStatus: "none"
        property string appleCarPlayStatus: "none"
        property bool androidAutoProjected: false
        property bool appleCarPlayProjected: false
        property variant androidAutoProjectionContainer: null
        property variant appleCarPlayProjectionContainer: null

        onAndroidAutoStatusChanged: {
            console.log("received onAndroidAutoStatusChanged signal");
            if (projectionMode.androidAutoStatus == "disconnected" && projectionMode.androidAutoProjected) {
                console.log("onAndroidAutoStatusChanged, try to flip helix-cockpit");
                projectionMode.flipHelixCockpitSurface(projectionMode.androidAuto);
            }
        }

        onAppleCarPlayStatusChanged: {
            console.log("onAppleCarPlayStatusChanged, projectionStatus is changed");
            if (projectionMode.appleCarPlayStatus == "disconnected" && projectionMode.appleCarPlayProjected) {
                console.log("onAppleCarPlayStatusChanged, try to flip helix-cockpit");
                projectionMode.flipHelixCockpitSurface(projectionMode.appleCarPlay);
            }
        }

        onReturnToHomeRequested: {
            console.log('return to home !!!');
            projectionMode.flipHelixCockpitSurface(projectionMode.androidAuto);
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
        property int projectionModeIdentity: -1

        front: HelixCockpitView {
            id: helixCockpitView
            root: root
            visible: !projectionMode.androidAutoProjected && !projectionMode.appleCarPlayProjected
            //z: projectionMode.androidAutoProjected?-1:200
        }
        back: Item {
            id: projectionViewList
            width: parent.width
            height: parent.height

            ConnectivityProjectionView { 
                id: androidAutoProjectionView 
                visible: !projectionMode.appleCarPlayProjected
                Component.onCompleted: {
                    projectionMode.androidAutoProjectionContainer = androidAutoProjectionView.projectionView
                }
            }
            ConnectivityProjectionView { 
                id: appleCarPlayPrjectionView 
                visible: !projectionMode.androidAutoProjected 
                Component.onCompleted: {
                    projectionMode.appleCarPlayProjectionContainer = appleCarPlayPrjectionView.projectionView
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
                        projectionMode.sendMousePressed(touchPoints[touch].pointId, touchPoints[touch].x, touchPoints[touch].y);
                    }
                }
                onReleased: {
                    for (var touch in touchPoints) {
                        projectionMode.sendMouseReleased(touchPoints[touch].pointId, touchPoints[touch].x, touchPoints[touch].y);
                    }
                }

                onTouchUpdated: {
                    for (var touch in touchPoints) {
                        projectionMode.sendMouseMove(touchPoints[touch].pointId, touchPoints[touch].x, touchPoints[touch].y);
                    }
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
                projectionMode.androidAutoProjected = false;
                projectionMode.appleCarPlayProjected = false;
                projectionMode.sendVideoFocus(false);
            } 
            else {
                console.log('onSideChanged(back), focused window is projectionView');
                if (windowFrameFlip.projectionModeIdentity == projectionMode.androidAuto) {
                    projectionMode.androidAutoProjected = true;
                    projectionMode.sendVideoFocus(true);
                } else {
                    projectionMode.appleCarPlayProjected = true;
                }
            }
        }
        Component.onCompleted: {
            projectionMode.flipProjectionViewSurface.connect(function(indentity) {
                console.log("Recevied flipProjectionViewSurface signal");
                windowFrameFlip.flipped = true; 
                windowFrameFlip.projectionModeIdentity = indentity;
            })

            projectionMode.flipHelixCockpitSurface.connect(function(indentity) {
                console.log("Recevied flipHelixCockpitSurface signal");
                windowFrameFlip.flipped = false; 
                windowFrameFlip.projectionModeIdentity = indentity;
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
        console.log('surface destroyed '+surface);
        console.log('surface destroyed title:'+surface.title);

        var windowFrame = CompositorLogic.findBySurface(surface);
        if(!windowFrame)
            return;

        if(root.currentWindow == windowFrame)
            root.currentWindow = null;

        if (surface.title == 'OpenGL Renderer' && windowFrame.projectionConnectivityStatus) {
            var name = windowFrame.clientPath;      
            if (name.indexOf('gal_media') != -1) {
                console.log("android-auto is disconnected");
                projectionMode.androidAutoStatus = "disconnected";
            } else if (name.indexOf('DiO-WrDemo') != -1) { 
                console.log("apple-carplay is disconnected");
                projectionMode.appleCarPlayStatus = "disconnected";
            } else {
                 console.log('cannot get valid client by pid for projectionMode, name = ' + name);
                return;
            }
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
            CompositorLogic.relayoutForMultiWindow(helixCockpitView.background.width, 
                helixCockpitView.background.height);

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

        var name = compositor.getProcessNameByPid(surface.client.processId);
        var layer = geniviExt.mainScreen.layerById(1000); // application layer
        var windowContainerComponent = Qt.createComponent("WindowFrame.qml");
        var windowFrame;
        if (surface.title == 'OpenGL Renderer') {  // gstreamer-0.1: gsteglgles
            if (name.indexOf('gal_media') != -1) {
                console.log('wayland android auto');
                projectionMode.androidAutoStatus = "connected";
                windowFrame = windowContainerComponent.createObject(projectionMode.androidAutoProjectionContainer);
            } else if (name.indexOf('DiO-WrDemo') != -1) {  
                console.log('wayland apple carplay');
                projectionMode.appleCarPlayStatus = "connected";
                windowFrame = windowContainerComponent.createObject(projectionMode.appleCarPlayProjectionContainer);   
            } else {
                console.log('cannot get valid client by pid for projectionMode, name = ' + name);
                return;
            }
            windowFrame.clientPath = name;
            windowFrame.projectionConnectivityStatus = true;
            windowFrame.z = -1;
            windowFrame.scaledWidth = Conf.displayWidth/surface.size.width;
            windowFrame.scaledHeight = Conf.displayHeight/surface.size.height;
        } else {
            windowFrame = windowContainerComponent.createObject(helixCockpitView.background);
            windowFrame.projectionConnectivityStatus = false;
            windowFrame.z = 50;   
            windowFrame.scaledWidth = helixCockpitView.background.width/surface.size.width;
            windowFrame.scaledHeight = helixCockpitView.background.height/surface.size.height;
            windowFrame.rootBackground = helixCockpitView.background
        }

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

        if (root.waitProcess && root.waitProcess.pid == surface.client.processId) {
            root.waitProcess.setWindow(windowFrame);
            root.waitProcess = null;
        }

        if (!Conf.useMultiWindowFeature) {
            // XXX scale to fit into main area
            CompositorLogic.addWindow(windowFrame);
        } else { // for multi surface feature enabled mode
            // stretch to maximum size as default
            windowFrame.scaledWidth = helixCockpitView.background.width/surface.size.width;
            windowFrame.scaledHeight = helixCockpitView.background.height/surface.size.height;
            console.log("oscaleds "+ helixCockpitView.background.height/surface.size.height);

            // add surface and relayout for multi surface feature
            CompositorLogic.addMultiWindow(windowFrame,
                                    helixCockpitView.background.width, background.height);
        }

        windowFrame.opacity = 1

        if(!windowFrame.projectionConnectivityStatus) {
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