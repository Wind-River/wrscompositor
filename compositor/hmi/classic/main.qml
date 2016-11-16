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
        var component                       = Qt.createComponent("CompositorElement.qml");
        var item                            = component.createObject(output);
        var subItem                         = compositor.item(surface);
        subItem.parent                      = item;
        item.subItemName                    = name;
        item.subItemSurface                 = surface;
        item.subItemIVISurface              = iviScene.findIVISurfaceByQWaylandSurface(surface);
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
        {
            var component1           = Qt.createComponent("CompositorElement.qml", Component.PreferSynchronous);
            var subComponent1        = Qt.createComponent("StatusBar.qml", Component.PreferSynchronous);
            var item1                = component1.createObject(output);
            var subItem1             = subComponent1.createObject(item1);
            item1.subItemName        = "StatusBar";
            var iviSurface1 = iviScene.createSurface(item1.x, item1.y, item1.width, item1.height, item1);
            iviScene.addIVISurface(iviSurface1);
            compositorWindowAdded(item1);
        }
        {
            var component2           = Qt.createComponent("CompositorElement.qml", Component.PreferSynchronous);
            var subComponent2        = Qt.createComponent("LauncherBar.qml", Component.PreferSynchronous);
            var item2                = component2.createObject(output);
            var subItem2             = subComponent2.createObject(item2);
            item2.subItemName        = "LauncherBar";
            var iviSurface2 = iviScene.createSurface(item2.x, item2.y, item2.width, item2.height, item2);
            iviScene.addIVISurface(iviSurface2);
            compositorWindowAdded(item2);
        }
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
