/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
import QtQuick 2.5
import QtMultimedia 5.0
import com.windriver.automotive 1.0
import QtWebKit 3.0;
import "config.js" as Conf

WebView {
    id: webview
    url: "html5navi/index.html"
    anchors.fill: parent
}
/*
import QtLocation 5.3

Map {
    id: map
    anchors.fill: parent
    zoomLevel: (maximumZoomLevel - minimumZoomLevel)/2
    center {
        // The Qt Company in Oslo
        latitude: 59.9485
        longitude: 10.7686
    }
}
*/
