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

.pragma library

var Event = { "WindowAdded" : 0,
              "WindowRemoved" : 1
            }; // From wrscompositor.js to hmi componets

var Request = { "HideWindow" : 0,
                "ShowWindow" : 1,
                "LaunchNative": 2,
                "HideLauncherWindow" : 3,
                "ShowLauncherWindow" : 4,
              }; // From hmi components to wrscompoisitor.js


var wrsCompositor = null;

var object = function(id, name, handler) {
    this.id = id;
    this.name = name;
}

var objectList = new Array();

function registerObjectItem(id, name)
{
    var newObject = new object(id, name);
    objectList.push(newObject);
}

function registerEventHandler(handler)
{
    wrsCompositor.sendEvent.connect(handler);
}

function findObjectByName(name) {
    for (var index = 0; index < objectList.length; index++) {
        var object = objectList[index];
        if (object.name == name) {
            return object.id;
        }
    }
    return null;
}

function launchNative(name) {
    wrsCompositor.sendRequest(Request.LaunchNative, name);
}

function hideLauncherWindow() {
    wrsCompositor.sendRequest(Request.HideLauncherWindow, null);
}

function showLauncherWindow() {
    wrsCompositor.sendRequest(Request.ShowLauncherWindow, null);
}

function hideWindow(window) {
    wrsCompositor.sendRequest(Request.HideWindow, window);
}

function showWindow(window) {
    wrsCompositor.sendRequest(Request.ShowWindow, window);
}