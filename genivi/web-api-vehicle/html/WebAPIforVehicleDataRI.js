/**
 * Copyright (C) 2013, LG Electronics
 *
 * This file is part of GENIVI Project Web Platform.
 *
 * Contributions are licensed to the GENIVI Alliance under one or more
 * Contribution License Agreements.
 *
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Joonhyung Kim <joonhg.kim@lge.com>
 *
 * @file WebAPIforVehicleDataRI.js
 * For further information, see http://www.genivi.org/
 */

var ACCESS_DENIED = 1;
var NOT_AVAILABLE = 2;
var UNKNOWN = 3;

window.onload = function() {
	vehicle = document.getElementById('plugin').vehicle;
	
	getInitialValues();
	setUiListeners();
	setVehicleEventListeners();
}
function getInitialValues() {
	vehicle.get('vehicleinfo_wmi', handleVehicleInfo, null);
	vehicle.get('vehicleinfo_vin', handleVehicleInfo, null);
	vehicle.get('vehicleinfo_vehicletype', handleVehicleInfo, null);
	vehicle.get('vehicleinfo_doortype_1strow', handleVehicleInfo, null);
	vehicle.get('vehicleinfo_doortype_2ndrow', handleVehicleInfo, null);
	vehicle.get('vehicleinfo_doortype_3rdrow', handleVehicleInfo, null);
	vehicle.get('vehicleinfo_fueltype', handleVehicleInfo, null);
	vehicle.get('vehicleinfo_transmissiongeartype', handleVehicleInfo, null);
	vehicle.get('vehicleinfo_wheelinfo_radius', handleVehicleInfo, null);
	vehicle.get('vehicleinfo_wheelinfo_track', handleVehicleInfo, null);
}
function setUiListeners() {
	document.getElementById('set_personalization_language').onclick = setVehicleData;
	document.getElementById('set_personalization_measurementsystem').onclick = setVehicleData;
	document.getElementById('set_personalization_mirror').onclick = setVehicleData;
	document.getElementById('set_personalization_mirror_driver').onclick = setVehicleData;
	document.getElementById('set_personalization_mirror_passenger').onclick = setVehicleData;
	document.getElementById('set_personalization_mirror_inside').onclick = setVehicleData;
	document.getElementById('set_personalization_steeringwheelposition').onclick = setVehicleData;
	document.getElementById('set_personalization_steeringwheelposition_slide').onclick = setVehicleData;
	document.getElementById('set_personalization_steeringwheelposition_tilt').onclick = setVehicleData;
	document.getElementById('set_personalization_drivingmode').onclick = setVehicleData;
	document.getElementById('set_personalization_driverseatposition').onclick = setVehicleData;
	document.getElementById('set_personalization_driverseatposition_reclineseatback').onclick = setVehicleData;
	document.getElementById('set_personalization_driverseatposition_slide').onclick = setVehicleData;
	document.getElementById('set_personalization_driverseatposition_cushionheight').onclick = setVehicleData;
	document.getElementById('set_personalization_driverseatposition_headrest').onclick = setVehicleData;
	document.getElementById('set_personalization_driverseatposition_backcushion').onclick = setVehicleData;
	document.getElementById('set_personalization_driverseatposition_sidecushion').onclick = setVehicleData;
	document.getElementById('set_personalization_passengerseatposition').onclick = setVehicleData;
	document.getElementById('set_personalization_passengerseatposition_reclineseatback').onclick = setVehicleData;
	document.getElementById('set_personalization_passengerseatposition_slide').onclick = setVehicleData;
	document.getElementById('set_personalization_passengerseatposition_cushionheight').onclick = setVehicleData;
	document.getElementById('set_personalization_passengerseatposition_headrest').onclick = setVehicleData;
	document.getElementById('set_personalization_passengerseatposition_backcushion').onclick = setVehicleData;
	document.getElementById('set_personalization_passengerseatposition_sidecushion').onclick = setVehicleData;
	document.getElementById('set_personalization_dashboardillumination').onclick = setVehicleData;
	document.getElementById('set_personalization_generatedvehiclesoundmode').onclick = setVehicleData;
}
function setVehicleEventListeners() {
	vehicle.addEventListener('vehicleinfo', handleVehicleInfo, false);
	vehicle.addEventListener('runningstatus', handleRunningStatus, false);
}
function handleVehicleInfo(data) {
	if (data.type == 'vehicleinfo_wmi') {
		document.getElementById(data.type).innerHTML = data.wmi;
	} else if (data.type == 'vehicleinfo_vin') {
		document.getElementById(data.type).innerHTML = data.vin;
	} else if (data.type == 'vehicleinfo_vehicletype') {
		if (data.vehicleType == 1) { document.getElementById(data.type).innerHTML = "Sedan"; }
		else if (data.vehicleType == 2) { document.getElementById(data.type).innerHTML = "Coupe"; }
		else if (data.vehicleType == 3) { document.getElementById(data.type).innerHTML = "Cabriolet"; }
		else if (data.vehicleType == 4) { document.getElementById(data.type).innerHTML = "Roadster"; }
		else if (data.vehicleType == 5) { document.getElementById(data.type).innerHTML = "SUV"; }
		else if (data.vehicleType == 6) { document.getElementById(data.type).innerHTML = "Truck"; }
	} else if (data.type == 'vehicleinfo_doortype_1strow') {
		document.getElementById(data.type).innerHTML = data.doorType1stRow;
	} else if (data.type == 'vehicleinfo_doortype_2ndrow') {
		document.getElementById(data.type).innerHTML = data.doorType2ndRow;
	} else if (data.type == 'vehicleinfo_doortype_3rdrow') {
		document.getElementById(data.type).innerHTML = data.doorType3rdRow;
	} else if (data.type == 'vehicleinfo_fueltype') {
		if (data.fuelType == 1) { document.getElementById(data.type).innerHTML = "Gasoline"; }
		else if (data.fuelType == 2) { document.getElementById(data.type).innerHTML = "Methanol"; }
		else if (data.fuelType == 3) { document.getElementById(data.type).innerHTML = "Ethanol"; }
		else if (data.fuelType == 4) { document.getElementById(data.type).innerHTML = "Diesel"; }
		else if (data.fuelType == 5) { document.getElementById(data.type).innerHTML = "LPG"; }
		else if (data.fuelType == 6) { document.getElementById(data.type).innerHTML = "CNG"; }
		else if (data.fuelType == 7) { document.getElementById(data.type).innerHTML = "Propane"; }
		else if (data.fuelType == 8) { document.getElementById(data.type).innerHTML = "Electric"; }
		else if (data.fuelType == 9) { document.getElementById(data.type).innerHTML = "Bifuel Running Gasoline"; }
		else if (data.fuelType == 10) { document.getElementById(data.type).innerHTML = "Bifuel Running Methanol"; }
		else if (data.fuelType == 11) { document.getElementById(data.type).innerHTML = "Bifuel Running Ethanol"; }
		else if (data.fuelType == 12) { document.getElementById(data.type).innerHTML = "Bifuel Running LPG"; }
		else if (data.fuelType == 13) { document.getElementById(data.type).innerHTML = "Bifuel Running CNG"; }
		else if (data.fuelType == 14) { document.getElementById(data.type).innerHTML = "Bifuel Running Prop"; }
		else if (data.fuelType == 15) { document.getElementById(data.type).innerHTML = "Bifuel Running Electricity"; }
		else if (data.fuelType == 16) { document.getElementById(data.type).innerHTML = "Bifuel Mixed Gas Electric"; }
		else if (data.fuelType == 17) { document.getElementById(data.type).innerHTML = "Hybrid Gasoline"; }
		else if (data.fuelType == 18) { document.getElementById(data.type).innerHTML = "Hybrid Ethanol"; }
		else if (data.fuelType == 19) { document.getElementById(data.type).innerHTML = "Hybrid Diesel"; }
		else if (data.fuelType == 20) { document.getElementById(data.type).innerHTML = "Hybrid Electric"; }
		else if (data.fuelType == 21) { document.getElementById(data.type).innerHTML = "Hybrid Mixed Fuel"; }
		else if (data.fuelType == 22) { document.getElementById(data.type).innerHTML = "Hybrid Regenerative"; }
	} else if (data.type == 'vehicleinfo_transmissiongeartype') {
		if (data.transmissionGearType == 1) { document.getElementById(data.type).innerHTML = "Auto"; } 
		else if (data.transmissionGearType == 2) { document.getElementById(data.type).innerHTML = "Manual"; }
		else if (data.transmissionGearType == 3) { document.getElementById(data.type).innerHTML = "CVT"; }
	} else if (data.type == 'vehicleinfo_wheelinfo_radius') {
		document.getElementById(data.type).innerHTML = data.wheelInfoRadius;
	} else if (data.type == 'vehicleinfo_wheelinfo_track') {
		document.getElementById(data.type).innerHTML = data.wheelInfoTrack;
	}
}
function handleRunningStatus(data) {
	if (data.type == 'runningstatus_vehiclepowermode') {
		if (data.vehiclePowerMode == 1) { document.getElementById(data.type).innerHTML = "Off"; }
		else if (data.vehiclePowerMode == 2) { document.getElementById(data.type).innerHTML = "Acc"; }
		else if (data.vehiclePowerMode == 3) { document.getElementById(data.type).innerHTML = "Run"; }
		else if (data.vehiclePowerMode == 4) { document.getElementById(data.type).innerHTML = "Ignition"; }
	} else if (data.type == 'runningstatus_speedometer') {
		document.getElementById(data.type).innerHTML = data.speedometer;
	} else if (data.type == 'runningstatus_enginespeed') {
		document.getElementById(data.type).innerHTML = data.engineSpeed;
	} else if (data.type == 'runningstatus_tripmeter_1_mileage') {
		document.getElementById(data.type).innerHTML = data.tripMeter1Mileage;
	} else if (data.type == 'runningstatus_tripmeter_2_mileage') {
		document.getElementById(data.type).innerHTML = data.tripMeter2Mileage;
	} else if (data.type == 'runningstatus_tripmeter_1_averagespeed') {
		document.getElementById(data.type).innerHTML = data.tripMeter1AverageSpeed;
	} else if (data.type == 'runningstatus_tripmeter_2_averagespeed') {
		document.getElementById(data.type).innerHTML = data.tripMeter2AverageSpeed;
	} else if (data.type == 'runningstatus_tripmeter_1_fuelconsumption') {
		document.getElementById(data.type).innerHTML = data.tripMeter1FuelConsumption;
	} else if (data.type == 'runningstatus_tripmeter_2_fuelconsumption') {
		document.getElementById(data.type).innerHTML = data.tripMeter2FuelConsumption;
	} else if (data.type == 'runningstatus_transmissiongearstatus') {
		if (data.transmissionGearStatus == 0) { document.getElementById(data.type).innerHTML = "Neutral"; }
		else if (data.transmissionGearStatus == 1) { document.getElementById(data.type).innerHTML = "1 (Manual)"; }
		else if (data.transmissionGearStatus == 2) { document.getElementById(data.type).innerHTML = "2 (Manual)"; }
		else if (data.transmissionGearStatus == 3) { document.getElementById(data.type).innerHTML = "3 (Manual)"; }
		else if (data.transmissionGearStatus == 4) { document.getElementById(data.type).innerHTML = "4 (Manual)"; }
		else if (data.transmissionGearStatus == 5) { document.getElementById(data.type).innerHTML = "5 (Manual)"; }
		else if (data.transmissionGearStatus == 6) { document.getElementById(data.type).innerHTML = "6 (Manual)"; }
		else if (data.transmissionGearStatus == 7) { document.getElementById(data.type).innerHTML = "7 (Manual)"; }
		else if (data.transmissionGearStatus == 8) { document.getElementById(data.type).innerHTML = "8 (Manual)"; }
		else if (data.transmissionGearStatus == 9) { document.getElementById(data.type).innerHTML = "9 (Manual)"; }
		else if (data.transmissionGearStatus == 10) { document.getElementById(data.type).innerHTML = "10 (Manual)"; }
		else if (data.transmissionGearStatus == 11) { document.getElementById(data.type).innerHTML = "1 (Auto)"; }
		else if (data.transmissionGearStatus == 12) { document.getElementById(data.type).innerHTML = "2 (Auto)"; }
		else if (data.transmissionGearStatus == 13) { document.getElementById(data.type).innerHTML = "3 (Auto)"; }
		else if (data.transmissionGearStatus == 14) { document.getElementById(data.type).innerHTML = "4 (Auto)"; }
		else if (data.transmissionGearStatus == 15) { document.getElementById(data.type).innerHTML = "5 (Auto)"; }
		else if (data.transmissionGearStatus == 16) { document.getElementById(data.type).innerHTML = "6 (Auto)"; }
		else if (data.transmissionGearStatus == 17) { document.getElementById(data.type).innerHTML = "7 (Auto)"; }
		else if (data.transmissionGearStatus == 18) { document.getElementById(data.type).innerHTML = "8 (Auto)"; }
		else if (data.transmissionGearStatus == 19) { document.getElementById(data.type).innerHTML = "9 (Auto)"; }
		else if (data.transmissionGearStatus == 20) { document.getElementById(data.type).innerHTML = "10 (Auto)"; }
		else if (data.transmissionGearStatus == 32) { document.getElementById(data.type).innerHTML = "Drive"; }
		else if (data.transmissionGearStatus == 64) { document.getElementById(data.type).innerHTML = "Parking"; }
		else if (data.transmissionGearStatus == 128) { document.getElementById(data.type).innerHTML = "Reverse"; }
	} else if (data.type == 'runningstatus_cruisecontrol_status') {
		document.getElementById(data.type).innerHTML = data.cruiseControlStatus;
	} else if (data.type == 'runningstatus_cruisecontrol_speed') {
		document.getElementById(data.type).innerHTML = data.cruiseControlSpeed;
	} else if (data.type == 'runningstatus_wheelbrake') {
		if (data.wheelBrake == 1) { document.getElementById(data.type).innerHTML = "Idle"; }
		else if (data.wheelBrake == 2) { document.getElementById(data.type).innerHTML = "Engaged"; }
		else if (data.wheelBrake == 3) { document.getElementById(data.type).innerHTML = "Malfunction"; }
	} else if (data.type == 'runningstatus_lightsstatus_head') {
		document.getElementById(data.type).innerHTML = data.lightsStatusHead;
	} else if (data.type == 'runningstatus_lightsstatus_highbeam') {
		document.getElementById(data.type).innerHTML = data.lightsStatusHighBeam;
	} else if (data.type == 'runningstatus_lightsstatus_turnleft') {
		document.getElementById(data.type).innerHTML = data.lightsStatusTurnLeft;
	} else if (data.type == 'runningstatus_lightsstatus_turnright') {
		document.getElementById(data.type).innerHTML = data.lightsStatusTurnRight;
	} else if (data.type == 'runningstatus_lightsstatus_brake') {
		document.getElementById(data.type).innerHTML = data.lightsStatusBrake;
	} else if (data.type == 'runningstatus_lightsstatus_fogfront') {
		document.getElementById(data.type).innerHTML = data.lightsStatusFogFront;
	} else if (data.type == 'runningstatus_lightsstatus_fogrear') {
		document.getElementById(data.type).innerHTML = data.lightsStatusFogRear;
	} else if (data.type == 'runningstatus_lightsstatus_hazard') {
		document.getElementById(data.type).innerHTML = data.lightsStatusHazard;
	} else if (data.type == 'runningstatus_lightsstatus_parking') {
		document.getElementById(data.type).innerHTML = data.lightsStatusParking;
	} else if (data.type == 'runningstatus_interiorlightsstatus_driver') {
		document.getElementById(data.type).innerHTML = data.interiorLightsStatusDriver;
	} else if (data.type == 'runningstatus_interiorlightsstatus_passenger') {
		document.getElementById(data.type).innerHTML = data.interiorLightsStatusPassenger;
	} else if (data.type == 'runningstatus_interiorlightsstatus_center') {
		document.getElementById(data.type).innerHTML = data.interiorLightsStatusCenter;
	} else if (data.type == 'runningstatus_automaticheadlights') {
		document.getElementById(data.type).innerHTML = data.automaticHeadlights;
	} else if (data.type == 'runningstatus_dynamichighbeam') {
		document.getElementById(data.type).innerHTML = data.dynamicHighBeam;
	} else if (data.type == 'runningstatus_horn') {
		document.getElementById(data.type).innerHTML = data.horn;
	} else if (data.type == 'runningstatus_chime') {
		document.getElementById(data.type).innerHTML = data.chime;
	} else if (data.type == 'runningstatus_fuel') {
		document.getElementById(data.type).innerHTML = data.fuel;
	} else if (data.type == 'runningstatus_estimatedrange') {
		document.getElementById(data.type).innerHTML = data.estimatedRange;
	} else if (data.type == 'runningstatus_engineoil_remaining') {
		document.getElementById(data.type).innerHTML = data.engineOilRemaining;
	} else if (data.type == 'runningstatus_engineoil_change') {
		document.getElementById(data.type).innerHTML = data.engineOilChange;
	} else if (data.type == 'runningstatus_engineoil_temp') {
		document.getElementById(data.type).innerHTML = data.engineOilTemp;
	} else if (data.type == 'runningstatus_enginecoolant_level') {
		if (data.engineCoolantLevel == 0) { document.getElementById(data.type).innerHTML = "Normal"; }
		else if (data.engineCoolantLevel == 1) { document.getElementById(data.type).innerHTML = "Low"; }
	} else if (data.type == 'runningstatus_enginecoolant_temp') {
		document.getElementById(data.type).innerHTML = data.engineCoolantTemp;
	} else if (data.type == 'runningstatus_steeringwheelangle') {
		document.getElementById(data.type).innerHTML = data.steeringWheelAngle;
	}
}
function setVehicleData() {
	var obj = new Object();
	if (this.id == 'set_personalization_language') {
		obj.language = document.getElementById(this.id.substring(4)).value;
	} else if (this.id == 'set_personalization_measurementsystem') {
		obj.measurementSystem = document.getElementById(this.id.substring(4)).value;
	} else if (this.id == 'set_personalization_mirror') {
		obj.mirrorDriver = document.getElementById(this.id.substring(4) + '_driver').value;
		obj.mirrorPassenger = document.getElementById(this.id.substring(4) + '_passenger').value;
		obj.mirrorInside = document.getElementById(this.id.substring(4) + '_inside').value;
	} else if (this.id == 'set_personalization_mirror_driver') {
		obj.mirrorDriver = document.getElementById(this.id.substring(4)).value;
	} else if (this.id == 'set_personalization_mirror_passenger') {
		obj.mirrorPassenger = document.getElementById(this.id.substring(4)).value;
	} else if (this.id == 'set_personalization_mirror_inside') {
		obj.mirrorInside = document.getElementById(this.id.substring(4)).value;
	} else if (this.id == 'set_personalization_steeringwheelposition') {
		obj.steeringWheelPositionSlide = document.getElementById(this.id.substring(4) + '_slide').value;
		obj.steeringWheelPositionTilt = document.getElementById(this.id.substring(4) + '_tilt').value;
	} else if (this.id == 'set_personalization_steeringwheelposition_slide') {
		obj.steeringWheelPositionSlide = document.getElementById(this.id.substring(4)).value;
	} else if (this.id == 'set_personalization_steeringwheelposition_tilt') {
		obj.steeringWheelPositionTilt = document.getElementById(this.id.substring(4)).value;
	} else if (this.id == 'set_personalization_drivingmode') {
		obj.drivingMode = document.getElementById(this.id.substring(4)).value;
	} else if (this.id == 'set_personalization_driverseatposition') {
		obj.driverSeatPositionReclineSeatback = document.getElementById(this.id.substring(4) + '_reclineseatback').value;
		obj.driverSeatPositionSlide = document.getElementById(this.id.substring(4) + '_slide').value;
		obj.driverSeatPositionCushionHeight = document.getElementById(this.id.substring(4) + '_cushionheight').value;
		obj.driverSeatPositionHeadrest = document.getElementById(this.id.substring(4) + '_headrest').value;
		obj.driverSeatPositionBackCushion = document.getElementById(this.id.substring(4) + '_backcushion').value;
		obj.driverSeatPositionSideCushion = document.getElementById(this.id.substring(4) + '_sidecushion').value;
	} else if (this.id == 'set_personalization_driverseatposition_reclineseatback') {
		obj.driverSeatPositionReclineSeatback = document.getElementById(this.id.substring(4)).value;
	} else if (this.id == 'set_personalization_driverseatposition_slide') {
		obj.driverSeatPositionSlide = document.getElementById(this.id.substring(4)).value;
	} else if (this.id == 'set_personalization_driverseatposition_cushionheight') {
		obj.driverSeatPositionCushionHeight = document.getElementById(this.id.substring(4)).value;
	} else if (this.id == 'set_personalization_driverseatposition_headrest') {
		obj.driverSeatPositionHeadrest = document.getElementById(this.id.substring(4)).value;
	} else if (this.id == 'set_personalization_driverseatposition_backcushion') {
		obj.driverSeatPositionBackCushion = document.getElementById(this.id.substring(4)).value;
	} else if (this.id == 'set_personalization_driverseatposition_sidecushion') {
		obj.driverSeatPositionSideCushion = document.getElementById(this.id.substring(4)).value;
	} else if (this.id == 'set_personalization_passengerseatposition') {
		obj.passengerSeatPositionReclineSeatback = document.getElementById(this.id.substring(4) + '_reclineseatback').value;
		obj.passengerSeatPositionSlide = document.getElementById(this.id.substring(4) + '_slide').value;
		obj.passengerSeatPositionCushionHeight = document.getElementById(this.id.substring(4) + '_cushionheight').value;
		obj.passengerSeatPositionHeadrest = document.getElementById(this.id.substring(4) + '_headrest').value;
		obj.passengerSeatPositionBackCushion = document.getElementById(this.id.substring(4) + '_backcushion').value;
		obj.passengerSeatPositionSideCushion = document.getElementById(this.id.substring(4) + '_sidecushion').value;
	} else if (this.id == 'set_personalization_passengerseatposition_reclineseatback') {
		obj.passengerSeatPositionReclineSeatback = document.getElementById(this.id.substring(4)).value;
	} else if (this.id == 'set_personalization_passengerseatposition_slide') {
		obj.passengerSeatPositionSlide = document.getElementById(this.id.substring(4)).value;
	} else if (this.id == 'set_personalization_passengerseatposition_cushionheight') {
		obj.passengerSeatPositionCushionHeight = document.getElementById(this.id.substring(4)).value;
	} else if (this.id == 'set_personalization_passengerseatposition_headrest') {
		obj.passengerSeatPositionHeadrest = document.getElementById(this.id.substring(4)).value;
	} else if (this.id == 'set_personalization_passengerseatposition_backcushion') {
		obj.passengerSeatPositionBackCushion = document.getElementById(this.id.substring(4)).value;
	} else if (this.id == 'set_personalization_passengerseatposition_sidecushion') {
		obj.passengerSeatPositionSideCushion = document.getElementById(this.id.substring(4)).value;
	} else if (this.id == 'set_personalization_dashboardillumination') {
		obj.dashboardIllumination = document.getElementById(this.id.substring(4)).value;
	} else if (this.id == 'set_personalization_generatedvehiclesoundmode') {
		obj.generatedVehicleSoundMode = document.getElementById(this.id.substring(4)).value;
	}
	vehicle.set(this.id.substring(4), obj, null, null);
}
