/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */

var windowList = new Array();
var indexes = null;

function addWindow(window)
{
    windowList.push(window);
}

function relayoutForMultiWindow(areaWidth, areaHeight)
{
    var i;
    if(windowList.length > 2) {
        var posX = 0;
        var newWidth = areaWidth/Math.floor((windowList.length+1)/2);
        for (i = 0; i < windowList.length; i++) {
            windowList[i].width = newWidth;
            windowList[i].scaledWidth = windowList[i].width/windowList[i].targetWidth;
            windowList[i].targetX = posX;
            if(i%2 == 0) {
				windowList[i].targetY = 0;
            } else {
				windowList[i].targetY = areaHeight/2;
				posX += newWidth;
            }
			if(windowList[i] == windowList[windowList.length-1]) {
				if(i%2 == 1)
					windowList[i].height = areaHeight/2;
				else
					windowList[i].height = areaHeight;
			} else
				windowList[i].height = areaHeight/2;

			windowList[i].scaledHeight = windowList[i].height/windowList[i].targetHeight;
        }
    } else if (windowList.length == 2) {
		windowList[0].targetX = 0;
		windowList[1].targetX = areaWidth/2;
        for (i = 0; i < windowList.length; i++) {
			windowList[i].targetY = 0;
			windowList[i].width = areaWidth/2;
			windowList[i].height = areaHeight;
            windowList[i].scaledWidth = windowList[i].width/windowList[i].targetWidth;
            windowList[i].scaledHeight = areaHeight/windowList[i].targetHeight;
		}
	} else if (windowList.length == 1) {
		windowList[0].targetX = 0;
		windowList[0].targetY = 0;
		windowList[0].width = areaWidth
		windowList[0].height = areaHeight;
		windowList[0].scaledWidth = areaWidth/windowList[0].targetWidth;
		windowList[0].scaledHeight = areaHeight/windowList[0].targetHeight;
	}
}

function addMultiWindow(window, areaWidth, areaHeight)
{
	addWindow(window);
	relayoutForMultiWindow(areaWidth, areaHeight);
}

function hideWithout(window) {
    var i;
    for (i = 0; i < windowList.length; i++) {
        if (windowList[i] != window)
            windowList[i].hide();
    }
}

function removeWindow(window)
{
    var i;
    for (i = 0; i < windowList.length; i++) {
        if (windowList[i] == window)
            break;
    }

    windowList.splice(i, 1);
}

function findBySurface(surface)
{
    var i;
    //console.log("findBySurface "+surface);
    for (i = 0; i < windowList.length; i++) {
        //console.log(i+". "+windowList[i]);
        if (windowList[i].surface == surface)
            return windowList[i];
    }
    //console.log("couldn't find windowFrmae for this surface");
    return null;
}

function findByProcessId(pid)
{
    var i;
    for (i = 0; i < windowList.length; i++) {
        if (windowList[i].processId == pid)
            return windowList[i];
    }
    return null;
}

function resizedCurrentWindow(window, resizedWidth, resizedHeight) 
{
    window.targetX = 0;
    window.targetY = 0;
    window.width = resizedWidth;
    window.height = resizedHeight;
    window.scaledWidth = resizedWidth/window.targetWidth;
    window.scaledHeight = resizedHeight/window.targetHeight;
}

function switchNextWindow(currentWindow)
{
    var nextWindow = null;
    var i;

    if (windowList.length == 1) {
        console.log("only one window existed in compositor. Nothing to do");
        return null;
    }

    for (i = 0; i < windowList.length; i++) {
        if (windowList[i] == currentWindow) {
            currentWindow.hide();
            console.log("swiped current window. try to switch next window");
            if (i + 1 == windowList.length) {
                console.log("find first window in windowList");
                nextWindow = windowList[0];
            } else {
                console.log("find next-window in windowList");
                nextWindow = windowList[i+1];
            }
            nextWindow.show();
            break;
        }
    }
    return nextWindow;
}