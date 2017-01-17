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

var COMPOSITOR_EVENT = { "ADD_WINDOW" : 0,
                         "REMOVE_WINDOW" : 1
                       };


var HMI_EVENT = { "HIDE_WINDOW" : 0,
                  "SHOW_WINDOW" : 1,
                  "RAISE_WINDOW" : 2,
                  "LAUNCH_NATIVE": 3,
                  "LAUNCHED_NATIVE": 4,
                  "HIDE_LAUNCHER_WINDOW" : 5,
                  "SHOW_LAUNCHER_WINDOW" : 6,
                  "RESIZE_DEFAULT_WINDOW" : 7,
                };


var wrsCompositor = null;

var component = function(id, name) {
    this.id = id;
    this.name = name;
}

var componentList = new Array();

function registerComponent(id, name)
{
    var obj = new component(id, name);
    componentList.push(obj);
}

function registerNotifyEventHandler(handler)
{
    wrsCompositor.notifyEvent.connect(handler);
}

function findComponentByName(name) {
    for (var index = 0; index < componentList.length; index++) {
        var component = componentList[index];
        if (component.name == name) {
            return component.id;
        }
    }
    return null;
}

function launchNative(name) {
    wrsCompositor.sendEvent(HMI_EVENT.LAUNCH_NATIVE, name);
}

function launchedNative(launcher) {
    wrsCompositor.sendEvent(HMI_EVENT.LAUNCHED_NATIVE, launcher);
}

function hideLauncherWindow() {
    wrsCompositor.sendEvent(HMI_EVENT.HIDE_LAUNCHER_WINDOW, null);
}

function showLauncherWindow() {
    wrsCompositor.sendEvent(HMI_EVENT.SHOW_LAUNCHER_WINDOW, null);
}

function hideWindow(window) {
    wrsCompositor.sendEvent(HMI_EVENT.HIDE_WINDOW, window);
}

function showWindow(window) {
    wrsCompositor.sendEvent(HMI_EVENT.SHOW_WINDOW, window);
}

function resizeDefaultWindow(fullsize) {
    wrsCompositor.sendEvent(HMI_EVENT.RESIZE_DEFAULT_WINDOW, fullsize);
}

function raiseWindow(window) {
    wrsCompositor.sendEvent(HMI_EVENT.RAISE_WINDOW, window);
}