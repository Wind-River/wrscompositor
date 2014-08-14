#!/usr/bin/env python
# -*- coding: utf-8 -*-
from twisted.internet import reactor
from WSProtocols import sendSteeringWheelButtonEvent, sendSteeringWheelAxisEvent
from twisted.internet.abstract import FileDescriptor
from twisted.internet import inotify
from twisted.python.filepath import FilePath
import sys
import time
#from Xlib import X, display, Xutil
#from Xlib.ext import xtest
import os, time
from evdev import UInput, ecodes as e
from WSProtocols import DSGWProtocol

from joystick import *
'''
keycode 123 = XF86AudioRaiseVolume
keycode 122 = XF86AudioLowerVolume
keycode 231 = Cancel
keycode 177 = XF86Send
keycode 147 = XF86Phone
keycode 166 = XF86Stop
keycode 225 = XF86Search
keycode 124 = XF86PowerOff
keycode 179 = XF86WebCam
keycode 220 = XF86Pictures
'''


try:
    ui = UInput()
except:
    while True:
        if os.path.exists('/dev/uinput'):
            break;
        time.sleep(0.2)
#display = display.Display(":0")
shutdown_pressed = 0
duduregi_pressed = 0
cluster_pressed = 0
lastKey = 0

ttsmode = -1 # 0: en, 1: ko, 2: ja, 3: zh

def focusToApp():
    return
    print 'focus to app'
    xtest.fake_input(display, X.MotionNotify,x=1,y=150)
    xtest.fake_input(display, X.ButtonPress,1)
    xtest.fake_input(display, X.ButtonRelease,1)
    display.sync()

class WheelHandler(FileDescriptor):
    def __init__(self, reactor=None):
        FileDescriptor.__init__(self, reactor)
        print 'found'

        j = Joystick(0)
        self.j = j
        @j.event
        def on_button(button, pressed):
            global display, shutdown_pressed, cluster_pressed, duduregi_pressed
            print 'button', button, pressed
            sendSteeringWheelButtonEvent(button, pressed)
            enterKey = 36
            homeKey = 166  # XF86Stop
            iKey = 31  # 'i' key
            cancelX = 1679
            cancelY = 1049
            if button in (8,):
                if pressed:
                    #xtest.fake_input(display, X.KeyPress, enterKey)
                    ui.write(e.EV_KEY, e.KEY_ENTER, 1)
                else:
                    #xtest.fake_input(display, X.KeyRelease, enterKey)
                    ui.write(e.EV_KEY, e.KEY_ENTER, 0)
                #display.sync()
                ui.syn()
            elif button in (9,):
                if pressed:
                    ui.write(e.EV_KEY, e.KEY_BACKSPACE, 1)
                    #xtest.fake_input(display, X.KeyPress, homeKey)
                    #xtest.fake_input(display, X.MotionNotify, x=cancelX,y=cancelY)
                    #xtest.fake_input(display, X.ButtonPress,1)
                else:
                    ui.write(e.EV_KEY, e.KEY_BACKSPACE, 0)
                    #xtest.fake_input(display, X.KeyRelease, homeKey)
                    #xtest.fake_input(display, X.ButtonRelease,1)
                #display.sync()
                ui.syn()
            elif button in (5,):
                if pressed:
                        shutdown_pressed = int(time.time())
                else:
                        print int(time.time()) , shutdown_pressed
                        print (int(time.time()) - shutdown_pressed)
                        if shutdown_pressed != 0 and \
                                (int(time.time()) - shutdown_pressed)>=5:
                                os.system('reboot')
            elif button in (7,):
                if pressed:
                    #xtest.fake_input(display, X.KeyPress, iKey)
                    ui.write(e.EV_KEY, e.KEY_I, 1)
                else:
                    #xtest.fake_input(display, X.KeyRelease, iKey)
                    ui.write(e.EV_KEY, e.KEY_I, 0)
                #display.sync()
                ui.syn()

                if not pressed:
                    global ttsmode
                    if ttsmode == -1: # skip
                        ttsmode += 1
                        return

                    import ifconfig
                    ipaddrs = ''
                    for intf in ifconfig.list_ifs():
                        if intf.ip:
                            if len(ipaddrs)>0:
                                ipaddrs += ' or '
                            ipaddrs += intf.ip
                    lang = ["en", "ko", "ja", "zh"][ttsmode % 4]
                    ttsmode += 1
                    text = "unknown"
                    if len(ipaddrs)>0:
                        text = "IP Address is "+ipaddrs.replace(".", " ")
                        if lang == "ko":
                            text = "IP 주소는 "+ipaddrs.replace(".", "쩜").replace('or','또는')
                            text += " 입니다."
                        elif lang == "ja":
                            text = "IPアドレスは"+ipaddrs.replace(".", " ").replace('or','または')
                            text += "です。"
                        elif lang == "zh":
                            text = "IP地址是"+ipaddrs.replace(".", " ").replace('or','或')
                    else:
                        if lang == "ko":
                            text = "IP 주소 몰라요."
                        elif lang == "ja":
                            text = "知りません。"
                        elif lang == "zh":
                            text = "我不知道的IP地址。"
                    os.system('killall gst-launch-1.0; gst-launch-1.0 playbin uri="http://translate.google.com/translate_tts?ie=UTF-8&q=%s&tl=%s" audio-sink=pulsesink role=music &' % (text, lang))
            elif button in (1,): # right turn signal
                if pressed:
                    print 'right turn signal'
                    DSGWProtocol.sendBroadcast('TurnSignal', 'Right')
                else:
                    print 'off turn signal'
                    DSGWProtocol.sendBroadcast('TurnSignal', 'Off')
            elif button in (0,): # left turn signal
                if pressed:
                    print 'left turn signal'
                    DSGWProtocol.sendBroadcast('TurnSignal', 'Left')
                else:
                    print 'off turn signal'
                    DSGWProtocol.sendBroadcast('TurnSignal', 'Off')
            elif button in (10,): # kill duduregi
                if pressed:
                        duduregi_pressed = int(time.time())
                else:
                        print int(time.time()) , duduregi_pressed
                        print (int(time.time()) - duduregi_pressed)
                        if duduregi_pressed != 0 and \
                                (int(time.time()) - duduregi_pressed)>=5:
                                os.system('killall duduregi')
            elif button in (6,): # quit
                if pressed:
                    ui.write(e.EV_KEY, e.KEY_F2, 1)
                else:
                    ui.write(e.EV_KEY, e.KEY_F2, 0)
                ui.syn()
            elif button in (4,): # Menu
                if pressed:
                    ui.write(e.EV_KEY, e.KEY_F1, 1)
                else:
                    ui.write(e.EV_KEY, e.KEY_F1, 0)
                ui.syn()
            elif button in (11,): # reload google earth map
                sendSteeringWheelButtonEvent(button, pressed)
            '''
            elif button in (4,): # reload google earth map
                if pressed:
                    cluster_pressed = int(time.time())
                else:
                    print int(time.time()) , cluster_pressed
                    print (int(time.time()) - cluster_pressed)
                    if cluster_pressed != 0 and \
                                (int(time.time()) - cluster_pressed)>=5:
                        os.system('xrandr -d :0 --output HDMI3 --left-of HDMI2')
                        os.system('killall cluster')
                        time.sleep(1)
                        os.system('launch_app com.windriver.cluster')
            '''

        @j.event
        def on_axis(axis, value):
            global display, lastKey
            print 'axis', axis, value
            if axis == 0:
                sendSteeringWheelAxisEvent(axis, value)
            elif axis == 4:
                if value != 0:
                    focusToApp()
                    if value > 0:
                        print 'right'
                        #lastKey = 114
                        #xtest.fake_input(display, X.KeyPress, lastKey)
                        lastKey = e.KEY_RIGHT
                        ui.write(e.EV_KEY, lastKey, 1)
                    else:
                        print 'left'
                        #lastKey = 113
                        #xtest.fake_input(display, X.KeyPress, lastKey)
                        lastKey = e.KEY_LEFT
                        ui.write(e.EV_KEY, lastKey, 1)
                else:
                    print 'release', lastKey
                    #if lastKey in (113, 114):
                    #    xtest.fake_input(display, X.KeyRelease, lastKey)
                    #    lastKey = 0
                    if lastKey in (e.KEY_LEFT, e.KEY_RIGHT):
                        ui.write(e.EV_KEY, lastKey, 0)
                        lastKey = 0
                #display.sync()
                ui.syn()
            elif axis == 5:
                if value != 0:
                    focusToApp()
                    if value > 0:
                        print 'down'
                        #lastKey = 116
                        #xtest.fake_input(display, X.KeyPress, lastKey)
                        lastKey = e.KEY_DOWN
                        ui.write(e.EV_KEY, lastKey, 1)
                    else:
                        print 'up'
                        #lastKey = 111
                        #xtest.fake_input(display, X.KeyPress, lastKey)
                        lastKey = e.KEY_UP
                        ui.write(e.EV_KEY, lastKey, 1)
                else:
                    print 'release', lastKey
                    #if lastKey in (116, 111):
                    #    xtest.fake_input(display, X.KeyRelease, lastKey)
                    #    lastKey = 0
                    if lastKey in (e.KEY_DOWN, e.KEY_UP):
                        ui.write(e.EV_KEY, lastKey, 0)
                        lastKey = 0
                #display.sync()
                ui.syn()
    def fileno(self):
        return self.j.dev.fileno()
    def doRead(self):
        self.j.dispatch_events()
    def connectionLost(self, reason):
        print 'joystick closed'

reader = None

def monitor_joystick(reactor):
    global reader
    if os.path.exists('/dev/input/js0'):
        reader = WheelHandler(reactor)
        reactor.addReader(reader)

    def cb_inotify(self, filepath, mask):
        global reader
        evts = inotify.humanReadableMask(mask)
        if len(evts)==0:
            return
        evt = evts[0]
        if filepath.basename()=="js0":
            if evt == "delete":
                if reader:
                    reactor.removeReader(reader)
                    reader = None
            elif evt == "create":
                if reader:
                    reactor.removeReader(reader)
                reader = WheelHandler(reactor)
                reactor.addReader(reader)

    notifier = inotify.INotify(reactor)
    notifier.startReading()
    notifier.watch(FilePath("/dev/input"), callbacks=[cb_inotify,])

# vim: sw=4 ts=8 sts=4 et bs=2 fdm=marker fileencoding=utf8
