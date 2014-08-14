// gsimulator.js

var directionDisplay;
var directionsService = null;
var map;
var mapdiv;
var gsocket;
var setzoom = false;
var zoomSize = 18;
var carMarker = null;
var inNavigation = false;
var addEvent = false;
var speedMarker = null;
var changeCount = 0;

/*
// Union Square, San Francisco
var startlat = 37.788039;
var startlng = -122.407460;
// Golden Gate Park, San Francisco
var destlat = 37.769171;
var destlng = -122.483516;
*/

// santoninyo, Barcelona
var startlat = 41.405159;
var startlng = 2.174373;
// columbus, Barcelona
var destlat = 41.375822;
var destlng = 2.177762;

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

function changeLocation(lot, lng, spd) {
	var currentLoc = new google.maps.LatLng(lot, lng);

	if (!setzoom) {
		setzoom = true;
		map.setZoom(zoomSize);
	}

	document.getElementById("car-img").style.display = "inline";
	var speed = spd + " Km/h";
	map.panTo(currentLoc);
	document.getElementById("speed-marker").innerHTML = speed;

	/*
	if (!carMarker) {
		var image = new google.maps.MarkerImage('car_marker.png',
				null,null,null,new google.maps.Size(32, 32));

		carMarker = new google.maps.Marker({
				position: currentLoc,
				icon: image,});
		carMarker.setMap(map);
	}
	carMarker.setPosition(currentLoc);

	if (!speedMarker) {

                var labelText = "0Km/h";
                var myOptions = {
                         content: labelText
			,boxStyle: {
                		textAlign: "center"
                		,fontSize: "35pt"
                		,color: "#ff0000"
                		,textShadow: "20px 20px 20px #888"
                		,fontWeight: "bold"
                		,width: "150px"
             		}
                        ,disableAutoPan: true
                        ,pixelOffset: new google.maps.Size(-400, -150)
                        ,position: currentLoc
                        ,closeBoxURL: ""
                        ,isHidden: false
                        };

                speedMarker = new InfoBox(myOptions);
                speedMarker.open(map);
        }
		speedMarker.setPosition(currentLoc);
		speedMarker.setContent(speed);

		if (changeCount > 1) {
			changeCount = 0;
			mapdiv.animate({rotate: "-" + bearing(currentLoc) + 'deg'}, 0);
		} else {
			changeCount++;
		}
		*/
}

function GSimulator() {
	this.startCalc = false;
	//this.socketUrl = "ws://localhost:23000";
	//this.socketProtocol = "http-only";
	// For Alternative AMB
	if(window.location.protocol.indexOf('file')==0)
		this.socketUrl = "ws://localhost:9090";
	else
		this.socketUrl = "ws://"+window.location.hostname+":9090/";
	this.socketProtocol = "AMB";

	this.currentLat = startlat;
	this.currentLng = startlng;
	this.currentSpeed = 0;
}

GSimulator.prototype.initialize = function() {
	directionsDisplay = new google.maps.DirectionsRenderer();
	var loc = new google.maps.LatLng(this.currentLat, this.currentLng);
	var mapOptions = {
		zoom:7,
		mapTypeId: google.maps.MapTypeId.ROADMAP,
		center: loc 
	}
	map = new google.maps.Map(document.getElementById('map-canvas'), mapOptions);
	mapdiv = $('#map-canvas');
	directionsDisplay.setMap(map);

	document.getElementById("car-img").style.display = "none";

	gsocket = new GSocket(this.connected, this.socketUrl, this.socketProtocol);
}

GSimulator.prototype.connected = function() {
	console.log("connected");
	if (!addEvent) {
		gsocket.subscribe(["VehicleSpeed","Latitude","Longitude"]);

		document.addEventListener("VehicleSpeed",function(data) {
                        this.currentSpeed = data.value.value;
                }, false);
		document.addEventListener("Latitude",function(data) {
			this.currentLat = data.value.value;
		}, false);
		document.addEventListener("Longitude", function(data) {
			this.currentLng = data.value.value;
			changeLocation(this.currentLat, this.currentLng, this.currentSpeed);
		}, false);
		addEvent = true;
	}
}

GSimulator.prototype.calcRoute = function(start, end) {
	var request = {
		origin:start,
		destination:end,
		travelMode: google.maps.DirectionsTravelMode.DRIVING
	};
	if (!directionsService)
		directionsService = new google.maps.DirectionsService();
	directionsService.route(request, function(response, status) {
		if (status == google.maps.DirectionsStatus.OK) {
			directionsDisplay.setDirections(response);
			//map.fitBounds(new google.maps.LatLngBounds(response.routes[0].legs[0].steps[0].start).extend(response.routes[0].legs[0].steps[0].end));
		}
	});
	this.startCalc = true;
	setTimeout(function() {
		webSocketConnect(gsocket);
	}, 3000);
}

GSimulator.prototype.inNavigationview = function() {
	inNavigation = true;
}

GSimulator.prototype.outNavigationview = function() {
	inNavigation = false;
}
