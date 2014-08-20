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
 * @file RunningStatusEvent.cpp 
 * For further information, see http://www.genivi.org/
 */

#include "JSObject.h"
#include <boost/assign.hpp>
#include "DOM/Document.h"

#include "RunningStatusEvent.h"

const VehicleEventType RunningStatusEvent::RUNNINGSTATUS = "runningstatus";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_VEHICLEPOWERMODE = "runningstatus_vehiclepowermode";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_SPEEDOMETER = "runningstatus_speedometer";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_ENGINESPEED = "runningstatus_enginespeed";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_TRIPMETER = "runningstatus_tripmeter";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_TRIPMETER_1 = "runningstatus_tripmeter_1";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_TRIPMETER_2 = "runningstatus_tripmeter_2";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_TRIPMETER_1_MILEAGE = "runningstatus_tripmeter_1_mileage";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_TRIPMETER_2_MILEAGE = "runningstatus_tripmeter_2_mileage";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_TRIPMETER_1_AVERAGESPEED = "runningstatus_tripmeter_1_averagespeed";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_TRIPMETER_2_AVERAGESPEED = "runningstatus_tripmeter_2_averagespeed";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_TRIPMETER_1_FUELCONSUMPTION = "runningstatus_tripmeter_1_fuelconsumption";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_TRIPMETER_2_FUELCONSUMPTION = "runningstatus_tripmeter_2_fuelconsumption";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_TRANSMISSIONGEARSTATUS = "runningstatus_transmissiongearstatus";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_CRUISECONTROL = "runningstatus_cruisecontrol";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_CRUISECONTROL_STATUS = "runningstatus_cruisecontrol_status";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_CRUISECONTROL_SPEED = "runningstatus_cruisecontrol_speed";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_WHEELBRAKE = "runningstatus_wheelbrake";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_LIGHTSSTATUS = "runningstatus_lightsstatus";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_LIGHTSSTATUS_HEAD = "runningstatus_lightsstatus_head";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_LIGHTSSTATUS_HIGHBEAM = "runningstatus_lightsstatus_highbeam";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_LIGHTSSTATUS_TURNLEFT = "runningstatus_lightsstatus_turnleft";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_LIGHTSSTATUS_TURNRIGHT = "runningstatus_lightsstatus_turnright";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_LIGHTSSTATUS_BRAKE = "runningstatus_lightsstatus_brake";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_LIGHTSSTATUS_FOGFRONT = "runningstatus_lightsstatus_fogfront";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_LIGHTSSTATUS_FOGREAR = "runningstatus_lightsstatus_fogrear";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_LIGHTSSTATUS_HAZARD = "runningstatus_lightsstatus_hazard";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_LIGHTSSTATUS_PARKING = "runningstatus_lightsstatus_parking";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_INTERIORLIGHTSSTATUS = "runningstatus_interiorlightsstatus";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_INTERIORLIGHTSSTATUS_DRIVER = "runningstatus_interiorlightsstatus_driver";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_INTERIORLIGHTSSTATUS_PASSENGER = "runningstatus_interiorlightsstatus_passenger";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_INTERIORLIGHTSSTATUS_CENTER = "runningstatus_interiorlightsstatus_center";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_AUTOMATICHEADLIGHTS = "runningstatus_automaticheadlights";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_DYNAMICHIGHBEAM = "runningstatus_dynamichighbeam";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_HORN = "runningstatus_horn";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_CHIME = "runningstatus_chime";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_FUEL = "runningstatus_fuel";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_ESTIMATEDRANGE = "runningstatus_estimatedrange";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_ENGINEOIL = "runningstatus_engineoil";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_ENGINEOIL_REMAINING = "runningstatus_engineoil_remaining";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_ENGINEOIL_CHANGE = "runningstatus_engineoil_change";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_ENGINEOIL_TEMP = "runningstatus_engineoil_temp";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_ENGINECOOLANT = "runningstatus_enginecoolant";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_ENGINECOOLANT_LEVEL = "runningstatus_enginecoolant_level";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_ENGINECOOLANT_TEMP = "runningstatus_enginecoolant_temp";
const VehicleEventType RunningStatusEvent::RUNNINGSTATUS_STEERINGWHEELANGLE = "runningstatus_steeringwheelangle";

RunningStatusEvent::RunningStatusEvent(const FB::BrowserHostPtr& host)
{
	registerProperty("type",
					 make_property(this,
								   &RunningStatusEvent::getType));
	registerProperty("vehiclePowerMode",
					 make_property(this,
								   &RunningStatusEvent::getVehiclePowerMode));
	registerProperty("speedometer",
					 make_property(this,
								   &RunningStatusEvent::getSpeedometer));
	registerProperty("engineSpeed",
					 make_property(this,
								   &RunningStatusEvent::getEngineSpeed));
	registerProperty("tripMeter1Mileage",
					 make_property(this,
								   &RunningStatusEvent::getTripMeter1Mileage));
	registerProperty("tripMeter2Mileage",
					 make_property(this,
								   &RunningStatusEvent::getTripMeter2Mileage));
	registerProperty("tripMeter1AverageSpeed",
					 make_property(this,
								   &RunningStatusEvent::getTripMeter1AverageSpeed));
	registerProperty("tripMeter2AverageSpeed",
					 make_property(this,
								   &RunningStatusEvent::getTripMeter2AverageSpeed));
	registerProperty("tripMeter1FuelConsumption",
					 make_property(this,
								   &RunningStatusEvent::getTripMeter1FuelConsumption));
	registerProperty("tripMeter2FuelConsumption",
					 make_property(this,
								   &RunningStatusEvent::getTripMeter2FuelConsumption));
	registerProperty("transmissionGearStatus",
					 make_property(this,
								   &RunningStatusEvent::getTransmissionGearStatus));
	registerProperty("cruiseControlStatus",
					 make_property(this,
								   &RunningStatusEvent::getCruiseControlStatus));
	registerProperty("cruiseControlSpeed",
					 make_property(this,
								   &RunningStatusEvent::getCruiseControlSpeed));
	registerProperty("wheelBrake",
					 make_property(this,
								   &RunningStatusEvent::getWheelBrake));
	registerProperty("lightsStatusHead",
					 make_property(this,
								   &RunningStatusEvent::getLightsStatusHead));
	registerProperty("lightsStatusHighBeam",
					 make_property(this,
								   &RunningStatusEvent::getLightsStatusHighBeam));
	registerProperty("lightsStatusTurnLeft",
					 make_property(this,
								   &RunningStatusEvent::getLightsStatusTurnLeft));
	registerProperty("lightsStatusTurnRight",
					 make_property(this,
								   &RunningStatusEvent::getLightsStatusTurnRight));
	registerProperty("lightsStatusBrake",
					 make_property(this,
								   &RunningStatusEvent::getLightsStatusBrake));
	registerProperty("lightsStatusFogFront",
					 make_property(this,
								   &RunningStatusEvent::getLightsStatusFogFront));
	registerProperty("lightsStatusFogRear",
					 make_property(this,
								   &RunningStatusEvent::getLightsStatusFogRear));
	registerProperty("lightsStatusHazard",
					 make_property(this,
								   &RunningStatusEvent::getLightsStatusHazard));
	registerProperty("lightsStatusParking",
					 make_property(this,
								   &RunningStatusEvent::getLightsStatusParking));
	registerProperty("interiorLightsStatusDriver",
					 make_property(this,
								   &RunningStatusEvent::getInteriorLightsStatusDriver));
	registerProperty("interiorLightsStatusPassenger",
					 make_property(this,
								   &RunningStatusEvent::getInteriorLightsStatusPassenger));
	registerProperty("interiorLightsStatusCenter",
					 make_property(this,
								   &RunningStatusEvent::getInteriorLightsStatusCenter));
	registerProperty("automaticHeadlights",
					 make_property(this,
								   &RunningStatusEvent::getAutomaticHeadlights));
	registerProperty("dynamicHighBeam",
					 make_property(this,
								   &RunningStatusEvent::getDynamicHighBeam));
	registerProperty("horn",
					 make_property(this,
								   &RunningStatusEvent::getHorn));
	registerProperty("chime",
					 make_property(this,
								   &RunningStatusEvent::getChime));
	registerProperty("fuel",
					 make_property(this,
								   &RunningStatusEvent::getFuel));
	registerProperty("estimatedRange",
					 make_property(this,
								   &RunningStatusEvent::getEstimatedRange));
	registerProperty("engineOilRemaining",
					 make_property(this,
								   &RunningStatusEvent::getEngineOilRemaining));
	registerProperty("engineOilChange",
					 make_property(this,
								   &RunningStatusEvent::getEngineOilChange));
	registerProperty("engineOilTemp",
					 make_property(this,
								   &RunningStatusEvent::getEngineOilTemp));
	registerProperty("engineCoolantLevel",
					 make_property(this,
								   &RunningStatusEvent::getEngineCoolantLevel));
	registerProperty("engineCoolantTemp",
					 make_property(this,
								   &RunningStatusEvent::getEngineCoolantTemp));
	registerProperty("steeringWheelAngle",
					 make_property(this,
								   &RunningStatusEvent::getSteeringWheelAngle));

	allSupportedEventTypesR.push_back(RUNNINGSTATUS);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_VEHICLEPOWERMODE);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_SPEEDOMETER);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_ENGINESPEED);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_TRIPMETER);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_TRIPMETER_1);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_TRIPMETER_2);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_TRIPMETER_1_MILEAGE);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_TRIPMETER_2_MILEAGE);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_TRIPMETER_1_AVERAGESPEED);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_TRIPMETER_2_AVERAGESPEED);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_TRIPMETER_1_FUELCONSUMPTION);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_TRIPMETER_2_FUELCONSUMPTION);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_TRANSMISSIONGEARSTATUS);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_CRUISECONTROL);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_CRUISECONTROL_STATUS);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_CRUISECONTROL_SPEED);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_WHEELBRAKE);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_LIGHTSSTATUS);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_LIGHTSSTATUS_HEAD);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_LIGHTSSTATUS_HIGHBEAM);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_LIGHTSSTATUS_TURNLEFT);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_LIGHTSSTATUS_TURNRIGHT);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_LIGHTSSTATUS_BRAKE);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_LIGHTSSTATUS_FOGFRONT);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_LIGHTSSTATUS_FOGREAR);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_LIGHTSSTATUS_HAZARD);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_LIGHTSSTATUS_PARKING);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_INTERIORLIGHTSSTATUS);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_INTERIORLIGHTSSTATUS_DRIVER);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_INTERIORLIGHTSSTATUS_PASSENGER);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_INTERIORLIGHTSSTATUS_CENTER);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_AUTOMATICHEADLIGHTS);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_DYNAMICHIGHBEAM);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_HORN);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_CHIME);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_FUEL);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_ESTIMATEDRANGE);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_ENGINEOIL);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_ENGINEOIL_REMAINING);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_ENGINEOIL_CHANGE);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_ENGINEOIL_TEMP);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_ENGINECOOLANT);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_ENGINECOOLANT_LEVEL);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_ENGINECOOLANT_TEMP);
	allSupportedEventTypesR.push_back(RUNNINGSTATUS_STEERINGWHEELANGLE);

	validity[RUNNINGSTATUS] = false;
	validity[RUNNINGSTATUS_VEHICLEPOWERMODE] = false;
	validity[RUNNINGSTATUS_SPEEDOMETER] = false;
	validity[RUNNINGSTATUS_ENGINESPEED] = false;
	validity[RUNNINGSTATUS_TRIPMETER] = false;
	validity[RUNNINGSTATUS_TRIPMETER_1] = false;
	validity[RUNNINGSTATUS_TRIPMETER_2] = false;
	validity[RUNNINGSTATUS_TRIPMETER_1_MILEAGE] = false;
	validity[RUNNINGSTATUS_TRIPMETER_2_MILEAGE] = false;
	validity[RUNNINGSTATUS_TRIPMETER_1_AVERAGESPEED] = false;
	validity[RUNNINGSTATUS_TRIPMETER_2_AVERAGESPEED] = false;
	validity[RUNNINGSTATUS_TRIPMETER_1_FUELCONSUMPTION] = false;
	validity[RUNNINGSTATUS_TRIPMETER_2_FUELCONSUMPTION] = false;
	validity[RUNNINGSTATUS_TRANSMISSIONGEARSTATUS] = false;
	validity[RUNNINGSTATUS_CRUISECONTROL] = false;
	validity[RUNNINGSTATUS_CRUISECONTROL_STATUS] = false;
	validity[RUNNINGSTATUS_CRUISECONTROL_SPEED] = false;
	validity[RUNNINGSTATUS_WHEELBRAKE] = false;
	validity[RUNNINGSTATUS_LIGHTSSTATUS] = false;
	validity[RUNNINGSTATUS_LIGHTSSTATUS_HEAD] = false;
	validity[RUNNINGSTATUS_LIGHTSSTATUS_HIGHBEAM] = false;
	validity[RUNNINGSTATUS_LIGHTSSTATUS_TURNLEFT] = false;
	validity[RUNNINGSTATUS_LIGHTSSTATUS_TURNRIGHT] = false;
	validity[RUNNINGSTATUS_LIGHTSSTATUS_BRAKE] = false;
	validity[RUNNINGSTATUS_LIGHTSSTATUS_FOGFRONT] = false;
	validity[RUNNINGSTATUS_LIGHTSSTATUS_FOGREAR] = false;
	validity[RUNNINGSTATUS_LIGHTSSTATUS_HAZARD] = false;
	validity[RUNNINGSTATUS_LIGHTSSTATUS_PARKING] = false;
	validity[RUNNINGSTATUS_INTERIORLIGHTSSTATUS] = false;
	validity[RUNNINGSTATUS_INTERIORLIGHTSSTATUS_DRIVER] = false;
	validity[RUNNINGSTATUS_INTERIORLIGHTSSTATUS_PASSENGER] = false;
	validity[RUNNINGSTATUS_INTERIORLIGHTSSTATUS_CENTER] = false;
	validity[RUNNINGSTATUS_AUTOMATICHEADLIGHTS] = false;
	validity[RUNNINGSTATUS_DYNAMICHIGHBEAM] = false;
	validity[RUNNINGSTATUS_HORN] = false;
	validity[RUNNINGSTATUS_CHIME] = false;
	validity[RUNNINGSTATUS_FUEL] = false;
	validity[RUNNINGSTATUS_ESTIMATEDRANGE] = false;
	validity[RUNNINGSTATUS_ENGINEOIL] = false;
	validity[RUNNINGSTATUS_ENGINEOIL_REMAINING] = false;
	validity[RUNNINGSTATUS_ENGINEOIL_CHANGE] = false;
	validity[RUNNINGSTATUS_ENGINEOIL_TEMP] = false;
	validity[RUNNINGSTATUS_ENGINECOOLANT] = false;
	validity[RUNNINGSTATUS_ENGINECOOLANT_LEVEL] = false;
	validity[RUNNINGSTATUS_ENGINECOOLANT_TEMP] = false;
	validity[RUNNINGSTATUS_STEERINGWHEELANGLE] = false;
}

RunningStatusEvent::~RunningStatusEvent()
{

}

void RunningStatusEvent::updateValidity(VehicleEventType type)
{
	std::string str = type;
	size_t pos;

	while ((pos = str.find_last_of('_')) != std::string::npos)
	{
		str = str.substr(0, pos);

		if (str.compare(RUNNINGSTATUS) == 0)
		{
			setValidity(RUNNINGSTATUS,	isValid(RUNNINGSTATUS_VEHICLEPOWERMODE)
						& isValid(RUNNINGSTATUS_SPEEDOMETER)
						& isValid(RUNNINGSTATUS_ENGINESPEED)
						& isValid(RUNNINGSTATUS_TRIPMETER)
						& isValid(RUNNINGSTATUS_TRANSMISSIONGEARSTATUS)
						& isValid(RUNNINGSTATUS_CRUISECONTROL)
						& isValid(RUNNINGSTATUS_WHEELBRAKE)
						& isValid(RUNNINGSTATUS_LIGHTSSTATUS)
						& isValid(RUNNINGSTATUS_INTERIORLIGHTSSTATUS)
						& isValid(RUNNINGSTATUS_AUTOMATICHEADLIGHTS)
						& isValid(RUNNINGSTATUS_DYNAMICHIGHBEAM)
						& isValid(RUNNINGSTATUS_HORN)
						& isValid(RUNNINGSTATUS_CHIME)
						& isValid(RUNNINGSTATUS_FUEL)
						& isValid(RUNNINGSTATUS_ESTIMATEDRANGE)
						& isValid(RUNNINGSTATUS_ENGINEOIL)
						& isValid(RUNNINGSTATUS_ENGINECOOLANT)
						& isValid(RUNNINGSTATUS_STEERINGWHEELANGLE));
		}
		else if (str.compare(RUNNINGSTATUS_TRIPMETER) == 0)
		{
			setValidity(RUNNINGSTATUS_TRIPMETER, isValid(RUNNINGSTATUS_TRIPMETER_1)
						& isValid(RUNNINGSTATUS_TRIPMETER_2));
		}
		else if (str.compare(RUNNINGSTATUS_TRIPMETER_1) == 0)
		{
			setValidity(RUNNINGSTATUS_TRIPMETER_1,
						isValid(RUNNINGSTATUS_TRIPMETER_1_MILEAGE)
						& isValid(RUNNINGSTATUS_TRIPMETER_1_AVERAGESPEED)
						& isValid(RUNNINGSTATUS_TRIPMETER_1_FUELCONSUMPTION));
		}
		else if (str.compare(RUNNINGSTATUS_TRIPMETER_2) == 0)
		{
			setValidity(RUNNINGSTATUS_TRIPMETER_2,
						isValid(RUNNINGSTATUS_TRIPMETER_2_MILEAGE)
						& isValid(RUNNINGSTATUS_TRIPMETER_2_AVERAGESPEED)
						& isValid(RUNNINGSTATUS_TRIPMETER_2_FUELCONSUMPTION));
		}
		else if (str.compare(RUNNINGSTATUS_CRUISECONTROL) == 0)
		{
			setValidity(RUNNINGSTATUS_CRUISECONTROL, isValid(RUNNINGSTATUS_CRUISECONTROL_STATUS)
						& isValid(RUNNINGSTATUS_CRUISECONTROL_SPEED));
		}
		else if (str.compare(RUNNINGSTATUS_LIGHTSSTATUS) == 0)
		{
			setValidity(RUNNINGSTATUS_LIGHTSSTATUS, isValid(RUNNINGSTATUS_LIGHTSSTATUS_HEAD)
						& isValid(RUNNINGSTATUS_LIGHTSSTATUS_HIGHBEAM)
						& isValid(RUNNINGSTATUS_LIGHTSSTATUS_TURNLEFT)
						& isValid(RUNNINGSTATUS_LIGHTSSTATUS_TURNRIGHT)
						& isValid(RUNNINGSTATUS_LIGHTSSTATUS_BRAKE)
						& isValid(RUNNINGSTATUS_LIGHTSSTATUS_FOGFRONT)
						& isValid(RUNNINGSTATUS_LIGHTSSTATUS_FOGREAR)
						& isValid(RUNNINGSTATUS_LIGHTSSTATUS_HAZARD)
						& isValid(RUNNINGSTATUS_LIGHTSSTATUS_PARKING));
		}
		else if (str.compare(RUNNINGSTATUS_INTERIORLIGHTSSTATUS) == 0)
		{
			setValidity(RUNNINGSTATUS_INTERIORLIGHTSSTATUS, isValid(RUNNINGSTATUS_INTERIORLIGHTSSTATUS_DRIVER)
						& isValid(RUNNINGSTATUS_INTERIORLIGHTSSTATUS_PASSENGER)
						& isValid(RUNNINGSTATUS_INTERIORLIGHTSSTATUS_CENTER));
		}
		else if (str.compare(RUNNINGSTATUS_ENGINEOIL) == 0)
		{
			setValidity(RUNNINGSTATUS_ENGINEOIL, isValid(RUNNINGSTATUS_ENGINEOIL_REMAINING)
						& isValid(RUNNINGSTATUS_ENGINEOIL_CHANGE)
						& isValid(RUNNINGSTATUS_ENGINEOIL_TEMP));
		}
		else if (str.compare(RUNNINGSTATUS_ENGINECOOLANT) == 0)
		{
			setValidity(RUNNINGSTATUS_ENGINECOOLANT, isValid(RUNNINGSTATUS_ENGINECOOLANT_LEVEL)
						& isValid(RUNNINGSTATUS_ENGINECOOLANT_TEMP));
		}
	}
}

VehicleEventType RunningStatusEvent::getType()
{
	return type;
}


unsigned short RunningStatusEvent::getVehiclePowerMode()
{
	return vehiclePowerMode;
}

unsigned short RunningStatusEvent::getSpeedometer()
{
	return speedometer;
}

unsigned short RunningStatusEvent::getEngineSpeed()
{
	return engineSpeed;
}

unsigned long RunningStatusEvent::getTripMeter1Mileage()
{
	return tripMeter1Mileage;
}

unsigned long RunningStatusEvent::getTripMeter2Mileage()
{
	return tripMeter2Mileage;
}

unsigned short RunningStatusEvent::getTripMeter1AverageSpeed()
{
	return tripMeter1AverageSpeed;
}

unsigned short RunningStatusEvent::getTripMeter2AverageSpeed()
{
	return tripMeter2AverageSpeed;
}

unsigned long RunningStatusEvent::getTripMeter1FuelConsumption()
{
	return tripMeter1FuelConsumption;
}

unsigned long RunningStatusEvent::getTripMeter2FuelConsumption()
{
	return tripMeter2FuelConsumption;
}

unsigned short RunningStatusEvent::getTransmissionGearStatus()
{
	return transmissionGearStatus;
}

bool RunningStatusEvent::getCruiseControlStatus()
{
	return cruiseControlStatus;
}

unsigned short RunningStatusEvent::getCruiseControlSpeed()
{
	return cruiseControlSpeed;
}

unsigned short RunningStatusEvent::getWheelBrake()
{
	return wheelBrake;
}

bool RunningStatusEvent::getLightsStatusHead()
{
	return lightsStatusHead;
}

bool RunningStatusEvent::getLightsStatusHighBeam()
{
	return lightsStatusHighBeam;
}

bool RunningStatusEvent::getLightsStatusTurnLeft()
{
	return lightsStatusTurnLeft;
}

bool RunningStatusEvent::getLightsStatusTurnRight()
{
	return lightsStatusTurnRight;
}

bool RunningStatusEvent::getLightsStatusBrake()
{
	return lightsStatusBrake;
}

bool RunningStatusEvent::getLightsStatusFogFront()
{
	return lightsStatusFogFront;
}

bool RunningStatusEvent::getLightsStatusFogRear()
{
	return lightsStatusFogRear;
}

bool RunningStatusEvent::getLightsStatusHazard()
{
	return lightsStatusHazard;
}

bool RunningStatusEvent::getLightsStatusParking()
{
	return lightsStatusParking;
}

bool RunningStatusEvent::getInteriorLightsStatusDriver()
{
	return interiorLightsStatusDriver;
}

bool RunningStatusEvent::getInteriorLightsStatusPassenger()
{
	return interiorLightsStatusPassenger;
}

bool RunningStatusEvent::getInteriorLightsStatusCenter()
{
	return interiorLightsStatusCenter;
}

bool RunningStatusEvent::getAutomaticHeadlights()
{
	return automaticHeadlights;
}

bool RunningStatusEvent::getDynamicHighBeam()
{
	return dynamicHighBeam;
}

bool RunningStatusEvent::getHorn()
{
	return horn;
}

bool RunningStatusEvent::getChime()
{
	return chime;
}

unsigned short RunningStatusEvent::getFuel()
{
	return fuel;
}

unsigned long RunningStatusEvent::getEstimatedRange()
{
	return estimatedRange;
}

unsigned short RunningStatusEvent::getEngineOilRemaining()
{
	return engineOilRemaining;
}

bool RunningStatusEvent::getEngineOilChange()
{
	return engineOilChange;
}

short RunningStatusEvent::getEngineOilTemp()
{
	return engineOilTemp;
}

unsigned short RunningStatusEvent::getEngineCoolantLevel()
{
	return engineCoolantLevel;
}

short RunningStatusEvent::getEngineCoolantTemp()
{
	return engineCoolantTemp;
}

short RunningStatusEvent::getSteeringWheelAngle()
{
	return steeringWheelAngle;
}

void RunningStatusEvent::setType(VehicleEventType type)
{
	this->type = type;
}

void RunningStatusEvent::setVehiclePowerMode(unsigned short vehiclePowerMode)
{
	this->vehiclePowerMode = vehiclePowerMode;
}

void RunningStatusEvent::setSpeedometer(unsigned short speedometer)
{
	this->speedometer = speedometer;
}

void RunningStatusEvent::setEngineSpeed(unsigned short engineSpeed)
{
	this->engineSpeed = engineSpeed;
}

void RunningStatusEvent::setTripMeter1Mileage(unsigned long tripMeter1Mileage)
{
	this->tripMeter1Mileage = tripMeter1Mileage;
}

void RunningStatusEvent::setTripMeter2Mileage(unsigned long tripMeter2Mileage)
{
	this->tripMeter2Mileage = tripMeter2Mileage;
}

void RunningStatusEvent::setTripMeter1AverageSpeed(unsigned short tripMeter1AverageSpeed)
{
	this->tripMeter1AverageSpeed = tripMeter1AverageSpeed;
}

void RunningStatusEvent::setTripMeter2AverageSpeed(unsigned short tripMeter2AverageSpeed)
{
	this->tripMeter2AverageSpeed = tripMeter2AverageSpeed;
}

void RunningStatusEvent::setTripMeter1FuelConsumption(unsigned long tripMeter1FuelConsumption)
{
	this->tripMeter1FuelConsumption = tripMeter1FuelConsumption;
}

void RunningStatusEvent::setTripMeter2FuelConsumption(unsigned long tripMeter2FuelConsumption)
{
	this->tripMeter2FuelConsumption = tripMeter2FuelConsumption;
}

void RunningStatusEvent::setTransmissionGearStatus(unsigned short transmissionGearStatus)
{
	this->transmissionGearStatus = transmissionGearStatus;
}

void RunningStatusEvent::setCruiseControlStatus(bool cruiseControlStatus)
{
	this->cruiseControlStatus = cruiseControlStatus;
}

void RunningStatusEvent::setCruiseControlSpeed(unsigned short cruiseControlSpeed)
{
	this->cruiseControlSpeed = cruiseControlSpeed;
}

void RunningStatusEvent::setWheelBrake(unsigned short wheelBrake)
{
	this->wheelBrake = wheelBrake;
}

void RunningStatusEvent::setLightsStatusHead(bool lightsStatusHead)
{
	this->lightsStatusHead = lightsStatusHead;
}

void RunningStatusEvent::setLightsStatusHighBeam(bool lightsStatusHighBeam)
{
	this->lightsStatusHighBeam = lightsStatusHighBeam;
}

void RunningStatusEvent::setLightsStatusTurnLeft(bool lightsStatusTurnLeft)
{
	this->lightsStatusTurnLeft = lightsStatusTurnLeft;
}

void RunningStatusEvent::setLightsStatusTurnRight(bool lightsStatusTurnRight)
{
	this->lightsStatusTurnRight = lightsStatusTurnRight;
}

void RunningStatusEvent::setLightsStatusBrake(bool lightsStatusBrake)
{
	this->lightsStatusBrake = lightsStatusBrake;
}

void RunningStatusEvent::setLightsStatusFogFront(bool lightsStatusFogFront)
{
	this->lightsStatusFogFront = lightsStatusFogFront;
}

void RunningStatusEvent::setLightsStatusFogRear(bool lightsStatusFogRear)
{
	this->lightsStatusFogRear = lightsStatusFogRear;
}

void RunningStatusEvent::setLightsStatusHazard(bool lightsStatusHazard)
{
	this->lightsStatusHazard = lightsStatusHazard;
}

void RunningStatusEvent::setLightsStatusParking(bool lightsStatusParking)
{
	this->lightsStatusParking = lightsStatusParking;
}

void RunningStatusEvent::setInteriorLightsStatusDriver(bool interiorLightsStatusDriver)
{
	this->interiorLightsStatusDriver = interiorLightsStatusDriver;
}

void RunningStatusEvent::setInteriorLightsStatusPassenger(bool interiorLightsStatusPassenger)
{
	this->interiorLightsStatusPassenger = interiorLightsStatusPassenger;
}

void RunningStatusEvent::setInteriorLightsStatusCenter(bool interiorLightsStatusCenter)
{
	this->interiorLightsStatusCenter = interiorLightsStatusCenter;
}

void RunningStatusEvent::setAutomaticHeadlights(bool automaticHeadlights)
{
	this->automaticHeadlights = automaticHeadlights;
}

void RunningStatusEvent::setDynamicHighBeam(bool dynamicHighBeam)
{
	this->dynamicHighBeam = dynamicHighBeam;
}

void RunningStatusEvent::setHorn(bool horn)
{
	this->horn = horn;
}

void RunningStatusEvent::setChime(bool chime)
{
	this->chime = chime;
}

void RunningStatusEvent::setFuel(unsigned short fuel)
{
	this->fuel = fuel;
}

void RunningStatusEvent::setEstimatedRange(unsigned long estimatedRange)
{
	this->estimatedRange = estimatedRange;
}

void RunningStatusEvent::setEngineOilRemaining(unsigned short engineOilRemaining)
{
	this->engineOilRemaining = engineOilRemaining;
}

void RunningStatusEvent::setEngineOilChange(bool engineOilChange)
{
	this->engineOilChange = engineOilChange;
}

void RunningStatusEvent::setEngineOilTemp(short engineOilTemp)
{
	this->engineOilTemp = engineOilTemp;
}

void RunningStatusEvent::setEngineCoolantLevel(unsigned short engineCoolantLevel)
{
	this->engineCoolantLevel = engineCoolantLevel;
}

void RunningStatusEvent::setEngineCoolantTemp(short engineCoolantTemp)
{
	this->engineCoolantTemp = engineCoolantTemp;
}

void RunningStatusEvent::setSteeringWheelAngle(short steeringWheelAngle)
{
	this->steeringWheelAngle = steeringWheelAngle;
}

