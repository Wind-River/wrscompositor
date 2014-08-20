/**
 * Copyright (C) 2013, LG Electronics
 *
 * This file is part of GENIVI Project web-api-vehicle.
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
 * @file DrivingSafetyEvent.cpp
 * For further information, see http://www.genivi.org/
 */

#include "JSObject.h"
#include <boost/assign.hpp>
#include "DOM/Document.h"

#include "DrivingSafetyEvent.h"

const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY = "drivingsafety";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_ANTILOCKBRAKINGSYSTEM = "drivingsafety_antilockbrakingsystem";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_TRACTIONCONTROLSYSTEM = "drivingsafety_tractioncontrolsystem";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_ELECTRONICSTABILITYCONTROL = "drivingsafety_electronicstabilitycontrol";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_VEHICLETOPSPEEDLIMIT = "drivingsafety_vehicletopspeedlimit";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_AIRBAGSTATUS = "drivingsafety_airbagstatus";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_AIRBAGSTATUS_DRIVER = "drivingsafety_airbagstatus_driver";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_AIRBAGSTATUS_PASSENGER = "drivingsafety_airbagstatus_passenger";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_AIRBAGSTATUS_SIDE = "drivingsafety_airbagstatus_side";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_DOOROPENSTATUS = "drivingsafety_dooropenstatus";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_DOOROPENSTATUS_DRIVER = "drivingsafety_dooropenstatus_driver";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_DOOROPENSTATUS_PASSENGER = "drivingsafety_dooropenstatus_passenger";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_DOOROPENSTATUS_REARLEFT = "drivingsafety_dooropenstatus_rearleft";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_DOOROPENSTATUS_REARRIGHT = "drivingsafety_dooropenstatus_rearright";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_DOOROPENSTATUS_TRUNK = "drivingsafety_dooropenstatus_trunk";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_DOOROPENSTATUS_FUELFILTERCAP = "drivingsafety_dooropenstatus_fuelfiltercap";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_DOOROPENSTATUS_HOOD = "drivingsafety_dooropenstatus_hood";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_DOORLOCKSTATUS = "drivingsafety_doorlockstatus";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_DOORLOCKSTATUS_DRIVER = "drivingsafety_doorlockstatus_driver";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_DOORLOCKSTATUS_PASSENGER = "drivingsafety_doorlockstatus_passenger";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_DOORLOCKSTATUS_REARLEFT = "drivingsafety_doorlockstatus_rearleft";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_DOORLOCKSTATUS_REARRIGHT = "drivingsafety_doorlockstatus_rearright";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_CHILDSAFETYLOCK = "drivingsafety_childsafetylock";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_OCCUPANTSSTATUS = "drivingsafety_occupantsstatus";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_OCCUPANTSSTATUS_DRIVER = "drivingsafety_occupantsstatus_driver";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_OCCUPANTSSTATUS_PASSENGER = "drivingsafety_occupantsstatus_passenger";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_OCCUPANTSSTATUS_REARLEFT = "drivingsafety_occupantsstatus_rearleft";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_OCCUPANTSSTATUS_REARRIGHT = "drivingsafety_occupantsstatus_rearright";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_SEATBELT = "drivingsafety_seatbelt";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_SEATBELT_DRIVER = "drivingsafety_seatbelt_driver";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_SEATBELT_PASSENGER = "drivingsafety_seatbelt_passenger";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_SEATBELT_REARLEFT = "drivingsafety_seatbelt_rearleft";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_SEATBELT_REARRIGHT = "drivingsafety_seatbelt_rearright";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_WINDOWLOCK = "drivingsafety_windowlock";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_WINDOWLOCK_DRIVER = "drivingsafety_windowlock_driver";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_WINDOWLOCK_PASSENGER = "drivingsafety_windowlock_passenger";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_WINDOWLOCK_REARLEFT = "drivingsafety_windowlock_rearleft";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_WINDOWLOCK_REARRIGHT = "drivingsafety_windowlock_rearright";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_OBSTACLEDISTANCE = "drivingsafety_obstacledistance";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_OBSTACLEDISTANCE_SENSORSTATUS = "drivingsafety_obstacledistance_sensorstatus";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTCENTER = "drivingsafety_obstacledistance_frontcenter";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_OBSTACLEDISTANCE_REARCENTER = "drivingsafety_obstacledistance_rearcenter";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTLEFT = "drivingsafety_obstacledistance_frontleft";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTRIGHT = "drivingsafety_obstacledistance_frontright";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_OBSTACLEDISTANCE_MIDDLELEFT = "drivingsafety_obstacledistance_middleleft";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_OBSTACLEDISTANCE_MIDDLERIGHT = "drivingsafety_obstacledistance_middleright";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_OBSTACLEDISTANCE_REARLEFT = "drivingsafety_obstacledistance_rearleft";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_OBSTACLEDISTANCE_REARRIGHT = "drivingsafety_obstacledistance_rearright";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_FRONTCOLLISIONDETECTION = "drivingsafety_frontcollisiondetection";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_FRONTCOLLISIONDETECTION_STATUS = "drivingsafety_frontcollisiondetection_status";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_FRONTCOLLISIONDETECTION_DISTANCE = "drivingsafety_frontcollisiondetection_distance";
const VehicleEventType DrivingSafetyEvent::DRIVINGSAFETY_FRONTCOLLISIONDETECTION_TIME = "drivingsafety_frontcollisiondetection_time";

DrivingSafetyEvent::DrivingSafetyEvent(const FB::BrowserHostPtr& host)
{
	registerProperty("type",
					 make_property(this,
								   &DrivingSafetyEvent::getType));
	registerProperty("antilockBrakingSystem",
					 make_property(this,
								   &DrivingSafetyEvent::getAntilockBrakingSystem));
	registerProperty("tractionControlSystem",
					 make_property(this,
								   &DrivingSafetyEvent::getTractionControlSystem));
	registerProperty("electronicStabilityControl",
					 make_property(this,
								   &DrivingSafetyEvent::getElectronicStabilityControl));
	registerProperty("vehicleTopSpeedLimit",
					 make_property(this,
								   &DrivingSafetyEvent::getVehicleTopSpeedLimit));
	registerProperty("airbagStatusDriver",
					 make_property(this,
								   &DrivingSafetyEvent::getAirbagStatusDriver));
	registerProperty("airbagStatusPassenger",
					 make_property(this,
								   &DrivingSafetyEvent::getAirbagStatusPassenger));
	registerProperty("airbagStatusSide",
					 make_property(this,
								   &DrivingSafetyEvent::getAirbagStatusSide));
	registerProperty("doorOpenStatusDriver",
					 make_property(this,
								   &DrivingSafetyEvent::getDoorOpenStatusDriver));
	registerProperty("doorOpenStatusPassenger",
					 make_property(this,
								   &DrivingSafetyEvent::getDoorOpenStatusPassenger));
	registerProperty("doorOpenStatusRearLeft",
					 make_property(this,
								   &DrivingSafetyEvent::getDoorOpenStatusRearLeft));
	registerProperty("doorOpenStatusRearRight",
					 make_property(this,
								   &DrivingSafetyEvent::getDoorOpenStatusRearRight));
	registerProperty("doorOpenStatusTrunk",
					 make_property(this,
								   &DrivingSafetyEvent::getDoorOpenStatusTrunk));
	registerProperty("doorOpenStatusFuelFilterCap",
					 make_property(this,
								   &DrivingSafetyEvent::getDoorOpenStatusFuelFilterCap));
	registerProperty("doorOpenStatusHood",
					 make_property(this,
								   &DrivingSafetyEvent::getDoorOpenStatusHood));
	registerProperty("doorLockStatusDriver",
					 make_property(this,
								   &DrivingSafetyEvent::getDoorLockStatusDriver));
	registerProperty("doorLockStatusPassenger",
					 make_property(this,
								   &DrivingSafetyEvent::getDoorLockStatusPassenger));
	registerProperty("doorLockStatusRearLeft",
					 make_property(this,
								   &DrivingSafetyEvent::getDoorLockStatusRearLeft));
	registerProperty("doorLockStatusRearRight",
					 make_property(this,
								   &DrivingSafetyEvent::getDoorLockStatusRearRight));
	registerProperty("childSafetyLock",
					 make_property(this,
								   &DrivingSafetyEvent::getChildSafetyLock));
	registerProperty("occupantsStatusDriver",
					 make_property(this,
								   &DrivingSafetyEvent::getOccupantsStatusDriver));
	registerProperty("occupantsStatusPassenger",
					 make_property(this,
								   &DrivingSafetyEvent::getOccupantsStatusPassenger));
	registerProperty("occupantsStatusRearLeft",
					 make_property(this,
								   &DrivingSafetyEvent::getOccupantsStatusRearLeft));
	registerProperty("occupantsStatusRearRight",
					 make_property(this,
								   &DrivingSafetyEvent::getOccupantsStatusRearRight));
	registerProperty("seatBeltDriver",
					 make_property(this,
								   &DrivingSafetyEvent::getSeatBeltDriver));
	registerProperty("seatBeltPassenger",
					 make_property(this,
								   &DrivingSafetyEvent::getSeatBeltPassenger));
	registerProperty("seatBeltRearLeft",
					 make_property(this,
								   &DrivingSafetyEvent::getSeatBeltRearLeft));
	registerProperty("seatBeltRearRight",
					 make_property(this,
								   &DrivingSafetyEvent::getSeatBeltRearRight));
	registerProperty("windowLockDriver",
					 make_property(this,
								   &DrivingSafetyEvent::getWindowLockDriver));
	registerProperty("windowLockPassenger",
					 make_property(this,
								   &DrivingSafetyEvent::getWindowLockPassenger));
	registerProperty("windowLockRearLeft",
					 make_property(this,
								   &DrivingSafetyEvent::getWindowLockRearLeft));
	registerProperty("windowLockRearRight",
					 make_property(this,
								   &DrivingSafetyEvent::getWindowLockRearRight));
	registerProperty("obstacleDistanceSensorStatus",
					 make_property(this,
								   &DrivingSafetyEvent::getObstacleDistanceSensorStatus));
	registerProperty("obstacleDistanceFrontCenter",
					 make_property(this,
								   &DrivingSafetyEvent::getObstacleDistanceFrontCenter));
	registerProperty("obstacleDistanceRearCenter",
					 make_property(this,
								   &DrivingSafetyEvent::getObstacleDistanceRearCenter));
	registerProperty("obstacleDistanceFrontLeft",
					 make_property(this,
								   &DrivingSafetyEvent::getObstacleDistanceFrontLeft));
	registerProperty("obstacleDistanceFrontRight",
					 make_property(this,
								   &DrivingSafetyEvent::getObstacleDistanceFrontRight));
	registerProperty("obstacleDistanceMiddleLeft",
					 make_property(this,
								   &DrivingSafetyEvent::getObstacleDistanceMiddleLeft));
	registerProperty("obstacleDistanceMiddleRight",
					 make_property(this,
								   &DrivingSafetyEvent::getObstacleDistanceMiddleRight));
	registerProperty("obstacleDistanceRearLeft",
					 make_property(this,
								   &DrivingSafetyEvent::getObstacleDistanceRearLeft));
	registerProperty("obstacleDistanceRearRight",
					 make_property(this,
								   &DrivingSafetyEvent::getObstacleDistanceRearRight));
	registerProperty("frontCollisionDetectionStatus",
					 make_property(this,
								   &DrivingSafetyEvent::getFrontCollisionDetectionStatus));
	registerProperty("frontCollisionDetectionDistance",
					 make_property(this,
								   &DrivingSafetyEvent::getFrontCollisionDetectionDistance));
	registerProperty("frontCollisionDetectionTime",
					 make_property(this,
								   &DrivingSafetyEvent::getFrontCollisionDetectionTime));

	allSupportedEventTypesR.push_back(DRIVINGSAFETY);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_ANTILOCKBRAKINGSYSTEM);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_TRACTIONCONTROLSYSTEM);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_ELECTRONICSTABILITYCONTROL);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_VEHICLETOPSPEEDLIMIT);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_AIRBAGSTATUS);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_AIRBAGSTATUS_DRIVER);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_AIRBAGSTATUS_PASSENGER);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_AIRBAGSTATUS_SIDE);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_DOOROPENSTATUS);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_DOOROPENSTATUS_DRIVER);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_DOOROPENSTATUS_PASSENGER);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_DOOROPENSTATUS_REARLEFT);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_DOOROPENSTATUS_REARRIGHT);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_DOOROPENSTATUS_TRUNK); 
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_DOOROPENSTATUS_FUELFILTERCAP);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_DOOROPENSTATUS_HOOD);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_DOORLOCKSTATUS);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_DOORLOCKSTATUS_DRIVER);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_DOORLOCKSTATUS_PASSENGER);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_DOORLOCKSTATUS_REARLEFT);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_DOORLOCKSTATUS_REARRIGHT);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_CHILDSAFETYLOCK);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_OCCUPANTSSTATUS);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_OCCUPANTSSTATUS_DRIVER);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_OCCUPANTSSTATUS_PASSENGER);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_OCCUPANTSSTATUS_REARLEFT);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_OCCUPANTSSTATUS_REARRIGHT);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_SEATBELT);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_SEATBELT_DRIVER);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_SEATBELT_PASSENGER);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_SEATBELT_REARLEFT);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_SEATBELT_REARRIGHT);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_WINDOWLOCK);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_WINDOWLOCK_DRIVER);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_WINDOWLOCK_PASSENGER);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_WINDOWLOCK_REARLEFT);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_WINDOWLOCK_REARRIGHT);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_OBSTACLEDISTANCE);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_OBSTACLEDISTANCE_SENSORSTATUS);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTCENTER);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_OBSTACLEDISTANCE_REARCENTER);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTLEFT);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTRIGHT);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_OBSTACLEDISTANCE_MIDDLELEFT);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_OBSTACLEDISTANCE_MIDDLERIGHT);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_OBSTACLEDISTANCE_REARLEFT);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_OBSTACLEDISTANCE_REARRIGHT);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_FRONTCOLLISIONDETECTION);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_FRONTCOLLISIONDETECTION_STATUS);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_FRONTCOLLISIONDETECTION_DISTANCE);
	allSupportedEventTypesR.push_back(DRIVINGSAFETY_FRONTCOLLISIONDETECTION_TIME);

	allSupportedEventTypesW.push_back(DRIVINGSAFETY_VEHICLETOPSPEEDLIMIT);
	allSupportedEventTypesW.push_back(DRIVINGSAFETY_DOORLOCKSTATUS);
	allSupportedEventTypesW.push_back(DRIVINGSAFETY_DOORLOCKSTATUS_DRIVER);
	allSupportedEventTypesW.push_back(DRIVINGSAFETY_DOORLOCKSTATUS_PASSENGER);
	allSupportedEventTypesW.push_back(DRIVINGSAFETY_DOORLOCKSTATUS_REARLEFT);
	allSupportedEventTypesW.push_back(DRIVINGSAFETY_DOORLOCKSTATUS_REARRIGHT);
	allSupportedEventTypesW.push_back(DRIVINGSAFETY_WINDOWLOCK);
	allSupportedEventTypesW.push_back(DRIVINGSAFETY_WINDOWLOCK_DRIVER);
	allSupportedEventTypesW.push_back(DRIVINGSAFETY_WINDOWLOCK_PASSENGER);
	allSupportedEventTypesW.push_back(DRIVINGSAFETY_WINDOWLOCK_REARLEFT);
	allSupportedEventTypesW.push_back(DRIVINGSAFETY_WINDOWLOCK_REARRIGHT);

	validity[DRIVINGSAFETY] = false;
	validity[DRIVINGSAFETY_TRACTIONCONTROLSYSTEM] = false;
	validity[DRIVINGSAFETY_ELECTRONICSTABILITYCONTROL] = false;
	validity[DRIVINGSAFETY_VEHICLETOPSPEEDLIMIT] = false;
	validity[DRIVINGSAFETY_AIRBAGSTATUS] = false;
	validity[DRIVINGSAFETY_AIRBAGSTATUS_DRIVER] = false;
	validity[DRIVINGSAFETY_AIRBAGSTATUS_PASSENGER] = false;
	validity[DRIVINGSAFETY_AIRBAGSTATUS_SIDE] = false;
	validity[DRIVINGSAFETY_DOOROPENSTATUS] = false;
	validity[DRIVINGSAFETY_DOOROPENSTATUS_DRIVER] = false;
	validity[DRIVINGSAFETY_DOOROPENSTATUS_PASSENGER] = false;
	validity[DRIVINGSAFETY_DOOROPENSTATUS_REARLEFT] = false;
	validity[DRIVINGSAFETY_DOOROPENSTATUS_REARRIGHT] = false;
	validity[DRIVINGSAFETY_DOOROPENSTATUS_TRUNK] = false; 
	validity[DRIVINGSAFETY_DOOROPENSTATUS_FUELFILTERCAP] = false;
	validity[DRIVINGSAFETY_DOOROPENSTATUS_HOOD] = false;
	validity[DRIVINGSAFETY_DOORLOCKSTATUS] = false;
	validity[DRIVINGSAFETY_DOORLOCKSTATUS_DRIVER] = false;
	validity[DRIVINGSAFETY_DOORLOCKSTATUS_PASSENGER] = false;
	validity[DRIVINGSAFETY_DOORLOCKSTATUS_REARLEFT] = false;
	validity[DRIVINGSAFETY_DOORLOCKSTATUS_REARRIGHT] = false;
	validity[DRIVINGSAFETY_CHILDSAFETYLOCK] = false;
	validity[DRIVINGSAFETY_OCCUPANTSSTATUS] = false;
	validity[DRIVINGSAFETY_OCCUPANTSSTATUS_DRIVER] = false;
	validity[DRIVINGSAFETY_OCCUPANTSSTATUS_PASSENGER] = false;
	validity[DRIVINGSAFETY_OCCUPANTSSTATUS_REARLEFT] = false;
	validity[DRIVINGSAFETY_OCCUPANTSSTATUS_REARRIGHT] = false;
	validity[DRIVINGSAFETY_SEATBELT] = false;
	validity[DRIVINGSAFETY_SEATBELT_DRIVER] = false;
	validity[DRIVINGSAFETY_SEATBELT_PASSENGER] = false;
	validity[DRIVINGSAFETY_SEATBELT_REARLEFT] = false;
	validity[DRIVINGSAFETY_SEATBELT_REARRIGHT] = false;
	validity[DRIVINGSAFETY_WINDOWLOCK] = false;
	validity[DRIVINGSAFETY_WINDOWLOCK_DRIVER] = false;
	validity[DRIVINGSAFETY_WINDOWLOCK_PASSENGER] = false;
	validity[DRIVINGSAFETY_WINDOWLOCK_REARLEFT] = false;
	validity[DRIVINGSAFETY_WINDOWLOCK_REARRIGHT] = false;
	validity[DRIVINGSAFETY_OBSTACLEDISTANCE] = false;
	validity[DRIVINGSAFETY_OBSTACLEDISTANCE_SENSORSTATUS] = false;
	validity[DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTCENTER] = false;
	validity[DRIVINGSAFETY_OBSTACLEDISTANCE_REARCENTER] = false;
	validity[DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTLEFT] = false;
	validity[DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTRIGHT] = false;
	validity[DRIVINGSAFETY_OBSTACLEDISTANCE_MIDDLELEFT] = false;
	validity[DRIVINGSAFETY_OBSTACLEDISTANCE_MIDDLERIGHT] = false;
	validity[DRIVINGSAFETY_OBSTACLEDISTANCE_REARLEFT] = false;
	validity[DRIVINGSAFETY_OBSTACLEDISTANCE_REARRIGHT] = false;
	validity[DRIVINGSAFETY_FRONTCOLLISIONDETECTION] = false;
	validity[DRIVINGSAFETY_FRONTCOLLISIONDETECTION_STATUS] = false;
	validity[DRIVINGSAFETY_FRONTCOLLISIONDETECTION_DISTANCE] = false;
	validity[DRIVINGSAFETY_FRONTCOLLISIONDETECTION_TIME] = false;
}

DrivingSafetyEvent::~DrivingSafetyEvent()
{

}

void DrivingSafetyEvent::updateValidity(VehicleEventType type)
{
	std::string str = type;
	size_t pos;

	while ((pos = str.find_last_of('_')) != std::string::npos)
	{
		str = str.substr(0, pos);

		if (str.compare(DRIVINGSAFETY) == 0)
		{
			setValidity(DRIVINGSAFETY, isValid(DRIVINGSAFETY_ANTILOCKBRAKINGSYSTEM)
						& isValid(DRIVINGSAFETY_TRACTIONCONTROLSYSTEM)
						& isValid(DRIVINGSAFETY_ELECTRONICSTABILITYCONTROL)
						& isValid(DRIVINGSAFETY_VEHICLETOPSPEEDLIMIT)
						& isValid(DRIVINGSAFETY_AIRBAGSTATUS)
						& isValid(DRIVINGSAFETY_DOOROPENSTATUS)
						& isValid(DRIVINGSAFETY_DOORLOCKSTATUS)
						& isValid(DRIVINGSAFETY_CHILDSAFETYLOCK)
						& isValid(DRIVINGSAFETY_OCCUPANTSSTATUS)
						& isValid(DRIVINGSAFETY_SEATBELT)
						& isValid(DRIVINGSAFETY_WINDOWLOCK)
						& isValid(DRIVINGSAFETY_OBSTACLEDISTANCE)
						& isValid(DRIVINGSAFETY_FRONTCOLLISIONDETECTION));
		}
		else if (str.compare(DRIVINGSAFETY_AIRBAGSTATUS) == 0)
		{
			setValidity(DRIVINGSAFETY_AIRBAGSTATUS, isValid(DRIVINGSAFETY_AIRBAGSTATUS_DRIVER)
						& isValid(DRIVINGSAFETY_AIRBAGSTATUS_PASSENGER)
						& isValid(DRIVINGSAFETY_AIRBAGSTATUS_SIDE));

		}
		else if (str.compare(DRIVINGSAFETY_DOOROPENSTATUS) == 0)
		{
			setValidity(DRIVINGSAFETY_DOOROPENSTATUS, isValid(DRIVINGSAFETY_DOOROPENSTATUS_DRIVER)
						& isValid(DRIVINGSAFETY_DOOROPENSTATUS_PASSENGER)
						& isValid(DRIVINGSAFETY_DOOROPENSTATUS_REARLEFT)
						& isValid(DRIVINGSAFETY_DOOROPENSTATUS_REARRIGHT)
						& isValid(DRIVINGSAFETY_DOOROPENSTATUS_TRUNK) 
						& isValid(DRIVINGSAFETY_DOOROPENSTATUS_FUELFILTERCAP)
						& isValid(DRIVINGSAFETY_DOOROPENSTATUS_HOOD));

		}
		else if (str.compare(DRIVINGSAFETY_DOORLOCKSTATUS) == 0)
		{
			setValidity(DRIVINGSAFETY_DOORLOCKSTATUS, isValid(DRIVINGSAFETY_DOORLOCKSTATUS_DRIVER)
						& isValid(DRIVINGSAFETY_DOORLOCKSTATUS_PASSENGER)
						& isValid(DRIVINGSAFETY_DOORLOCKSTATUS_REARLEFT)
						& isValid(DRIVINGSAFETY_DOORLOCKSTATUS_REARRIGHT));

		}
		else if (str.compare(DRIVINGSAFETY_OCCUPANTSSTATUS) == 0)
		{
			setValidity(DRIVINGSAFETY_OCCUPANTSSTATUS, isValid(DRIVINGSAFETY_OCCUPANTSSTATUS_DRIVER)
						& isValid(DRIVINGSAFETY_OCCUPANTSSTATUS_PASSENGER)
						& isValid(DRIVINGSAFETY_OCCUPANTSSTATUS_REARLEFT)
						& isValid(DRIVINGSAFETY_OCCUPANTSSTATUS_REARRIGHT));

		}
		else if (str.compare(DRIVINGSAFETY_SEATBELT) == 0)
		{
			setValidity(DRIVINGSAFETY_SEATBELT, isValid(DRIVINGSAFETY_SEATBELT_DRIVER)
						& isValid(DRIVINGSAFETY_SEATBELT_PASSENGER)
						& isValid(DRIVINGSAFETY_SEATBELT_REARLEFT)
						& isValid(DRIVINGSAFETY_SEATBELT_REARRIGHT));

		}
		else if (str.compare(DRIVINGSAFETY_WINDOWLOCK) == 0)
		{
			setValidity(DRIVINGSAFETY_WINDOWLOCK, isValid(DRIVINGSAFETY_WINDOWLOCK_DRIVER)
						& isValid(DRIVINGSAFETY_WINDOWLOCK_PASSENGER)
						& isValid(DRIVINGSAFETY_WINDOWLOCK_REARLEFT)
						& isValid(DRIVINGSAFETY_WINDOWLOCK_REARRIGHT));

		}
		else if (str.compare(DRIVINGSAFETY_OBSTACLEDISTANCE) == 0)
		{
			setValidity(DRIVINGSAFETY_OBSTACLEDISTANCE, isValid(DRIVINGSAFETY_OBSTACLEDISTANCE_SENSORSTATUS)
						& isValid(DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTCENTER)
						& isValid(DRIVINGSAFETY_OBSTACLEDISTANCE_REARCENTER)
						& isValid(DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTLEFT)
						& isValid(DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTRIGHT)
						& isValid(DRIVINGSAFETY_OBSTACLEDISTANCE_MIDDLELEFT)
						& isValid(DRIVINGSAFETY_OBSTACLEDISTANCE_MIDDLERIGHT)
						& isValid(DRIVINGSAFETY_OBSTACLEDISTANCE_REARLEFT)
						& isValid(DRIVINGSAFETY_OBSTACLEDISTANCE_REARRIGHT));

		}
		else if (str.compare(DRIVINGSAFETY_FRONTCOLLISIONDETECTION) == 0)
		{
			setValidity(DRIVINGSAFETY_FRONTCOLLISIONDETECTION, isValid(DRIVINGSAFETY_FRONTCOLLISIONDETECTION_STATUS)
						& isValid(DRIVINGSAFETY_FRONTCOLLISIONDETECTION_DISTANCE)
						& isValid(DRIVINGSAFETY_FRONTCOLLISIONDETECTION_TIME));

		}
	}
}

VehicleEventType DrivingSafetyEvent::getType()
{
	return type;
}

unsigned short DrivingSafetyEvent::getAntilockBrakingSystem()
{
	return antilockBrakingSystem;
}

unsigned short DrivingSafetyEvent::getTractionControlSystem()
{
	return tractionControlSystem;
}

unsigned short DrivingSafetyEvent::getElectronicStabilityControl()
{
	return electronicStabilityControl;
}

unsigned short DrivingSafetyEvent::getVehicleTopSpeedLimit()
{
	return vehicleTopSpeedLimit;
}

unsigned short DrivingSafetyEvent::getAirbagStatusDriver()
{
	return airbagStatusDriver;
}

unsigned short DrivingSafetyEvent::getAirbagStatusPassenger()
{
	return airbagStatusPassenger;
}

unsigned short DrivingSafetyEvent::getAirbagStatusSide()
{
	return airbagStatusSide;
}

unsigned short DrivingSafetyEvent::getDoorOpenStatusDriver()
{
	return doorOpenStatusDriver;
}

unsigned short DrivingSafetyEvent::getDoorOpenStatusPassenger()
{
	return doorOpenStatusPassenger;
}

unsigned short DrivingSafetyEvent::getDoorOpenStatusRearLeft()
{
	return doorOpenStatusRearLeft;
}

unsigned short DrivingSafetyEvent::getDoorOpenStatusRearRight()
{
	return doorOpenStatusRearRight;
}

unsigned short DrivingSafetyEvent::getDoorOpenStatusTrunk()
{
	return doorOpenStatusTrunk;
}

unsigned short DrivingSafetyEvent::getDoorOpenStatusFuelFilterCap()
{
	return doorOpenStatusFuelFilterCap;
}

unsigned short DrivingSafetyEvent::getDoorOpenStatusHood()
{
	return doorOpenStatusHood;
}

bool DrivingSafetyEvent::getDoorLockStatusDriver()
{
	return doorLockStatusDriver;
}

bool DrivingSafetyEvent::getDoorLockStatusPassenger()
{
	return doorLockStatusPassenger;
}

bool DrivingSafetyEvent::getDoorLockStatusRearLeft()
{
	return doorLockStatusRearLeft;
}

bool DrivingSafetyEvent::getDoorLockStatusRearRight()
{
	return doorLockStatusRearRight;
}

bool DrivingSafetyEvent::getChildSafetyLock()
{
	return childSafetyLock;
}

unsigned short DrivingSafetyEvent::getOccupantsStatusDriver()
{
	return occupantsStatusDriver;
}

unsigned short DrivingSafetyEvent::getOccupantsStatusPassenger()
{
	return occupantsStatusPassenger;
}

unsigned short DrivingSafetyEvent::getOccupantsStatusRearLeft()
{
	return occupantsStatusRearLeft;
}

unsigned short DrivingSafetyEvent::getOccupantsStatusRearRight()
{
	return occupantsStatusRearRight;
}

bool DrivingSafetyEvent::getSeatBeltDriver()
{
	return seatBeltDriver;
}

bool DrivingSafetyEvent::getSeatBeltPassenger()
{
	return seatBeltPassenger;
}

bool DrivingSafetyEvent::getSeatBeltRearLeft()
{
	return seatBeltRearLeft;
}

bool DrivingSafetyEvent::getSeatBeltRearRight()
{
	return seatBeltRearRight;
}

bool DrivingSafetyEvent::getWindowLockDriver()
{
	return windowLockDriver;
}

bool DrivingSafetyEvent::getWindowLockPassenger()
{
	return windowLockPassenger;
}

bool DrivingSafetyEvent::getWindowLockRearLeft()
{
	return windowLockRearLeft;
}

bool DrivingSafetyEvent::getWindowLockRearRight()
{
	return windowLockRearRight;
}

bool DrivingSafetyEvent::getObstacleDistanceSensorStatus()
{
	return obstacleDistanceSensorStatus;
}

unsigned short DrivingSafetyEvent::getObstacleDistanceFrontCenter()
{
	return obstacleDistanceFrontCenter;
}

unsigned short DrivingSafetyEvent::getObstacleDistanceRearCenter()
{
	return obstacleDistanceRearCenter;
}

unsigned short DrivingSafetyEvent::getObstacleDistanceFrontLeft()
{
	return obstacleDistanceFrontLeft;
}

unsigned short DrivingSafetyEvent::getObstacleDistanceFrontRight()
{
	return obstacleDistanceFrontRight;
}

unsigned short DrivingSafetyEvent::getObstacleDistanceMiddleLeft()
{
	return obstacleDistanceMiddleLeft;
}

unsigned short DrivingSafetyEvent::getObstacleDistanceMiddleRight()
{
	return obstacleDistanceMiddleRight;
}

unsigned short DrivingSafetyEvent::getObstacleDistanceRearLeft()
{
	return obstacleDistanceRearLeft;
}

unsigned short DrivingSafetyEvent::getObstacleDistanceRearRight()
{
	return obstacleDistanceRearRight;
}

bool DrivingSafetyEvent::getFrontCollisionDetectionStatus()
{
	return frontCollisionDetectionStatus;
}

unsigned long DrivingSafetyEvent::getFrontCollisionDetectionDistance()
{
	return frontCollisionDetectionDistance;
}

unsigned long DrivingSafetyEvent::getFrontCollisionDetectionTime()
{
	return frontCollisionDetectionTime;
}

void DrivingSafetyEvent::setType(VehicleEventType type)
{
	this->type = type;
}

void DrivingSafetyEvent::setAntilockBrakingSystem(unsigned short antilockBrakingSystem)
{
	this->antilockBrakingSystem = antilockBrakingSystem;
}

void DrivingSafetyEvent::setTractionControlSystem(unsigned short tractionControlSystem)
{
	this->tractionControlSystem = tractionControlSystem;
}

void DrivingSafetyEvent::setElectronicStabilityControl(unsigned short electronicStabilityControl)
{
	this->electronicStabilityControl = electronicStabilityControl;
}

void DrivingSafetyEvent::setVehicleTopSpeedLimit(unsigned short vehicleTopSpeedLimit)
{
	this->vehicleTopSpeedLimit = vehicleTopSpeedLimit;
}

void DrivingSafetyEvent::setAirbagStatusDriver(unsigned short airbagStatusDriver)
{
	this->airbagStatusDriver = airbagStatusDriver;
}

void DrivingSafetyEvent::setAirbagStatusPassenger(unsigned short airbagStatusPassenger)
{
	this->airbagStatusPassenger = airbagStatusPassenger;
}

void DrivingSafetyEvent::setAirbagStatusSide(unsigned short airbagStatusSide)
{
	this->airbagStatusSide = airbagStatusSide;
}

void DrivingSafetyEvent::setDoorOpenStatusDriver(unsigned short doorOpenStatusDriver)
{
	this->doorOpenStatusDriver = doorOpenStatusDriver;
}

void DrivingSafetyEvent::setDoorOpenStatusPassenger(unsigned short doorOpenStatusPassenger)
{
	this->doorOpenStatusPassenger = doorOpenStatusPassenger;
}

void DrivingSafetyEvent::setDoorOpenStatusRearLeft(unsigned short doorOpenStatusRearLeft)
{
	this->doorOpenStatusRearLeft = doorOpenStatusRearLeft;
}

void DrivingSafetyEvent::setDoorOpenStatusRearRight(unsigned short doorOpenStatusRearRight)
{
	this->doorOpenStatusRearRight = doorOpenStatusRearRight;
}

void DrivingSafetyEvent::setDoorOpenStatusTrunk(unsigned short doorOpenStatusTrunk)
{
	this->doorOpenStatusTrunk = doorOpenStatusTrunk;
}

void DrivingSafetyEvent::setDoorOpenStatusFuelFilterCap(unsigned short doorOpenStatusFuelFilterCap)
{
	this->doorOpenStatusFuelFilterCap = doorOpenStatusFuelFilterCap;
}

void DrivingSafetyEvent::setDoorOpenStatusHood(unsigned short doorOpenStatusHood)
{
	this->doorOpenStatusHood = doorOpenStatusHood;
}

void DrivingSafetyEvent::setDoorLockStatusDriver(bool doorLockStatusDriver)
{
	this->doorLockStatusDriver = doorLockStatusDriver;
}

void DrivingSafetyEvent::setDoorLockStatusPassenger(bool doorLockStatusPassenger)
{
	this->doorLockStatusPassenger = doorLockStatusPassenger;
}

void DrivingSafetyEvent::setDoorLockStatusRearLeft(bool doorLockStatusRearLeft)
{
	this->doorLockStatusRearLeft = doorLockStatusRearLeft;
}

void DrivingSafetyEvent::setDoorLockStatusRearRight(bool doorLockStatusRearRight)
{
	this->doorLockStatusRearRight = doorLockStatusRearRight;
}

void DrivingSafetyEvent::setChildSafetyLock(bool childSafetyLock)
{
	this->childSafetyLock = childSafetyLock;
}

void DrivingSafetyEvent::setOccupantsStatusDriver(unsigned short occupantsStatusDriver)
{
	this->occupantsStatusDriver = occupantsStatusDriver;
}

void DrivingSafetyEvent::setOccupantsStatusPassenger(unsigned short occupantsStatusPassenger)
{
	this->occupantsStatusPassenger = occupantsStatusPassenger;
}

void DrivingSafetyEvent::setOccupantsStatusRearLeft(unsigned short occupantsStatusRearLeft)
{
	this->occupantsStatusRearLeft = occupantsStatusRearLeft;
}

void DrivingSafetyEvent::setOccupantsStatusRearRight(unsigned short occupantsStatusRearRight)
{
	this->occupantsStatusRearRight = occupantsStatusRearRight;
}

void DrivingSafetyEvent::setSeatBeltDriver(bool seatBeltDriver)
{
	this->seatBeltDriver = seatBeltDriver;
}

void DrivingSafetyEvent::setSeatBeltPassenger(bool seatBeltPassenger)
{
	this->seatBeltPassenger = seatBeltPassenger;
}

void DrivingSafetyEvent::setSeatBeltRearLeft(bool seatBeltRearLeft)
{
	this->seatBeltRearLeft = seatBeltRearLeft;
}

void DrivingSafetyEvent::setSeatBeltRearRight(bool seatBeltRearRight)
{
	this->seatBeltRearRight = seatBeltRearRight;
}

void DrivingSafetyEvent::setWindowLockDriver(bool windowLockDriver)
{
	this->windowLockDriver = windowLockDriver;
}

void DrivingSafetyEvent::setWindowLockPassenger(bool windowLockPassenger)
{
	this->windowLockPassenger = windowLockPassenger;
}

void DrivingSafetyEvent::setWindowLockRearLeft(bool windowLockRearLeft)
{
	this->windowLockRearLeft = windowLockRearLeft;
}

void DrivingSafetyEvent::setWindowLockRearRight(bool windowLockRearRight)
{
	this->windowLockRearRight = windowLockRearRight;
}

void DrivingSafetyEvent::setObstacleDistanceSensorStatus(bool obstacleDistanceSensorStatus)
{
	this->obstacleDistanceSensorStatus = obstacleDistanceSensorStatus;
}

void DrivingSafetyEvent::setObstacleDistanceFrontCenter(unsigned short obstacleDistanceFrontCenter)
{
	this->obstacleDistanceFrontCenter = obstacleDistanceFrontCenter;
}

void DrivingSafetyEvent::setObstacleDistanceRearCenter(unsigned short obstacleDistanceRearCenter)
{
	this->obstacleDistanceRearCenter = obstacleDistanceRearCenter;
}

void DrivingSafetyEvent::setObstacleDistanceFrontLeft(unsigned short obstacleDistanceFrontLeft)
{
	this->obstacleDistanceFrontLeft = obstacleDistanceFrontLeft;
}

void DrivingSafetyEvent::setObstacleDistanceFrontRight(unsigned short obstacleDistanceFrontRight)
{
	this->obstacleDistanceFrontRight = obstacleDistanceFrontRight;
}

void DrivingSafetyEvent::setObstacleDistanceMiddleLeft(unsigned short obstacleDistanceMiddleLeft)
{
	this->obstacleDistanceMiddleLeft = obstacleDistanceMiddleLeft;
}

void DrivingSafetyEvent::setObstacleDistanceMiddleRight(unsigned short obstacleDistanceMiddleRight)
{
	this->obstacleDistanceMiddleRight = obstacleDistanceMiddleRight;
}

void DrivingSafetyEvent::setObstacleDistanceRearLeft(unsigned short obstacleDistanceRearLeft)
{
	this->obstacleDistanceRearLeft = obstacleDistanceRearLeft;
}

void DrivingSafetyEvent::setObstacleDistanceRearRight(unsigned short obstacleDistanceRearRight)
{
	this->obstacleDistanceRearRight = obstacleDistanceRearRight;
}

void DrivingSafetyEvent::setFrontCollisionDetectionStatus(bool frontCollisionDetectionStatus)
{
	this->frontCollisionDetectionStatus = frontCollisionDetectionStatus;
}

void DrivingSafetyEvent::setFrontCollisionDetectionDistance(unsigned long frontCollisionDetectionDistance)
{
	this->frontCollisionDetectionDistance = frontCollisionDetectionDistance;
}

void DrivingSafetyEvent::setFrontCollisionDetectionTime(unsigned long frontCollisionDetectionTime)
{
	this->frontCollisionDetectionTime = frontCollisionDetectionTime;
}

