// gsimulator.js

var directionDisplay;
var directionsService = new google.maps.DirectionsService();
var map;
var gsocket;
var setzoom = false;
var zoomSize = 15;

function changeLocation(lot, lng) {
	if (!setzoom) {
		setzoom = true;
		map.setZoom(zoomSize);
	}
	map.panTo(new google.maps.LatLng(lot, lng));
}

function GSimulator() {
	this.startCalc = false;
	this.socketUrl = "ws://"+window.location.hostname+":23000";
	this.socketProtocol = "http-only";

	this.currentLat = 41.850033;
	this.currentLng = -87.6500523;

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
	directionsDisplay.setMap(map);

	console.log(this.socketUrl);
	gsocket = new GSocket(this.connected, this.socketUrl, this.socketProtocol);
}

GSimulator.prototype.connected = function() {
	console.log("connected");
	gsocket.subscribe(["Latitude","Longitude"]);

	document.addEventListener("Latitude",function(data) {
		console.log("change Latitude " + data.value.value);
		this.currentLat = data.value.value;
	}, false);
	document.addEventListener("Longitude", function(data) {
		console.log("change Longitude " + data.value.value);
		this.currentLng = data.value.value;
		changeLocation(this.currentLat, this.currentLng);
	}, false);
}

GSimulator.prototype.calcRoute = function() {
	var start = document.getElementById('start').value;
	var end = document.getElementById('end').value;
	var request = {
		origin:start,
		destination:end,
		travelMode: google.maps.DirectionsTravelMode.DRIVING
	};
	directionsService.route(request, function(response, status) {
		if (status == google.maps.DirectionsStatus.OK) {
			directionsDisplay.setDirections(response);
		}
	});
	this.startCalc = true;
	setTimeout(function() {
		webSocketConnect(gsocket);
	}, 1000);
}

