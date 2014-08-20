import QtQuick 2.0
import "compositor.js" as CompositorLogic
import com.windriver.duduregi 1.0

Item {
    id: root

    property variant currentWindow: null
    property variant waitProcess: null

    property variant selectedWindow: null
    property bool hasFullscreenWindow: typeof compositor != "undefined" && compositor.fullscreenSurface !== null

    onHasFullscreenWindowChanged: {
        console.log("has fullscreen window: " + hasFullscreenWindow);
    }

    VNADBusClient {
        id: dbusClient

        onVehicleInfoChanged: {
            console.log('vehicleInfoChanged : '+vehicleInfoWMI);
			statusBar.setWMI(vehicleInfoWMI)
        }
    }

    StatusBar {
        id: statusBar
    }
    Image {
        id: background
        Behavior on opacity {
            NumberAnimation { easing.type: Easing.InCubic; duration: 400; }
        }
        anchors.top: statusBar.bottom
        width: parent.width
        height: parent.height - statusBar.height

        fillMode: Image.PreserveAspectCrop
        source: "alameda.jpg"
        smooth: true

        Item {
            id: currentApp
            anchors.fill: parent
        }

        MainMenu {
            id: mainmenu 
            root: root
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
    }
    /*
    WebView  {
        id: launcher
        //url: "http://0.0.0.0:7070/html5apps/WRLauncher/"
        url: "file:///opt/windriver/wr-amb/www/html5apps/WRLauncher/index.html"
        anchors.fill: parent
        focus: true
        Keys.onPressed: {
            console.log('key '+event.key);
            if (event.key == Qt.Key_F1) {
                console.log('menu');
                launcher.url = "file:///opt/windriver/wr-amb/www/html5apps/WRLauncher/index.html";
            } else if (event.key == Qt.Key_Backspace) {
                console.log('back');
                launcher.goBack();
            }
        }
    }
    */

    /*
    MouseArea {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        width: 2
        height: 2
        hoverEnabled: true
        onEntered: {
            console.log('entered');
            root.selectedWindow = null
            root.focus = true
        }
        z: 10
    }
    */
    function raiseWindow(window) {
        if(root.currentWindow != null)
            root.currentWindow.visible = false
        root.currentWindow = window
        root.currentWindow.visible = true
        if(mainmenu.visible)
            mainmenu.hide();
    }

    function windowAdded(window) {
        console.log('window added '+window);
        console.log(currentApp.width+' '+ currentApp.height);
        compositor.configure(window.surface, currentApp.width, currentApp.height);
        var windowContainerComponent = Qt.createComponent("WindowFrame.qml");
        var windowContainer = windowContainerComponent.createObject(root);

        window.parent = windowContainer;
        windowContainer.child = window;
        windowContainer.width = window.width;
        windowContainer.height = window.height;
        windowContainer.parent = currentApp
        if(root.currentWindow != null)
            root.currentWindow.visible = false
        root.currentWindow = windowContainer

        if(mainmenu.visible)
            mainmenu.hide();

        if(root.waitProcess) {
            root.waitProcess.setWindow(windowContainer);
            root.waitProcess = null;
        }
    }

    function windowDestroyed(window) {
        console.log('window destroyed '+window);
        if (window && window.parent) {
            //root.currentWindow.runDestroyAnimation();
            window.parent.child = null
            window.parent.destroy();
        }
    }

    function windowResized(window) {
        console.log('window resized '+window);
    }
    /*
    function windowAdded(window) {
        console.log('window added '+window);
        var windowContainerComponent = Qt.createComponent("WindowContainer.qml");
        var windowContainer = windowContainerComponent.createObject(root);

        window.parent = windowContainer;

        windowContainer.targetWidth = window.width;
        windowContainer.targetHeight = window.height;
        windowContainer.child = window;

        var windowChromeComponent = Qt.createComponent("WindowChrome.qml");
        var windowChrome = windowChromeComponent.createObject(window);

        CompositorLogic.addWindow(windowContainer);

        windowContainer.opacity = 1
        windowContainer.animationsEnabled = true;
        windowContainer.chrome = windowChrome;
    }

    function windowResized(window) {
        console.log('window resized '+window);
        var windowContainer = window.parent;
        windowContainer.width = window.width;
        windowContainer.height = window.height;

        CompositorLogic.relayout();
    }

    function windowDestroyed(window) {
        console.log('window destroyed '+window);
        return;
        var windowContainer = window.parent;
        if (windowContainer.runDestroyAnimation)
            windowContainer.runDestroyAnimation();
    }

    function removeWindow(window) {
        console.log('window remove '+window);
        var windowContainer = window.parent;
        CompositorLogic.removeWindow(windowContainer);
        windowContainer.chrome.destroy();
        windowContainer.destroy();
        compositor.destroyWindow(window);
    }


    onHeightChanged: CompositorLogic.relayout();
    onWidthChanged: CompositorLogic.relayout();
    Component.onCompleted: {
    }
    */

    Keys.onPressed: {
        console.log('key on main: '+event.key);
        if (event.key == Qt.Key_F1) {
            mainmenu.show()
        } else if (event.key == Qt.Key_Backspace) {
            console.log('backspace');
            if(mainmenu.visible)
                mainmenu.hide();
        }
    }
}
