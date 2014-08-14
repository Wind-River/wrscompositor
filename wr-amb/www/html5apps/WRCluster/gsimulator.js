// gsimulator.js

var gsocket;
var addEvent = false;


Number.prototype.toRad = function() { // convert degrees to radians
	return this * Math.PI / 180;
}
Number.prototype.toDeg = function() { // convert radians to degrees (signed)
	return this * 180 / Math.PI;
}
Number.prototype.toBrng = function() { // convert radians to degrees (as bearing: 0...360)
	return (this.toDeg()+360) % 360;
}

function bearing(currentloc) {
	console.log("bearing");
	var lat1 = startlat;
	var lon1 = startlng;
	var lat2 = currentloc.lat();
	var lon2 = currentloc.lng();

	lat1 = lat1.toRad(); lat2 = lat2.toRad();
	console.log("lat1: " + lat1 + ", lat2: " + lat2);
	var dLon = (lon2-lon1).toRad();
	var y = Math.sin(dLon) * Math.cos(lat2);
	var x = Math.cos(lat1)*Math.sin(lat2) -
		Math.sin(lat1)*Math.cos(lat2)*Math.cos(dLon);
	console.log("result: " + Math.atan2(y, x).toBrng());
	return Math.atan2(y, x).toBrng();
}


function GSimulator() {
	this.startCalc = false;
	//this.socketUrl = "ws://localhost:23000";
	//this.socketProtocol = "http-only";
	// For Alternative AMB
	if(window.location.protocol.indexOf('file')==0)
		this.socketUrl = "ws://localhost:9090";
		//this.socketUrl = "ws://192.168.0.198:9090";
	else
		this.socketUrl = "ws://"+window.location.hostname+":9090/";
	this.socketProtocol = "AMB";

}


GSimulator.prototype.initialize = function() {
	gsocket = new GSocket(this.connected, this.socketUrl, this.socketProtocol);
        webSocketConnect(gsocket);
}

var aconoff = false;
var acauto = false;
function setAC() {
    var obj = document.getElementById('acmode');
    console.log('aconoff: '+aconoff);
    console.log('acauto: '+acauto);
    if(aconoff) {
        if(acauto==true)
            obj.innerHTML = 'Auto';
        else
            obj.innerHTML = 'Manual';
    } else
        obj.innerHTML = 'Off';
}
GSimulator.prototype.connected = function() {
	console.log("connected1");


        var infoList = ['HvacAutoManual', 'HvacAirConditioning', 'HvacTemp', 'VehicleSpeed',];

        gsocket.getValueProperty(infoList,
                function(datalist) {
                    console.log(datalist);
                    for (var i = 0; i < datalist.length; i++) {
                        var data = datalist[i];
                        if (data.name == 'HvacAutoManual') {
                            acauto = data.value;
                            setAC();
                        } else if (data.name == 'HvacAirConditioning') {
                            aconoff = data.value;
                            setAC();
                        } else if (data.name == 'HvacTemp') {
                            var obj = document.getElementById('actemp');
                            obj.innerHTML = data.value+'°C';
                        } else if (data.name == 'VehicleSpeed') {
                            var obj = document.getElementById('speednumber');
                            obj.innerHTML = data.value;
                        }
                    }
                }, 
                function() {
                    console.log("getValue Error Callback");
                }
        );


	if (!addEvent) {
		gsocket.subscribe(["TurnSignal", "HvacAutoManual", "HvacAirConditioning", "HvacTemp", "VehicleSpeed"]);

		document.addEventListener("TurnSignal",function(data) {
                        var turnSignal = data.value.value;
                        setDirectionSignal(turnSignal);
                }, false);
		document.addEventListener("HvacAutoManual",function(data) {
                        console.log("HvacAutoManual: "+data.value.value);
                        acauto = data.value.value;
                        setAC();
                }, false);
		document.addEventListener("HvacAirConditioning",function(data) {
                        aconoff = data.value.value;
                        setAC();
                }, false);
		document.addEventListener("HvacTemp",function(data) {
                        var obj = document.getElementById('actemp');
                        obj.innerHTML = data.value.value+'°C';
                }, false);
		document.addEventListener("VehicleSpeed",function(data) {
                        var obj = document.getElementById('speednumber');
                        obj.innerHTML = data.value.value;
						//console.log(parseInt(data.value.value));
						setSpeed(parseInt(data.value.value));
                }, false);
		addEvent = true;
	}
}

