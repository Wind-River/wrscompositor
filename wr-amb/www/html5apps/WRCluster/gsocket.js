// gsocket.js

var gSocket;
var gSocketConnected = false;

function webSocketConnect(soc) {
	var gsoc = soc;

	if ("WebSocket" in window) {
		if(gsoc.socketProtocol.length > 0) {
			gSocket = new WebSocket(gsoc.socketUrl, gsoc.socketProtocol);
		} else {
			gSocket = new WebSocket(gsoc.socketUrl);
		}

		gSocket.onopen = function() {
			console.log("onopen");
			gSocketConnected = true;
			gsoc.successCB();
		};
		gSocket.onclose = function() {
			console.log("onclose");
			gSocketConnected = false;
			setTimeout(function(){
				webSocketConnect(soc);
			}, 5000);
		};
		gSocket.onerror = function(e) {
			console.log("onerror");
		};
		gSocket.onmessage = function (e) {
			gsoc.receive(e.data);
		};
	}
}

function GSocket(sCB, url, protocol) {
	// Default socket value
	this.socketUrl = "ws://localhost:9090";
	this.socketProtocol = "http-only";

	this.successCB = sCB;

	// variables for call management, supports up to 100 simultaneously
	this.methodIdx = 0;
	this.methodCalls = [];
	for(var i = 0; i < 100; i++)
	{
		this.methodCalls[i] = null;
	}

	this.timeouttime = 5000;

	if (url !== undefined) this.socketUrl = url;
	if (protocol !== undefined) this.socketProtocol = protocol;

	this.GMethodCall = function(id, name, sCB, eCB) {
		var me = this;
		this.sendSuccessCB = sCB;
		this.sendErrorCB = eCB;
		this.transactionid = id;
		this.name = name;
		this.done = false;
		this.start = function() {
			me.timeout = setTimeout(function() {
				me.finish();
			}, this.timeouttime);
		}
		this.finish = function() {
			if(me.timeout !== undefined) {
				clearTimeout(me.timeout);
			}
			me.done = true;
		}
	}
}

GSocket.prototype.generateTransactionId = function() {
	var i, val = [];
	for(i = 0; i < 8; i++)
	{
		var num = Math.floor((Math.random()+1)*65536);
		val[i] = num.toString(16).substring(1);
	}
	var uuid = val[0]+val[1]+"-"+
		val[2]+"-"+val[3]+"-"+val[4]+"-"+
		val[5]+val[6]+val[7];
	return uuid;
}

GSocket.prototype.sendEvent = function(name, value) {
	var evt = document.createEvent("Event");
	evt.initEvent(name, true, true);
	evt.name = name;
	evt.value = value;
	document.dispatchEvent(evt);
}

GSocket.prototype.send = function(obj, sCB, eCB) {
	if(!gSocketConnected) {
		console.log("not connected socket");
		return;
	}

	var i = this.methodIdx;
	this.methodIdx = (this.methodIdx + 1)%100;
	this.methodCalls[i] = new this.GMethodCall(obj.transactionid, 
			obj.name, sCB, eCB);
	gSocket.send(JSON.stringify(obj));
	this.methodCalls[i].start();
}

GSocket.prototype.receive = function(msg) {
	var Gevent;
	try {
		Gevent = JSON.parse(msg);
	} catch(e) {
		return;
	}

	if((Gevent === undefined) || (Gevent.type === undefined) ||
			(Gevent.name === undefined)) {
		return;
	}
	else {
		if(Gevent.type === "methodReply") {
			var calls = this.methodCalls;
			for(var i = 0; i < calls.length; i++) {
				var call = calls[i];
				if(!call)
					return;
				if(call && (!call.done) &&
						(call.transactionid === Gevent.transactionid)) {
					call.finish();
                }
					if(Gevent.error !== undefined) {
						call.sendErrorCB(Gevent.error);
					}
					if(Gevent.data !== undefined && call.sendSuccessCB !== undefined) {
						call.sendSuccessCB(Gevent.data);
					}
			}
		}
		else if(Gevent.type === "valuechanged") {
			this.sendEvent(Gevent.name, Gevent.data);
		}
	}
}

GSocket.prototype.subscribe = function(eventlist, sCB, eCB) {
	var obj = {
		"type" : "method",
		"name": "subscribe",
		"transactionid" : this.generateTransactionId(),
		"data" : eventlist
	};

	this.send(obj, sCB, eCB);
}

GSocket.prototype.unsubscribe = function(eventlist, sCB, eCB)
{
	var obj = {
		"type" : "method",
		"name": "unsubscribe",
		"transactionid" : this.generateTransactionId(),
		"data" : eventlist
	};

	this.send(obj, sCB, eCB);
}

GSocket.prototype.changeValueProperty = function(namelist, valuelist, sCB, eCB)
{
	if((namelist.length != valuelist.length)||(namelist.length <= 0))
	{
		return;
	}

	var obj = {
		"type" : "method",
		"name": "set",
		"transactionid" : this.generateTransactionId(),
		"data" : []
	};
	var list = [];
	for(var i = 0; i < namelist.length; i++)
	{
		var val = {"property" : namelist[i], "value" : valuelist[i]};
		list[list.length] = val;
	}
	obj.data = list;
	this.send(obj, sCB, eCB);
}

GSocket.prototype.getValueProperty = function(namelist, sCB, eCB)
{
	if(namelist.length <= 0)
	{
		return;
	}

	var obj = {
		"type" : "method",
		"name": "get",
		"transactionid" : this.generateTransactionId(),
		"data" : namelist
	};
	this.send(obj, sCB, eCB);
}
