var counter = 0;
var velocityStream;
var batteryVoltageStream;
var rpmStream;
var headingStream;
var engineCoolantStream;
var throttleStream;
var intakeAirTempStream;
var engineLoadStream;
var engineoiltemp;
var mafStream;

var vehicle;

var mpgReadings = 0;
var prevMpg = 0;
var curVss;

function calcAverageMpg(newMpg) {
	prevMpg += parseInt(newMpg);
	mpgReadings++;

	var averageMpg = prevMpg / mpgReadings;
	$("#avgmpg").text(Math.floor(averageMpg));
}


var velReadings = 0;
var prevVel = 0;

function calcAverageVelocity(newVel) {
	prevVel += parseInt(newVel);
	velReadings++;

	var averageVel = prevVel / velReadings;
	$("#avgspeed").text(Math.floor(averageVel));
}

window.onload = function()
{
    var vehicle = tizen.vehicle

    var vehicleSpeed = vehicle.get("VehicleSpeed");
    if(vehicleSpeed != undefined)
        console.log("Vehicle speed: " + vehicleSpeed.vehicleSpeed);

    vehicle.set("MachineGunTurretStatus", { "machineGunTurretStatus" : true },
                function(error) { console.log("set() error " + error); });

    var velocityUnits = $('#velocityUnits');
    velocityUnits.click(function() {
                              if(velocityUnits.text() === "MPH")
                              {
                                  velocityUnits.text("KPH");
                              }
                              else velocityUnits.text("MPH");
                        });

   vehicle.subscribe("VehicleSpeed",function(data) {
				  console.log("Vehicle data" + data.vehicleSpeed);
                                  adjvalue = data.vehicleSpeed;
                                  curVss = adjvalue;
                                  var velocityUnits = $('#velocityUnits');

                                  if(velocityUnits.text() === "MPH")
                                      adjvalue = Math.floor(adjvalue * 0.62137);

                                  $('#velocity').text(adjvalue);

                                  calcAverageVelocity(adjvalue);
                              });

   vehicle.subscribe("EngineSpeed", function(data) {
                                  var value = data.engineSpeed;
                                  if(value > 10000) value =10000;
                                  var needleDegs = value / 10000 * 180;
                                  $('#rpms').text(value);
                                  $('#rpmNeedle').css("-webkit-transform","rotate("+needleDegs+"deg)");
                              });

    vehicle.subscribe("Transmission",function(data) {
                                  value = data.gearPosition;
                                  if(value == 128)
                                      $('#gear').text('Reverse');
                                  else if(value == 0)
                                      $('#gear').text('Neutral');
                                  else $('#gear').text(value);

                              });

    vehicle.subscribe("SteeringWheelAngle", function(data) {
                                  value = data.steeringWheelAngle;
                                  $('#wheel').css("-webkit-transform","rotate("+value+"deg)");
                                  $('#machinegun').css("-webkit-transform","rotate("+value+"deg)");
                              });

    vehicle.subscribe("ThrottlePosition", function(data) {
                                  value = data.throttlePosition;
                                  var needleDegs = (value / 100 * 180) + 270

                                  $('#throttleNeedle').css("-webkit-transform","rotate("+needleDegs+"deg)");

                              });

    vehicle.subscribe("EngineCoolantTemperature", function(data) {
                                  value = data.engineCoolantTemperature;
                                  var needleDegs = (value / 180 * 70) + 270

                                  $('#engineCoolantNeedle').css("-webkit-transform","rotate("+needleDegs+"deg)");

                              });

    vehicle.subscribe("MachineGunTurretStatus", function(data) {
                                  value = data.machineGunTurretStatus;
                                  if(value === "1")
                                      $('#machineGunTurretPopup').popup('open');
                                  else $('#machineGunTurretPopup').popup('close');

                              });

}
