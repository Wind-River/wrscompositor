function NobdyStream(type) {
	this.type = type;
	var blah;
	this.propName = type.charAt(0).toLowerCase() + type.slice(1);

	this[this.propName] = "";
	this.val="";
}

NobdyStream.prototype.changed = function (value) { }
NobdyStream.prototype.type = undefined;
NobdyStream.prototype.propName = "";
NobdyStream.prototype.setValue = function(value) {
	this[this.propName] = value;
	this.changed(value);
}

var nobdy = new Nobdy();

function Nobdy() {
	this.websocket = undefined;
}

Nobdy.prototype.connect = function(address) {

	var that = this

    if(typeof MozWebSocket != "undefined")
		this.websocket = new MozWebSocket("ws://" + address);
	else this.websocket = new WebSocket("ws://" + address);

	this.websocket.onopen = function(evt) {

    }

	this.websocket.onclose = function(evt) {
		simulateData = true;
		that.setSupported(["Velocity","EngineRPM","EngineCoolantTemp","ThrottlePosition",
						   "IntakeAirTemp","EngineLoad","MassAirFlow"]);
		that.produceSimulatedData();
    }

	this.websocket.onmessage = function(evt) {
		var nobdy = that;
        eval(evt.data);
    }

	this.websocket.onerror = function(evt) {

    }

	this.setSupported = function(supportedList) {
		for(var i=0;i<supportedList.length; i++) {
			this.supported[supportedList[i]] = supportedList[i];
		}
		if(!this.isConnected) {
			this.connected();
			this.isConnected = true;
		}
	}

	this.streams = [];
}

Nobdy.prototype.supported = [];

Nobdy.prototype.isConnected = false;

Nobdy.prototype.simulateData = false;

Nobdy.prototype.requestReceived = function(code, value, time) {
	for(var i=0;i<this.streams.length;i++) {
		if(this.streams[i].type == code) {
			this.streams[i].setValue(value);
		}
	}

}

Nobdy.prototype.produceSimulatedData = function () {

	if(!simulateData) return;

	if(nobdy.streams.length)
	{
		for(var i=0;i<nobdy.streams.length;i++) {
			var value=0;

			if(nobdy.streams[i].type == nobdy.supported.Velocity)
			{
				value = Math.random()*250;
			}
			else if(nobdy.streams[i].type == nobdy.supported.EngineRPM)
			{
				value = Math.random()*10000;
			}
			else if(nobdy.streams[i].type == nobdy.supported.EngineCoolantTemp)
			{
				value = Math.random()*180;
			}
			else if(nobdy.streams[i].type == nobdy.supported.ThrottlePosition)
			{
				value = Math.random()*100;
			}
			else if(nobdy.streams[i].type == nobdy.supported.IntakeAirTemp)
			{
				value = Math.random()*200;
			}
			else if(nobdy.streams[i].type == nobdy.supported.EngineLoad)
			{
				value = Math.random()*100;
			}
			else if(nobdy.streams[i].type == nobdy.supported.MassAirFlow)
			{
				value = Math.random()*2550;
			}

			nobdy.streams[i].setValue(value);
		}
	}

	setTimeout(nobdy.produceSimulatedData,1000);
}


Nobdy.prototype.connected = function () { }

Nobdy.prototype.createStream = function(code) {
	this.websocket.send("nobdy.addRequest('"+code+"');");

	var stream = new NobdyStream(code);

	this.streams.push(stream);

	return stream;
}

Nobdy.prototype.issueCommand = function(command) {
	this.websocket.send("nobdy.issueCommand('"+command+"');");
}



