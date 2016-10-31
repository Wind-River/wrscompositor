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
    property string     prefferedPosition:  "";

    signal newSize (int x, int y, int width, int height)


    border.color: Qt.rgba(0.3, 0.3, 0.3, 0.3)
    border.width: 10
    color: Qt.rgba(0.3, 0.3, 0.3, 0.1)

    Component.onCompleted: {
        console.log("onCompleted:" + compositorElement.prefferedPosition)
        //output.windowAdded(compositorElement);
    }

    onNewSize: {
        console.log(">>>>>>>>>>>>>NewSize resize:" + width + "Height" + height);
//        xanimation.to = x;
//        yanimation.to = y;
//        wanimation.to = width;
//        hanimation.to = height;
//        xanimation.start();
//        yanimation.start();
//        wanimation.start();
//        hanimation.start();
        compositorElement.x = x;
        compositorElement.y = y;
        compositorElement.width = width
        compositorElement.height = height
        subItemSurface.requestSize(Qt.size(width, height));
    }

    NumberAnimation on x {
        target: compositorElement
        id: xanimation;
        properties: "x"
        duration: 100
        running: false
    }
    NumberAnimation on y {
        target: compositorElement
        id: yanimation;
        properties: "y"
        running: false
        duration: 100
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
