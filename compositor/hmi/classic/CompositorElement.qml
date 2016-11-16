import QtQuick 2.1

/**
 * Represents the compositor window frame
 * Layout
 */
Rectangle {
    id: compositorElement;
    property variant    subItem:            null;
    property string     subItemName:        "";
    property variant    subItemSurface:     null;
    property variant    subItemIVISurface:  null;
    property string     prefferedPosition:  "";

    signal newSize (int x, int y, int width, int height)


    //border.color: Qt.rgba(0.3, 0.3, 0.3, 0.3)
    //border.width: 2
    color: Qt.rgba(0.3, 0.3, 0.3, 0.1)

    Component.onCompleted: {
        console.log("onCompleted:" + compositorElement.prefferedPosition)
        //output.windowAdded(compositorElement);
    }

    onNewSize: {
        console.log("[DEBUG] onNewSize:" + width + "/" + height);
        console.log("[DEBUG] onNewPos:" + x + "/" + y);
        console.log("[DEBUG] compositorElement name:" + compositorElement.subItemName);
        //xanimation.to = x;
        //yanimation.to = y;
        //wanimation.to = width;
        //hanimation.to = height;
        //xanimation.start();
        //yanimation.start();
        //wanimation.start();
        //hanimation.start();
        compositorElement.x = x;
        compositorElement.y = y;
        compositorElement.width = width
        compositorElement.height = height
        if (subItemSurface != null) {
            subItemSurface.requestSize(Qt.size(width, height));
        }
    }

    NumberAnimation on x {
        target: compositorElement
        id: xanimation;
        properties: "x"
        running: false
        duration: 100
        onRunningChanged: {
            if(!xanimation.running) {
                console.log("[DEBUG] onNewPosAnimationX END:" + xanimation.to);
                compositorElement.x = xanimation.to;
            }
        }
    }

    NumberAnimation on y {
        target: compositorElement
        id: yanimation;
        properties: "y"
        running: false
        duration: 100
        onRunningChanged: {
            if(!xanimation.running) {
                console.log("[DEBUG] onNewPosAnimationY END:" + yanimation.to);
                compositorElement.y = yanimation.to;
            }
        }

    }
    NumberAnimation on width {
        target: compositorElement
        id: wanimation;
        properties: "width"
        running: false
        duration: 100
    }
    NumberAnimation on height {
        target: compositorElement
        id: hanimation;
        properties: "height"
        running: false
        duration: 100
    }
}
