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
import com.windriver.automotive 1.0
import com.windriver.genivi 1.0
import "CompositorLayout.js" as CompositorLayout
import QtGraphicalEffects 1.0;

Item {
    LinearGradient {
        anchors.fill: parent
        start: Qt.point(parent.width, 0)
        end: Qt.point(parent.width - (parent.width / 3), 0)
        gradient: Gradient {
            GradientStop { position: 0.0; color: "red" }
            GradientStop { position: 1.0; color: "black" }
        }
    }

    signal compositorWindowAdded( variant compositorElement );
    signal compositorwindowRemoved( variant compositorElement );

    signal createWestonSurfaceToCompositorElement( variant surface, string name );
    signal createWebViewToCompositorElement( string url, string name );
    signal createQmlComponentToCompositorElement(string qmlFile, string name );

    signal destoryCompositorElement( string name);


    id: output
    x: 0
    y: 0
    width: 1855
    height: 1056

    onCompositorWindowAdded: {
        console.log("[DEBUG] Window Added!:" + compositorElement);
        CompositorLayout.onWindowAdded(compositorElement);
    }


    onCompositorwindowRemoved: {
        console.log("[DEBUG] Window Removed!:" + compositorElement);
        CompositorLayout.onWindowRemoved(compositorElement);
    }


    onCreateWestonSurfaceToCompositorElement: {
        var iviSurface                      = iviScene.findIVISurfaceByQWaylandSurface(surface);
        var component                       = Qt.createComponent("CompositorElement.qml");
        var item                            = component.createObject(iviSurface.layer().qmlWindowFrame());
        var subItem                         = compositor.item(surface);
        subItem.parent                      = item;
        item.subItemName                    = name;
        item.subItemSurface                 = surface;
        item.width                          = subItem.width;
        item.height                         = subItem.height;
        item.subItemIVISurface              = iviSurface;
        subItem.anchors.horizontalCenter    = item.horizontalCenter;
        subItem.anchors.verticalCenter      = item.verticalCenter;
        item.subItemIVISurface.setQmlWindowFrame(item);
        compositorWindowAdded(item);
    }


    onCreateWebViewToCompositorElement: {
        var component = Qt.createComponent("CompositorElement.qml");
        var item = component.createObject(output);
        var webview = Qt.createQmlObject(
            "import QtQuick 2.1; " +
            "import QtWebEngine 1.2; " +
            "   WebEngineView { anchors.fill: parent;}",
            item);
        webview.url = url;
        item.subItem = webview;
        item.subItemName = name;
        var iviSurface = iviScene.createSurface(item.x, item.y, item.width, item.height, item);
        item.subItemIVISurface = iviSurface;
        iviScene.addIVISurface(iviSurface);
        item.parent = iviSurface.layer().qmlWindowFrame();
        compositorWindowAdded(item);        
    }


    onCreateQmlComponentToCompositorElement: {
        var component           = Qt.createComponent("CompositorElement.qml");
        var subComponent        = Qt.createComponent(qmlFile);
        var item                = component.createObject(output);
        var subItem             = subComponent.createObject(item);
        item.subItemName        = name;        
        var iviSurface = iviScene.createSurface(item.x, item.y, item.width, item.height, item);
        item.subItemIVISurface = iviSurface;
        iviScene.addIVISurface(iviSurface);
        item.parent = iviSurface.layer().qmlWindowFrame();
        compositorWindowAdded(item);
    }


    onDestoryCompositorElement: {
        var compositorElement = CompositorLayout.getCompositorElementByName(name);
        iviScene.removeIVISurface(compositorElement.subItemIVISurface);
        CompositorLayout.onWindowRemoved(compositorElement);
        compositorElement.destroy();
    }


    onHeightChanged: {
        console.log("ROOOT HEIGHT CHANGED!\n")
        CompositorLayout.compositorReLayout();
    }


    onWidthChanged: {
        console.log("ROOOT WIDTH CHANGED!\n")
        CompositorLayout.compositorReLayout();
    }


    Component.onCompleted: {
        //Create compositor layers

        var screenCount = iviScene.screenCount();
        for (var i = 0; i < screenCount; i++) {
            var screen = iviScene.screen(i);
            var layerCount = screen.layerCount();
            for (var j = 0; j < layerCount; j++) {
                var layer = screen.layer(j);
                var component        = Qt.createComponent("CompositorLayer.qml");
                var item             = component.createObject(output);
                item.z               = layer.id;
                item.subItemIVILayer = layer;
                item.subItemIVILayer.setQmlWindowFrame(item);
            }
        }


        //Create buiuld in surfaces (optional)
        createQmlComponentToCompositorElement("StatusBar.qml", "StatusBar");
        createQmlComponentToCompositorElement("LauncherBar.qml", "LauncherBar");

        CompositorLayout.setIviScene(iviScene);
        CompositorLayout.compositorReLayout();
    }

    /////////////////////////////////////////////////////////////////////////////////
    //Weston integration API
    /////////////////////////////////////////////////////////////////////////////////

    function raiseWindow(window) {
        CompositorLayout.compositorReLayout();
    }

    function swappedWindowRestored(surfaceItem) {
//        if(!Conf.useMultiWaylandDisplayFeature)
//            return;
//        console.log("swappedWindowRestored: "+surfaceItem);

//        var windowFrame = CompositorLogic.findBySurface(surfaceItem.surface);
//        console.log(windowFrame);
//        root.raiseWindow(windowFrame);
    }


    function windowDestroyed(surface) {
        console.log('=====================\n');
        console.log('window destroyed \n\n\n' + surface);

        var compositorElement = CompositorLayout.getCompositorElementBySurface(surface);
        CompositorLayout.onWindowRemoved(compositorElement);
        compositorElement.destroy();
    }


    function windowAdded(surface) {
        console.log('surface added ' + surface);
        console.log('surface added title:' + surface.title);
        console.log('surface added className:' + surface.className);
        console.log('surface added client: ' + surface.client);
        console.log('surface added pid: ' + surface.client.processId);
        console.log('surface added cmd: ' + util.getCmdForPid(surface.client.processId));
        console.log(iviScene.mainScreen);
        console.log(iviScene.mainScreen.layerCount());
        console.log(iviScene.mainScreen.layer(0));
        console.log(iviScene.mainScreen.layer(0).visibility);
        var role = iviScene.getSurfaceRole(surface);

        createWestonSurfaceToCompositorElement(surface, role);
    }


    function windowResized(surface) {
        console.log('!!!!!!!!!!!!!! surface resized '+surface);
        surface.width = surface.surface.size.width;
        surface.height = surface.surface.size.height;
        CompositorLayout.compositorReLayout();
    }

}
