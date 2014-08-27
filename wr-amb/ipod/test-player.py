#!/usr/bin/env python
# -*- coding: utf-8 -*-

from wrs_ipod_2 import *
import sys

cmds = []
def deferred_call(scenario, func, *args):
    cmds.insert(0, {'func': func, 'args': args, 'tid': None, 'scenario': scenario})
    if len(cmds) == 1:
        callinfo = cmds[-1]
        callinfo['tid'] = apply(callinfo['func'], callinfo['args'])

def event_cb(c, _ev, ud):
    ev = wrs_ipod_event_type(_ev)
    if WRSIPOD_EVENT_CONNECTED == ev:
        print 'connected'
    elif WRSIPOD_EVENT_AUTHENTICATED == ev:
        print 'authenticated'
    elif WRSIPOD_EVENT_DEVICE_READY == ev:
        print 'device ready'
        if wrs_ipod_device_count(c) > 0:
            wrs_ipod_open_session_sync(c, 0)
            deferred_call('we played it', wrs_ipod_play, c)
    elif WRSIPOD_EVENT_MODE_CHANGED == ev:
        print 'mode changed'
    elif WRSIPOD_EVENT_SAMPLERATE_RETRIEVED == ev:
        print 'samplerate retrieved'
    elif WRSIPOD_EVENT_DISCONNECTED == ev:
        print 'disconnected'
    elif WRSIPOD_EVENT_PLAYSTATE_CHANGED == ev:
        print 'play state changed'
    elif WRSIPOD_EVENT_TRACK_CHANGED == ev:
        print 'track changed', wrs_ipod_current_track_index(c)
    elif WRSIPOD_EVENT_SHUFFLE_CHANGED == ev:
        print 'changed shuffle status', wrs_ipod_current_shuffle_state(c)
    elif WRSIPOD_EVENT_REPEAT_CHANGED == ev:
        print 'changed repeat status', wrs_ipod_current_repeat_state(c)
    elif WRSIPOD_EVENT_TRACK_POSITION_CHANGED == ev:
        print 'track_position_changed', wrs_ipod_current_track_position(c)
        print 'has artwork: ',wrs_ipod_current_track_has_artwork(c)
    else:
        print 'event no ', ev

def reply_cb(c, tid, retval, error, ud):
    print 'reply_cb', c, tid, retval, error, ud
    called = cmds.pop()
    assert called['tid'] == tid
    if called['scenario']:
        print called['scenario']

    if cmds:
        next_call = cmds[-1]
        next_call['tid'] = apply(next_call['func'], next_call['args'])


c = wrs_ipod_connect(None)
if not c:
    print 'could not connect to ipod-daemon-2'
    sys.exit(0)

wrs_ipod_set_event_pycb(c, event_cb, None);
wrs_ipod_set_reply_pycb(c, reply_cb, None);



if wrs_ipod_device_count(c) > 0:
    wrs_ipod_open_session_sync(c, 0)
    deferred_call(None, wrs_ipod_play, c)

while True:
    wrs_ipod_read(c)

wrs_ipod_close(c)


# vim: sw=4 ts=8 sts=4 et bs=2 fdm=marker fileencoding=utf8
