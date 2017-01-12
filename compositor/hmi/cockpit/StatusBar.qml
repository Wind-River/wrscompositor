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
import QtGraphicalEffects 1.0
import "config.js" as Conf
import "hmi-interface.js" as Interface

Item {
    id: statusBar
    width: parent.width
    height: parent.height

    function eventHandler(event, arg) {
        var window = arg;

        switch(event) {
            case Interface.Event.WindowAdded:
                console.log("StatusBar, eventHandler receive WindowAdded event");
                break;
            case Interface.Event.WindowRemoved:
                console.log("StatusBar, eventHandler receive WindowRemoved event");
                break;
            default:
                return;
        }
    }

    Component.onCompleted: {
        /* hmi-interface.js's API: each QML for HMI should register object id and event handler */
        Interface.registerComponent(statusBar, "StatusBar");
        Interface.registerEventHandler(statusBar.eventHandler);
    }

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

    Text {
        id: dateTime1
        anchors.left: parent.left
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
        source: "resources/wifi-symbol.png"
        anchors.left: dateTime2.right
        anchors.leftMargin: parent.width/70
        anchors.verticalCenter: parent.verticalCenter
        width: (height*sourceSize.width)/sourceSize.height
        height: statusBar.height * 0.3
        smooth: true
    }
    Image {
        id: rssi
        source: "resources/signal-symbol.png"
        anchors.left: wifi.right
        anchors.leftMargin: parent.width/70
        anchors.verticalCenter: parent.verticalCenter
        width: (height*sourceSize.width)/sourceSize.height
        height: statusBar.height * 0.3
        smooth: true
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
