#!/usr/bin/env python
# -*- coding: utf-8 -*-
from twisted.python import log
from twisted.internet import reactor, protocol
from twisted.internet.task import LoopingCall
from twisted.web import server, resource, static, twcgi
from twisted.web.websockets import WebSocketsResource
from WSProtocols import WSFactory
from WSProtocols import sendSteeringWheelButtonEvent, sendSteeringWheelAxisEvent
#from WSProtocols import monitor_ipod
from DrivingWheelHandler import monitor_joystick
from twisted.internet.abstract import FileDescriptor
import os, sys
import time
from subprocess import Popen, PIPE, STDOUT
from GeniviVNA import initGeniviVNA



def setCluster():
    p=Popen(['xrandr','-q','-d',':0'], stdout=PIPE, stderr=PIPE)
    p.wait()
    output = p.stdout.read()
    if output.find('HDMI2 connected')>=0:
        os.system('xrandr -d :0 --output HDMI3 --left-of HDMI2')
        os.system('killall cluster')
        time.sleep(1)
        os.system('launch_app com.windriver.cluster')



def main():
    root = static.File(os.path.realpath(os.path.dirname(__file__))+'/www')
    root.processors = { '.cgi': twcgi.CGIScript, }
    reactor.listenTCP(7070, server.Site(root))

    resource = WebSocketsResource(WSFactory())
    reactor.listenTCP(9090, server.Site(resource))


    #reactor.callLater(5, setCluster)

    monitor_joystick(reactor)
    #monitor_ipod()

    # VehicleNetworkAdapter
    #initGeniviVNA(reactor)

    reactor.run()

if __name__ == '__main__':
    log.startLogging(sys.stdout)
    main()

# vim: sw=4 ts=8 sts=4 et bs=2 fdm=marker fileencoding=utf8
