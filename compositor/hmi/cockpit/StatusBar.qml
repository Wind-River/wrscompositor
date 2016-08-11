/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
import QtQuick 2.1
import "config.js" as Conf
import QtGraphicalEffects 1.0


Item {
    id: statusBar
    anchors.top: parent.top
    width: parent.width
    height: parent.height/10
    z: 50000
    property string wmi: ""
    property bool currentWindowExposed: false
    property bool cloneAvailable: true
    property bool fullscreenViewed: false
    property bool mainMenuActivated: false

    signal closeWindow
    signal logoClicked
    signal swapWindow
    signal cloneWindow
    signal clickFullscreenWindow

    FontLoader { id: tungsten; source: "fonts/Tungsten-Light.otf" }


    Rectangle {
        id: statusbarBackground
        color: "black"
        anchors.fill: parent
    }

    LinearGradient {
        width: parent.width * 5.39 / 16
        height: parent.height
        anchors.right: parent.right
        start: Qt.point(0, 0)
        end: Qt.point(300, 0)
        gradient: Gradient {
            GradientStop { position: 0.0; color: "black" }
            GradientStop { position: 1.0; color: "#3BBDED" }
        }
    }

    RingIcon {
        id: navi_icon
        icon: "resources/navi.svg"
        anchors.left: parent.left
        anchors.leftMargin: parent.width * 0.01
        anchors.verticalCenter: parent.verticalCenter
        height: parent.height * 0.7
        width: height
        smooth: true
        MouseArea {
            id: logoButtonArea
            anchors.fill: parent
            onClicked: {
                logoClicked();
            }
        }
    }


    /*
    Text {
        id: wmi_title
        anchors.left: logo_title.right
        anchors.verticalCenter: parent.verticalCenter
        text: statusBar.wmi
        visible: statusBar.wmi != ""
        font.pointSize: ((statusBar.height*0.4/Conf.densityAdjust)|0)
        color: "white"
        smooth: true
        font.bold: true
    }
    Image {
        id: cloneWindowButton
        source: statusBar.cloneAvailable?"icons/clone-window.svg":"icons/restore-cloned-window.svg"
        anchors.right: androidAuto.left
        anchors.rightMargin: width/10
        anchors.verticalCenter: parent.verticalCenter
        visible: Conf.useMultiWaylandDisplayFeature && statusBar.currentWindowExposed
        width: height
        height: parent.height * 3 / 5
        MouseArea {
            id: cloneButtonArea
            anchors.fill: parent
            onClicked: {
                cloneWindow();
            }
        }
        opacity: (cloneButtonArea.pressed?0.8:1.0)
        scale: (cloneButtonArea.pressed?0.9:1.0)
        smooth: true
    }

    Image {
        id: swapWindowButton
        source: "icons/swap-window.svg"
        anchors.right: cloneWindowButton.left
        anchors.rightMargin: width/10
        anchors.verticalCenter: parent.verticalCenter
        visible: Conf.useMultiWaylandDisplayFeature && statusBar.currentWindowExposed && statusBar.cloneAvailable
        width: height
        height: parent.height * 3 / 5
        MouseArea {
            id: swapButtonArea
            anchors.fill: parent
            onClicked: {
                swapWindow();
            }
        }
        opacity: (swapButtonArea.pressed?0.8:1.0)
        scale: (swapButtonArea.pressed?0.9:1.0)
        smooth: true
    }
    */
    Text {
        id: dateTime1
        anchors.left: navi_icon.right
        anchors.leftMargin: parent.width/60
        anchors.verticalCenter: parent.verticalCenter
        //text: Qt.formatDateTime(new Date(), "yyyy/MM/dd hh:mm:ss")
        text: Qt.formatDateTime(new Date(), "hh:mm")
        font.pointSize: ((statusBar.height*0.35/Conf.densityAdjust)|0)
        font.family: tungsten.name
        font.bold: true
        color: "white"
        smooth: true
    }

    Timer {
        interval: 1000; running: true; repeat: true
        onTriggered: dateTime1.text = Qt.formatDateTime(new Date(), "hh:mm")
    }

    Text {
        id: dateTime2
        anchors.left: dateTime1.right
        anchors.leftMargin: parent.width/60
        anchors.verticalCenter: parent.verticalCenter
        //text: Qt.formatDateTime(new Date(), "yyyy/MM/dd hh:mm:ss")
        text: Qt.formatDateTime(new Date(), "dddd MMMM dd")
        font.pointSize: ((statusBar.height*0.35/Conf.densityAdjust)|0)
        font.family: tungsten.name
        color: "white"
        smooth: true
    }

    Timer {
        interval: 1000; running: true; repeat: true
        onTriggered: dateTime2.text = Qt.formatDateTime(new Date(), "dddd MMMM dd")
    }

    Image {
        id: wifi
        source: "resources/wifi-symbol.svg"
        anchors.left: dateTime2.right
        anchors.leftMargin: parent.width/70
        anchors.verticalCenter: parent.verticalCenter
        width: (height*sourceSize.width)/sourceSize.height
        height: statusBar.height * 0.3
        smooth: true
    }
    Image {
        id: rssi
        source: "resources/signal-symbol.svg"
        anchors.left: wifi.right
        anchors.leftMargin: parent.width/70
        anchors.verticalCenter: parent.verticalCenter
        width: (height*sourceSize.width)/sourceSize.height
        height: statusBar.height * 0.3
        smooth: true
    }
    Image {
        id: fullscreen
        source: statusBar.fullscreenViewed ? "icons/full-screen-close.png" : "icons/full-screen.png"
        anchors.left: rssi.right
        anchors.leftMargin: parent.width/70
        anchors.verticalCenter: parent.verticalCenter
        width: (height*sourceSize.width)/sourceSize.height
        height: statusBar.height * 0.7
        smooth: true
        MouseArea {
            id: fullscreenButtonArea
            anchors.fill: parent
            onClicked: {
                clickFullscreenWindow();
            }
        }
        scale: (fullscreenButtonArea.pressed? 0.9 : 1.0)
    }

    Image {
        id: androidAuto
        source: (projectionMode.androidAutoStatus == "connected") ? "icons/android-auto.png" : "icons/android-auto-grey.png"
        anchors.left: fullscreen.right
        anchors.leftMargin: parent.width/70
        anchors.verticalCenter: parent.verticalCenter
        width: (height*sourceSize.width)/sourceSize.height
        height: statusBar.height * 0.6
        smooth: true
        MouseArea {
            id: aapButtonArea
            anchors.fill: parent
            onClicked: {
                if (projectionMode.androidAutoStatus == "connected" && !projectionMode.androidAutoProjected) {
                    console.log('AndroidAutoStatus is connected, try to flip projectionView');
                    projectionMode.flipProjectionViewSurface(projectionMode.androidAuto);
                }
            }
        }
        // opacity: (aapButtonArea.pressed? 0.8 : 1.0)
        scale: (aapButtonArea.pressed? 0.9 : 1.0)
    }
    Image {
        id: appleCarPlay
        source: (projectionMode.appleCarPlayStatus == "connected") ? "icons/apple-carplay.png" : "icons/apple-carplay-grey.png"
        anchors.left: androidAuto.right
        anchors.leftMargin: parent.width/70
        anchors.verticalCenter: parent.verticalCenter
        width: (height*sourceSize.width)/sourceSize.height
        height: statusBar.height * 0.6
        smooth: true
        MouseArea {
            id: carPlayButtonArea
            anchors.fill: parent
            onClicked: {
                if (projectionMode.appleCarPlayStatus == "connected" && !projectionMode.appleCarPlayProjected) {
                    console.log('appleCarPlay is connected, try to flip projectionView');
                    projectionMode.flipProjectionViewSurface(projectionMode.appleCarPlay);
                } 
            }
        }
        scale: (carPlayButtonArea.pressed? 0.9 : 1.0)
    }
    Image {
        id: weather
        source: "tango/error.svg"
        anchors.right: temp.left
        anchors.rightMargin: parent.width/70
        anchors.verticalCenter: parent.verticalCenter
        width: (height*sourceSize.width)/sourceSize.height
        height: statusBar.height * 0.7
        smooth: true
    }

    Text {
        id: temp
        anchors.right: parent.right
        anchors.rightMargin: parent.width/60
        anchors.verticalCenter: parent.verticalCenter
        text: "0°"
        font.pointSize: ((statusBar.height*0.35/Conf.densityAdjust)|0)
        font.family: tungsten.name
        font.bold: true
        color: "white"
        smooth: true
    }

    function notifyMainMenuStatus(flag) {
        mainMenuActivated = flag;
    }

    function setWMI(wmi) {
        statusBar.wmi = wmi
    }
    Timer {
        id: weatherCrawler
        interval: 2000; running: true; repeat: true
        onTriggered: {
            weatherCrawler.interval = 360000;
            var doc = new XMLHttpRequest();
            doc.onreadystatechange = function() {
                if (doc.readyState == XMLHttpRequest.HEADERS_RECEIVED) {
                    console.log('header received');
                    console.log(doc.status);
                    console.log(doc.readyState);
                    console.log(doc.getAllResponseHeaders ());
                    console.log(doc.getResponseHeader ("Last-Modified"));
                } else if (doc.readyState == XMLHttpRequest.DONE) {
                    console.log('done')
                    if(doc.status==200) {
                        console.log(doc.readyState);
                        var data = JSON.parse(doc.responseText);
                        //console.log(data.city);
                        //console.log(data.city.name);
                        //console.log(data.list.length);
                        if(data.list.length > 0) {
                            var i = data.list[0].weather[0];
                            // kelvin to celcius
                            var t=parseInt(data.list[0].main.temp-273.15);
                            //console.log(i.description);
                            if(i.icon == '01n')
                                weather.source='tango/01n.svg';
                            else
                                weather.source='tango/'+i.icon.slice(0, 2)+'d.svg';
                            temp.text = t+"°"
                        }
                    } else
                        weather.source='tango/error.svg';
                }
            }
            doc.onerror = function() {
                weather.source='tango/error.svg';
            }
            doc.open("GET", "http://api.openweathermap.org/data/2.5/forecast/city?id=1835848&APPID=a65e5b4240ef1dacf58c8d121734e48d");
            doc.send();
        }
    }

}