#!/usr/bin/env python
# -*- coding: utf-8 -*-
from twisted.internet import reactor, defer
from twisted.internet.abstract import FileDescriptor
from twisted.python.filepath import FilePath
import sys
import time
import base64
import struct
import png
from StringIO import StringIO
from ipod.wrs_ipod_2 import *

from txdbus import client, objects
from txdbus.interface import DBusInterface, Method, Signal, Property
from txdbus.objects import dbusMethod, DBusProperty

connection = None

cmds = []
def deferred_call(scenario, func, *args):
    cmds.insert(0, {'func': func, 'args': args, 'tid': None, 'scenario': scenario})
    if len(cmds) == 1:
        callinfo = cmds[-1]
        callinfo['tid'] = apply(callinfo['func'], callinfo['args'])

def rgb565topng(bin, w, h, rowstride):
    RGB565_MASK_RED       = 0xF800
    RGB565_MASK_GREEN     = 0x07E0
    RGB565_MASK_BLUE      = 0x001F

    pixels = []
    offset = 0
    for j in range(h):
            row = bin[offset:offset+rowstride]
            rows = []
            for i in range(w):
                    pixel = struct.unpack('H', row[i*2:(i+1)*2])[0]
                    r = ((pixel & RGB565_MASK_RED) >> 11) << 3
                    g = ((pixel & RGB565_MASK_GREEN) >> 5) << 2
                    b = ((pixel & RGB565_MASK_BLUE)) << 3
                    rows += [r, g, b]
            pixels.append(rows)
            offset += rowstride

    #pixels = [(0,255,0)*128]*128
    f = StringIO()
    w = png.from_array(pixels, mode='RGB')
    w.save(f)

    # binary PNG data
    return f.getvalue()


def event_cb(c, _ev, ud):
    ev = wrs_ipod_event_type(_ev)
    broadcast = ud
    if WRSIPOD_EVENT_CONNECTED == ev:
        print 'connected'
        broadcast({'event': 'ipod connected'})
    elif WRSIPOD_EVENT_AUTHENTICATED == ev:
        print 'authenticated'
    elif WRSIPOD_EVENT_DEVICE_READY == ev:
        print 'device ready'
        if wrs_ipod_device_count(c) > 0:
            wrs_ipod_open_session_sync(c, 0)
            broadcast({'event': 'ipod ready'})
            deferred_call(None, wrs_ipod_play, c)
    elif WRSIPOD_EVENT_MODE_CHANGED == ev:
        print 'mode changed'
    elif WRSIPOD_EVENT_SAMPLERATE_RETRIEVED == ev:
        print 'samplerate retrieved'
    elif WRSIPOD_EVENT_DISCONNECTED == ev:
        print 'disconnected'
        broadcast({'event': 'ipod disconnected'})
    elif WRSIPOD_EVENT_PLAYSTATE_CHANGED == ev:
        print 'play state changed'
        broadcast({'event': 'playstate changed', 'data': wrs_ipod_current_track_state(c)})
    elif WRSIPOD_EVENT_TRACK_CHANGED == ev:
        print 'track changed', wrs_ipod_current_track_index(c)
        broadcast({'event': 'track changed', 'data': wrs_ipod_current_track_index(c)})
        deferred_call('get trackinfo', wrs_ipod_current_track_info, c)
    elif WRSIPOD_EVENT_TRACKINFO_RETRIEVED == ev:
        print 'track info retrieved'
    elif WRSIPOD_EVENT_SHUFFLE_CHANGED == ev:
        print 'changed shuffle status', wrs_ipod_current_shuffle_state(c)
        broadcast({'event': 'shuffle changed', 'data': wrs_ipod_current_shuffle_state(c)})
    elif WRSIPOD_EVENT_REPEAT_CHANGED == ev:
        print 'changed repeat status', wrs_ipod_current_repeat_state(c)
        broadcast({'event': 'repeat changed', 'data': wrs_ipod_current_repeat_state(c)})
    elif WRSIPOD_EVENT_TRACK_POSITION_CHANGED == ev:
        print 'track_position_changed', wrs_ipod_current_track_position(c)
        broadcast({'event': 'track position changed', 'data': wrs_ipod_current_track_position(c)})
        #print 'has artwork: ',wrs_ipod_current_track_has_artwork(c)
    else:
        print 'event no ', ev

def reply_cb(c, tid, retval, error, ud):
    print 'reply_cb', c, tid, retval, error, ud
    broadcast = ud
    called = cmds.pop()
    assert called['tid'] == tid
    scenario = called['scenario']
    if scenario:
        if 'get trackinfo' == scenario:
            print 'title', wrs_ipod_current_track_title(c)
            print 'artist', wrs_ipod_current_track_artist(c)
            print 'album', wrs_ipod_current_track_album(c)
            print 'chapter', wrs_ipod_current_track_chapter(c)
            broadcast({'event': 'track info', 'data':
                        {'title': wrs_ipod_current_track_title(c),
                        #'uid': wrs_ipod_current_track_uid(c),
                        'artist': wrs_ipod_current_track_artist(c),
                        'album': wrs_ipod_current_track_album(c),
                        'chapter': wrs_ipod_current_track_chapter(c),
                        'playstate': wrs_ipod_current_track_state(c),
                        'has_artwork':wrs_ipod_current_track_has_artwork(c),
                        'has_chapters':wrs_ipod_current_track_has_chapters(c),
                        'shuffle_state':wrs_ipod_current_shuffle_state(c),
                        'repeat_state':wrs_ipod_current_repeat_state(c),
                        'number_of_tracks':wrs_ipod_current_number_of_tracks(c),
                        'track_length':wrs_ipod_current_track_length(c),
                        'track_position':wrs_ipod_current_track_position(c),
                        'track_timestamp':wrs_ipod_current_track_timestamp(c),
                        'track_timestamp':wrs_ipod_current_track_timestamp(c),
                        'track_index': wrs_ipod_current_track_index(c), }})
        elif 'get artwork' == scenario:
            bin = wrs_ipod_current_track_artwork_pydata(c)
            if bin:
                w = wrs_ipod_current_track_artwork_width(c)
                h = wrs_ipod_current_track_artwork_height(c)
                rowstride = wrs_ipod_current_track_artwork_rowstride(c)
                png = rgb565topng(bin, w, h, rowstride)
                broadcast({'event': 'current artwork', 'data':
                            {'image': base64.encodestring(png).replace('\n',''),
                            'width': w, 'height': h, 'rowstride': rowstride}})
        else:
            print scenario

    if cmds:
        next_call = cmds[-1]
        next_call['tid'] = apply(next_call['func'], next_call['args'])

class IPodClient(FileDescriptor):
    def __init__(self, c):
        FileDescriptor.__init__(self, reactor)
        global connection
        self.c = c
        connection = c
    def fileno(self):
        return wrs_ipod_fd(self.c)
    def doRead(self):
        wrs_ipod_read(self.c)
    def connectionLost(self, reason):
        global connection
        print 'ipod daemon closed'
        wrs_ipod_close(self.c)
        self.c = None
        connection = None

class IPodController:
    def __init__(self, c):
        self.c = c
    def command(self, func, args=()):
        if func == 'play':
            deferred_call(None, wrs_ipod_play, self.c)
        elif func == 'stop':
            deferred_call(None, wrs_ipod_stop, self.c)
        elif func == 'pause':
            deferred_call(None, wrs_ipod_pause, self.c)
        elif func == 'next':
            deferred_call(None, wrs_ipod_next_track, self.c)
        elif func == 'prev':
            deferred_call(None, wrs_ipod_prev_track, self.c)
        elif func == 'get_trackinfo':
            deferred_call('get trackinfo', wrs_ipod_current_track_info, self.c)
        elif func == 'get_artwork':
            deferred_call('get artwork', wrs_ipod_current_track_artwork, self.c, args[0])
        elif func == 'seek':
            deferred_call(None, wrs_ipod_set_position, self.c, args[0])

IMB = None
class IPodMessageBroker (objects.DBusObject):
    iface = DBusInterface( 'com.windriver.iAP1',
                           Method('play', arguments='', returns=''),
                           Method('pause', arguments='', returns=''),

                           Signal('track_position_changed', 'u'),
                           Signal('track_changed', 'u'),
                           Signal('track_info', 'iiuuuuussss'),
                           Signal('playstate_changed', 'u'),
                           Signal('shuffle_changed', 'i'),
                           Signal('repeat_changed', 'i'),
                           Signal('artwork_info', 'uus'),

                           Property('repeat_state', 'i', writeable=False),
                           Property('shuffle_state', 'i', writeable=False),
                           Property('number_of_tracks', 'u', writeable=False),
                           Property('track_timestamp', 'u', writeable=False),
                           Property('track_length', 'u', writeable=False),
                           Property('playstate', 'u', writeable=False),
                           Property('has_artwork', 'u', writeable=False),
                           Property('title', 's', writeable=False, emitsOnChange=True),
                           Property('chapter', 's', writeable=False),
                           Property('artist', 's', writeable=False),
                           Property('album', 's', writeable=False),

                           Property('artwork', 'ay', writeable=False),
                         )

    dbusInterfaces = [iface]
    chapter = DBusProperty('chapter'),
    repeat_state = DBusProperty('repeat_state'),
    shuffle_state = DBusProperty('shuffle_state'),
    number_of_tracks = DBusProperty('number_of_tracks'),
    track_timestamp = DBusProperty('track_timestamp'),
    track_length = DBusProperty('track_length'),
    playstate = DBusProperty('playstate'),
    has_artwork = DBusProperty('has_artwork'),
    title = DBusProperty('title'),
    artist = DBusProperty('artist'),
    album = DBusProperty('album'),
    artwork = DBusProperty('artwork'),

    def __init__(self, objectPath):
        objects.DBusObject.__init__(self, objectPath)

    def dbus_play(self):
        global connection
        print 'play', connection
        if connection:
            deferred_call(None, wrs_ipod_play, connection)

    def dbus_pause(self):
        global connection
        print 'pause', connection
        if connection:
            deferred_call(None, wrs_ipod_pause, connection)

    def sendEvent(self, obj):
        event = obj['event']
        data = obj.get('data', None)
        if event == 'track info':
            self.chapter = data['chapter']
            self.repeat_state = data['repeat_state']
            self.shuffle_state = data['shuffle_state']
            self.number_of_tracks = data['number_of_tracks']
            self.track_timestamp = data['track_timestamp']
            self.track_length = data['track_length']
            self.playstate = data['playstate']
            self.has_artwork = data['has_artwork']
            self.title = data['title']
            self.artist = data['artist']
            self.album = data['album']

            self.emitSignal('track_info', data['repeat_state'], data['shuffle_state'], data['number_of_tracks'], data['track_timestamp'], data['track_length'], data['playstate'], data['has_artwork'], data['title'] and data['title'] or "", data['chapter'] and data['chapter'] or "", data['artist'] and data['artist'] or "", data['album'] and data['album'] or "")
        elif event == 'current artwork':
            #self.artwork = base64.decodestring(data['image'])
            self.emitSignal('artwork_info', data['width'], data['height'], data['image'])
        else:
            signal = event.replace(' ', '_')
            self.emitSignal(signal, data)

@defer.inlineCallbacks
def dbusMain():
    global IMB
    try:
        conn = yield client.connect(reactor)
        IMB = IPodMessageBroker('/iPod')
        conn.exportObject( IMB )
        yield conn.requestBusName('com.windriver.automotive')
    except error.DBusException, e:
        print 'Failed to export object: ', e

def run_ipodclient(broadcastCallback):
    global connection
    if connection:
        return IPodController(connection)
    c = wrs_ipod_connect(None)
    if not c:
        print 'Could not connect to ipod-daemon-2'
        return None

    reactor.callLater(0, dbusMain)

    def dbus_broadcast(data):
        global IMB
        broadcastCallback(data)
        if IMB:
            IMB.sendEvent(data)

    wrs_ipod_set_event_pycb(c, event_cb, dbus_broadcast);
    wrs_ipod_set_reply_pycb(c, reply_cb, dbus_broadcast);
    if wrs_ipod_device_count(c) > 0:
        wrs_ipod_open_session_sync(c, 0)
        deferred_call(None, wrs_ipod_play, c)

    reactor.addReader(IPodClient(c))
    return IPodController(c)

# vim: sw=4 ts=8 sts=4 et bs=2 fdm=marker fileencoding=utf8
