# -*- coding: utf-8 -*-
from twisted.internet.protocol import Factory, Protocol, ProcessProtocol, connectionDone
from twisted.internet import reactor
from twisted.internet import defer
from twisted.internet.task import LoopingCall
import os
import sys
import time
from twisted.web import websockets
from iPodClient import run_ipodclient
import json
import uuid

# for libwebsockets echo client
websockets._encoders['echo'] = lambda(x): x
websockets._decoders['echo'] = lambda(x): x
websockets._encoders['http-only'] = lambda(x): x
websockets._decoders['http-only'] = lambda(x): x
websockets._encoders['steering-wheel'] = lambda(x): x
websockets._decoders['steering-wheel'] = lambda(x): x
websockets._encoders['AMB'] = lambda(x): x
websockets._decoders['AMB'] = lambda(x): x
websockets._encoders['ipod'] = lambda(x): x
websockets._decoders['ipod'] = lambda(x): x



ipod_subscribers = []
def sendIPodEvent(data):
    for transport in ipod_subscribers:
        transport.write(json.dumps(data))


steering_subscribers = []
def sendSteeringWheelButtonEvent(button, pressed):
    for transport in steering_subscribers:
        data = {'type': 'button', 'button': button, 'pressed': pressed}
        transport.write(json.dumps(data))
def sendSteeringWheelAxisEvent(axis, value):
    for transport in steering_subscribers:
        data = {'type': 'axis', 'axis': axis, 'value': value}
        transport.write(json.dumps(data))


tempTimer = None
def slotChangeTemp():
    print '[41mtimerout[0m'
    print DSGWProtocol.hvacInfo['HvacTemp']
    global tempTimer
    timeout = 3
    if DSGWProtocol.hvacInfo['HvacFanSpeed'] > 5:
        timeout = 1
    elif DSGWProtocol.hvacInfo['HvacFanSpeed'] > 2:
        timeout = 2
    if DSGWProtocol.hvacInfo['HvacAirConditioning']:
        if DSGWProtocol.hvacInfo['HvacTemp'] > 16:
            DSGWProtocol.hvacInfo['HvacTemp'] -= 1
            DSGWProtocol.sendBroadcast('HvacTemp', DSGWProtocol.hvacInfo['HvacTemp'])
            tempTimer = reactor.callLater(timeout, slotChangeTemp)
        else:
            tempTimer = None
    else:
        if DSGWProtocol.hvacInfo['HvacTemp'] < 35:
            DSGWProtocol.hvacInfo['HvacTemp'] += 1
            DSGWProtocol.sendBroadcast('HvacTemp', DSGWProtocol.hvacInfo['HvacTemp'])
            tempTimer = reactor.callLater(timeout, slotChangeTemp)
        else:
            tempTimer = None

class DSGWProtocol(Protocol):
    def callHandler(self, prefix, args=(), kwds={}):
        name = prefix
        if self.codec:
            name += 'For'+self.codec.replace('-','_').capitalize()
        if not hasattr(self, name):
            raise "No such WebSocket protocol handler for '%s'" % self.codec
        handler = getattr(self, name)
        return apply(handler, args, kwds)
        
    def makeConnection(self, transport):
        self.callHandler('handleMakeConnection', (transport,))

    def connectionLost(self, reason=connectionDone):
        self.callHandler('handleConnectionLost', (), {'reason': reason})

    def dataReceived(self, data):
        self.callHandler('handleDataReceived', (data,))

    # start broadcast protocol ================================================
    transports = []
    def sendAll(self, msg):
        for transport in DSGWProtocol.transports:
            transport.write(msg)
    def send(self, msg):
        for transport in DSGWProtocol.transports:
            if self.transport == transport:
                continue;
            transport.write(msg)

    def handleMakeConnection(self, transport):
        Protocol.makeConnection(self, transport)
        #self.sendAll('{"log": "connected peer"}')
        DSGWProtocol.transports.append(transport)

    def handleConnectionLost(self, reason=connectionDone):
        if self.transport in DSGWProtocol.transports:
            if self.transport in DSGWProtocol.transports:
                del DSGWProtocol.transports[DSGWProtocol.transports.index(self.transport)]
        Protocol.connectionLost(self, reason)
        #self.sendAll('{"log": "closed peer"}')

    def handleDataReceived(self, data):
        self.send(data)
        self.sendAMB(data)
    # end broadcast protocol ==================================================

    # start echo protocol ===============================================
    def handleMakeConnectionForEcho(self, transport):
        print 'echo connect', transport
        Protocol.makeConnection(self, transport)

    def handleConnectionLostForEcho(self, reason=connectionDone):
        print 'echo lost connection'
        Protocol.connectionLost(self, reason)

    def handleDataReceivedForEcho(self, data):
        print 'echo received', data
        self.transport.write(data)
    # end lws mirror protocol =================================================

    # DS to AMB ===============================================================
    def handleMakeConnectionForHttp_only(self, transport):
        return self.handleMakeConnection(transport)

    def handleConnectionLostForHttp_only(self, reason=connectionDone):
        return self.handleConnectionLost(reason)

    def handleDataReceivedForHttp_only(self, data):
        req = json.loads(data)
        if req['type'] == 'method' and req['name'] == 'getSupportedEventTypes':
            res = {'type': "methodReply",
                    'name': 'getSupportedEventTypes',
                    'transactionid': req['transactionid'],
                    'data': ['Latitude', 'Longitude', 'VehicleSpeed']}
            self.transport.write(json.dumps(res))
            return;

        return self.handleDataReceived(data)
    # DS to AMB ===============================================================

    # start steering ==========================================================
    def handleMakeConnectionForSteering_wheel(self, transport):
        Protocol.makeConnection(self, transport)
        steering_subscribers.append(transport)

    def handleConnectionLostForSteering_wheel(self, reason=connectionDone):
        if self.transport in steering_subscribers:
            del steering_subscribers[steering_subscribers.index(self.transport)]
        Protocol.connectionLost(self, reason)

    def handleDataReceivedForSteering_wheel(self, data):
        print data
    # end steering ==========================================================

    # Alternative AMB  ======================================================
    ambTransports = {}
    turnSignal = 'Off'
    hvacInfo = {"HvacAutoManual": True,
                "HvacFanSpeed": 0,
                "HvacAirConditioning": True,
                "HvacTemp": 24}
    engineRPM = 2;
    def handleMakeConnectionForAmb(self, transport):
        Protocol.makeConnection(self, transport)
        DSGWProtocol.ambTransports[transport] = []

    def handleConnectionLostForAmb(self, reason=connectionDone):
        if self.transport in DSGWProtocol.ambTransports.keys():
            del DSGWProtocol.ambTransports[self.transport]
        Protocol.connectionLost(self, reason)

    def sendAMB(self, msg):
        for transport in DSGWProtocol.ambTransports.keys():
            if self.transport == transport:
                continue;
            # XXX check and ignore
            data = json.loads(msg)
            if unicode(data['name']) in DSGWProtocol.ambTransports[transport]:
                transport.write(msg)

    def broadcastAMB(self, prop, val):
        self.sendAMB(json.dumps({"type":"valuechanged","name": prop,"data":{"value": val, "timestamp": time.time(),"sequence":"0"},"transactionid": str(uuid.uuid1())}));

    @staticmethod
    def sendBroadcast(prop, val):
        msg = json.dumps({"type":"valuechanged","name": prop,"data":{"value": val, "timestamp": time.time(),"sequence":"0"},"transactionid": str(uuid.uuid1())});
        for transport in DSGWProtocol.ambTransports.keys():
            data = json.loads(msg)
            if unicode(data['name']) in DSGWProtocol.ambTransports[transport]:
                transport.write(msg)


    def handleDataReceivedForAmb(self, data):
        print '[41mecho received[0m', data
        req = json.loads(data)
        if req['type'] == 'method':
            if req['name'] == 'subscribe':
                """{"type":"method","name":"subscribe","transactionid":"95158e31-0228-4211-7a80-adacc53758e7","data":["VehicleSpeed","Latitude","Longitude"]}"""
                for info in req['data']:
                    rep = """{"type":"methodReply","name":"subscribe","data":["%s"],"transactionid":"%s"}""" % (info, req['transactionid'])
                    DSGWProtocol.ambTransports[self.transport].append(info)
                    self.transport.write(json.dumps(data))
                '''
:onmessage: {"type":"valuechanged","name":"Latitude","data":{ "value":"41.4037","timestamp":"1366458596533.53","sequence":"0"},"transactionid":"3fd7d981-2ea3-317f-c6ea-03688483ab7f"}
D/ConsoleMessage( 1401): file:///root/WRNavigation/gsocket.js:32:onmessage: {"type":"valuechanged","name":"Longitude","data":{ "value":"2.1739","timestamp":"1366458596533.53","sequence":"1"},"transactionid":"3fd7d981-2ea3-317f-c6ea-03688483ab7f"}
D/ConsoleMessage( 1401): file:///root/WRNavigation/gsocket.js:32:onmessage: {"type":"valuechanged","name":"VehicleSpeed","data":{ "value":"8","timestamp":"1366458596533.53","sequence":"2"},"transactionid":"3fd7d981-2ea3-317f-c6ea-03688483ab7f"}
                '''
            elif req['name'] == 'get':
                """{"type":"method","name":"get","transactionid":"ef8319cc-d73d-9ce9-a9d4-3be78f8ad3b8","data":["HvacAutoManual","HvacFanSpeed","HvacAirConditioning"]}"""
                dictresult = []
                for info in req['data']:
                    if DSGWProtocol.hvacInfo.has_key(info):
                        dictresult.append({"name": info, "value": DSGWProtocol.hvacInfo[info]})
                    elif info == 'TurnSignal':
                        dictresult.append({"name": info, "value": DSGWProtocol.turnSignal})

                rep = {"type":"methodReply","name":"get","data": dictresult, "transactionid": req['transactionid']}
                self.transport.write(json.dumps(rep))
                print '=> '+json.dumps(rep)
            elif req['name'] == 'set':
                """{"type":"method","name":"set","transactionid":"d3506d7e-cd26-4f72-cea9-1f7f404154a7","data":[{"property":"HvacFanSpeed","value":1}]}"""
                dictresult = "successfully set:"
                for info in req['data']:
                    succeed = False
                    if DSGWProtocol.hvacInfo.has_key(info['property']):
                        DSGWProtocol.hvacInfo[info['property']] = info['value']
                        dictresult += " "+info['property']
                        succeed = True
                        if info['property']=='HvacAirConditioning':
                            global tempTimer
                            if not tempTimer:
                                tempTimer = reactor.callLater(1.5, slotChangeTemp)
                    elif info['property'] == 'TurnSignal':
                        if info['value'] in ['Left', 'Right', 'Off']:
                            DSGWProtocol.turnSignal = info['value']
                            succeed = True

                    if succeed:
                        self.broadcastAMB(info['property'], info['value'])

                rep = {"type":"methodReply","name":"set","data": dictresult, "error": "failed to set:", "transactionid": req['transactionid']}
                self.transport.write(json.dumps(rep))
                print '=> '+json.dumps(rep)


    # end Alternative AMB ==================================================


    # start iPod protocol ===============================================
    def handleMakeConnectionForIpod(self, transport):
        print 'ipod connect', transport
        Protocol.makeConnection(self, transport)
        ret = {'connection': True}
        self.controller = run_ipodclient(sendIPodEvent)
        if not self.controller:
            ret = {'connection': False,
                    'error': 'could not connect to ipod-daemon-2'}
        self.transport.write(json.dumps(ret))
        ipod_subscribers.append(transport)


    def handleConnectionLostForIpod(self, reason=connectionDone):
        print 'ipod lost connection'
        if self.transport in ipod_subscribers:
            del ipod_subscribers[ipod_subscribers.index(self.transport)]
        Protocol.connectionLost(self, reason)

    def handleDataReceivedForIpod(self, data):
        print 'ipod received', data
        #self.transport.write(data)
        try:
            call = json.loads(data)
        except:
            return
        func = call.get('function', '')
        args = call.get('args', ())
        self.controller.command(func, args)

    # end iPod protocol =================================================


class WSFactory(Factory):
    protocol = DSGWProtocol

def monitor_ipod():
    run_ipodclient(sendIPodEvent)

# vim: sw=4 ts=8 sts=4 et bs=2 fdm=marker fileencoding=utf8
