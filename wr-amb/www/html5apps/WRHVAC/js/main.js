
var acOnOff = true;
var AutoManual = true;
var fanSpeed = 0;
var gsocket = null;
var socketController = null;

function ac_arrow_animation(type) {
    var i;
    var arrows = document.getElementsByClassName('arrow');
    var len = arrows.length;
    for (i = 0; i < len; i++) {
        arrows[i].style.opacity = type;
    }
}

function cb_acOnOff() {
	if (acOnOff == true) {
		document.getElementById("aircon-text").innerHTML = "";
		$("#aircon").hide();
		acOnOff = false;
	} else {
		document.getElementById("aircon-text").innerHTML = "A/C";
		$("#aircon").show();
		acOnOff = true
	}

	var name = ['HvacAirConditioning'];
	var value = [acOnOff];
	socketController.changevlaue(name, value)
}

function fan_animation(value) {
    var fan = document.getElementById("fan").style;
    var fan_tmp = document.getElementById("fan-tmp").style;
    var duration;
    if (fanSpeed == 0) {
        duration = 0;
	fan.display = "none";
        fan_tmp.display = "inline";
    } else {
        duration = 1.0;
	fan.display = "inline";
    	fan_tmp.display = "none";
    }
    fan.webkitAnimationDuration = duration +"s";
}

function cb_fanUp(){
    fanSpeed++;
    if (fanSpeed > 8) { fanSpeed = 8; }
    if (fanSpeed < 0) { fanSpeed = 0; }

	var name = ['HvacFanSpeed'];
	var value = [fanSpeed];
	socketController.changevlaue(name, value)

    document.getElementById("fan-bar").src = "images/fan_bar_"+fanSpeed+ ".png";
    fan_animation(fanSpeed + 5);
    ac_arrow_animation(1);
}

function cb_fanDown(){
    fanSpeed--;
    if (fanSpeed > 8) { fanSpeed = 8; }
    if (fanSpeed < 0) { fanSpeed = 0; }
    if (fanSpeed == 0) { ac_arrow_animation(0); }

	var name = ['HvacFanSpeed'];
	var value = [fanSpeed];
	socketController.changevlaue(name, value)

    document.getElementById("fan-bar").src = "images/fan_bar_"+fanSpeed+ ".png";
    fan_animation(fanSpeed);
}

function cb_AutoManual() {
	if (AutoManual) {
		document.getElementById("auto-manual").innerHTML = "Manual";
		AutoManual = false;
	} else {
		document.getElementById("auto-manual").innerHTML = "Auto";
		AutoManual = true
	}
	var name = ['HvacAutoManual'];
	var value = [AutoManual];
	socketController.changevlaue(name, value)
}

function SocketController() {
	this.socketUrl = "ws://localhost:9090";
	this.socketProtocol = "AMB";
	socketController = this;
}

SocketController.prototype.initialize = function() {
	gsocket = new GSocket(this.connected, this.socketUrl, this.socketProtocol);
	webSocketConnect(gsocket);
}

var addEvent = false;
SocketController.prototype.connected = function() {
	console.log("connected");
	var infoList = ['HvacAutoManual', 'HvacFanSpeed', 'HvacAirConditioning', 'HvacTemp'];

	if (gsocket) {
		gsocket.getValueProperty(infoList,
				function(datalist) {
					for (var i = 0; i < datalist.length; i++) {
						var data = datalist[i];
						if (data.name == 'HvacAutoManual') {
							console.log("automanual " + data.value);
							if (data.value == true) {
								document.getElementById("auto-manual").innerHTML = "Auto";
							} else {
								document.getElementById("auto-manual").innerHTML = "Manual";
							}
						} else if (data.name == 'HvacFanSpeed') {
							fanSpeed = data.value;
							console.log("fanspeed " +  fanSpeed);

							if (fanSpeed == 0) { ac_arrow_animation(0); }
							else { ac_arrow_animation(1); }

							document.getElementById("fan-bar").src = "images/fan_bar_"+fanSpeed+".png"; 
							fan_animation(fanSpeed);
						} else if (data.name == 'HvacAirConditioning') {
							acOnOff = data.value;
							console.log("aconoff " + acOnOff);
							if (acOnOff == true) {
								document.getElementById("aircon-text").innerHTML = "A/C";
								$("#aircon").show();
							} else {
								document.getElementById("aircon-text").innerHTML = "";
								$("#aircon").hide();
							}
						} else if (data.name == 'HvacTemp') {
							document.getElementById("temperature").innerHTML = data.value+'°C';
						}
					}
				}, 
				function() {
					console.log("getValue Error Callback");
				});
	}

        if (!addEvent) {
                gsocket.subscribe(["HvacTemp", ]);

                document.addEventListener("HvacTemp",function(data) {
                        var obj = document.getElementById('actemp');
						document.getElementById("temperature").innerHTML = data.value.value+'°C';
                }, false);
                addEvent = true;
        }

}

SocketController.prototype.changevlaue = function(namelist, valuelist, sCB, eCB) {
	console.log("changevalue");
	if (gsocket) {
		gsocket.changeValueProperty(namelist, valuelist, sCB, eCB);
	}
}
