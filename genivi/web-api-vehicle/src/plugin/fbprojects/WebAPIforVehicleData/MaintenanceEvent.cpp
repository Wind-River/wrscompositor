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
 * @file MaintenanceEvent.cpp 
 * For further information, see http://www.genivi.org/
 */

#include "JSObject.h"
#include <boost/assign.hpp>
#include "DOM/Document.h"

#include "MaintenanceEvent.h"

const VehicleEventType MaintenanceEvent::MAINTENANCE = "maintenance";
const VehicleEventType MaintenanceEvent::MAINTENANCE_ODOMETER = "maintenance_odometer";
const VehicleEventType MaintenanceEvent::MAINTENANCE_TRANSMISSIONOIL = "maintenance_transmissionoil";
const VehicleEventType MaintenanceEvent::MAINTENANCE_TRANSMISSIONOIL_LIFELEVEL = "maintenance_transmissionoil_lifelevel";
const VehicleEventType MaintenanceEvent::MAINTENANCE_TRANSMISSIONOIL_TEMP = "maintenance_transmissionoil_temp";
const VehicleEventType MaintenanceEvent::MAINTENANCE_BRAKEFLUIDLEVEL = "maintenance_brakefluidlevel";
const VehicleEventType MaintenanceEvent::MAINTENANCE_WASHERFLUIDLEVEL = "maintenance_washerfluidlevel";
const VehicleEventType MaintenanceEvent::MAINTENANCE_MALFUNCTIONINDICATORLAMP = "maintenance_malfunctionindicatorlamp";
const VehicleEventType MaintenanceEvent::MAINTENANCE_BATTERY = "maintenance_battery";
const VehicleEventType MaintenanceEvent::MAINTENANCE_BATTERY_VOLTAGE = "maintenance_battery_voltage";
const VehicleEventType MaintenanceEvent::MAINTENANCE_BATTERY_CURRENT = "maintenance_battery_current";
const VehicleEventType MaintenanceEvent::MAINTENANCE_TIREPRESSURE = "maintenance_tirepressure";
const VehicleEventType MaintenanceEvent::MAINTENANCE_TIREPRESSURE_FRONTLEFT = "maintenance_tirepressure_frontleft";
const VehicleEventType MaintenanceEvent::MAINTENANCE_TIREPRESSURE_FRONTRIGHT = "maintenance_tirepressure_frontright";
const VehicleEventType MaintenanceEvent::MAINTENANCE_TIREPRESSURE_REARLEFT = "maintenance_tirepressure_rearleft";
const VehicleEventType MaintenanceEvent::MAINTENANCE_TIREPRESSURE_REARRIGHT = "maintenance_tirepressure_rearright";
const VehicleEventType MaintenanceEvent::MAINTENANCE_TIREPRESSURESTATUS = "maintenance_tirepressurestatus";
const VehicleEventType MaintenanceEvent::MAINTENANCE_TIREPRESSURESTATUS_FRONTLEFT = "maintenance_tirepressurestatus_frontleft";
const VehicleEventType MaintenanceEvent::MAINTENANCE_TIREPRESSURESTATUS_FRONTRIGHT = "maintenance_tirepressurestatus_frontright";
const VehicleEventType MaintenanceEvent::MAINTENANCE_TIREPRESSURESTATUS_REARLEFT = "maintenance_tirepressurestatus_rearleft";
const VehicleEventType MaintenanceEvent::MAINTENANCE_TIREPRESSURESTATUS_REARRIGHT = "maintenance_tirepressurestatus_rearright";

MaintenanceEvent::MaintenanceEvent(const FB::BrowserHostPtr& host)
{
	registerProperty("type",
					 make_property(this,
								   &MaintenanceEvent::getType));
	registerProperty("odometer",
					 make_property(this,
								   &MaintenanceEvent::getOdometer));
	registerProperty("transmissionOilLifeLevel",
					 make_property(this,
								   &MaintenanceEvent::getTransmissionOilLifeLevel));
	registerProperty("transmissionOilTemp",
					 make_property(this,
								   &MaintenanceEvent::getTransmissionOilTemp));
	registerProperty("brakeFluidLevel",
					 make_property(this,
								   &MaintenanceEvent::getBrakeFluidLevel));
	registerProperty("washerFluidLevel",
					 make_property(this,
								   &MaintenanceEvent::getWasherFluidLevel));
	registerProperty("malfunctionIndicatorLamp",
					 make_property(this,
								   &MaintenanceEvent::getMalfunctionIndicatorLamp));
	registerProperty("batteryVoltage",
					 make_property(this,
								   &MaintenanceEvent::getBatteryVoltage));
	registerProperty("batteryCurrent",
					 make_property(this,
								   &MaintenanceEvent::getBatteryCurrent));
	registerProperty("tirePressureFrontLeft",
					 make_property(this,
								   &MaintenanceEvent::getTirePressureFrontLeft));
	registerProperty("tirePressureFrontRight",
					 make_property(this,
								   &MaintenanceEvent::getTirePressureFrontRight));
	registerProperty("tirePressureRearLeft",
					 make_property(this,
								   &MaintenanceEvent::getTirePressureRearLeft));
	registerProperty("tirePressureRearRight",
					 make_property(this,
								   &MaintenanceEvent::getTirePressureRearRight));
	registerProperty("tirePressureStatusFrontLeft",
					 make_property(this,
								   &MaintenanceEvent::getTirePressureStatusFrontLeft));
	registerProperty("tirePressureStatusFrontRight",
					 make_property(this,
								   &MaintenanceEvent::getTirePressureStatusFrontRight));
	registerProperty("tirePressureStatusRearLeft",
					 make_property(this,
								   &MaintenanceEvent::getTirePressureStatusRearLeft));
	registerProperty("tirePressureStatusRearRight",
					 make_property(this,
								   &MaintenanceEvent::getTirePressureStatusRearRight));

	allSupportedEventTypesR.push_back(MAINTENANCE);
	allSupportedEventTypesR.push_back(MAINTENANCE_ODOMETER);
	allSupportedEventTypesR.push_back(MAINTENANCE_TRANSMISSIONOIL);
	allSupportedEventTypesR.push_back(MAINTENANCE_TRANSMISSIONOIL_LIFELEVEL);
	allSupportedEventTypesR.push_back(MAINTENANCE_TRANSMISSIONOIL_TEMP);
	allSupportedEventTypesR.push_back(MAINTENANCE_BRAKEFLUIDLEVEL);
	allSupportedEventTypesR.push_back(MAINTENANCE_WASHERFLUIDLEVEL);
	allSupportedEventTypesR.push_back(MAINTENANCE_MALFUNCTIONINDICATORLAMP);
	allSupportedEventTypesR.push_back(MAINTENANCE_BATTERY);
	allSupportedEventTypesR.push_back(MAINTENANCE_BATTERY_VOLTAGE);
	allSupportedEventTypesR.push_back(MAINTENANCE_BATTERY_CURRENT);
	allSupportedEventTypesR.push_back(MAINTENANCE_TIREPRESSURE);
	allSupportedEventTypesR.push_back(MAINTENANCE_TIREPRESSURE_FRONTLEFT);
	allSupportedEventTypesR.push_back(MAINTENANCE_TIREPRESSURE_FRONTRIGHT);
	allSupportedEventTypesR.push_back(MAINTENANCE_TIREPRESSURE_REARLEFT);
	allSupportedEventTypesR.push_back(MAINTENANCE_TIREPRESSURE_REARRIGHT);
	allSupportedEventTypesR.push_back(MAINTENANCE_TIREPRESSURESTATUS);
	allSupportedEventTypesR.push_back(MAINTENANCE_TIREPRESSURESTATUS_FRONTLEFT);
	allSupportedEventTypesR.push_back(MAINTENANCE_TIREPRESSURESTATUS_FRONTRIGHT);
	allSupportedEventTypesR.push_back(MAINTENANCE_TIREPRESSURESTATUS_REARLEFT);
	allSupportedEventTypesR.push_back(MAINTENANCE_TIREPRESSURESTATUS_REARRIGHT);

	validity[MAINTENANCE] = false;
	validity[MAINTENANCE_ODOMETER] = false;
	validity[MAINTENANCE_TRANSMISSIONOIL] = false;
	validity[MAINTENANCE_TRANSMISSIONOIL_LIFELEVEL] = false;
	validity[MAINTENANCE_TRANSMISSIONOIL_TEMP] = false;
	validity[MAINTENANCE_BRAKEFLUIDLEVEL] = false;
	validity[MAINTENANCE_WASHERFLUIDLEVEL] = false;
	validity[MAINTENANCE_MALFUNCTIONINDICATORLAMP] = false;
	validity[MAINTENANCE_BATTERY] = false;
	validity[MAINTENANCE_BATTERY_VOLTAGE] = false;
	validity[MAINTENANCE_BATTERY_CURRENT] = false;
	validity[MAINTENANCE_TIREPRESSURE] = false;
	validity[MAINTENANCE_TIREPRESSURE_FRONTLEFT] = false;
	validity[MAINTENANCE_TIREPRESSURE_FRONTRIGHT] = false;
	validity[MAINTENANCE_TIREPRESSURE_REARLEFT] = false;
	validity[MAINTENANCE_TIREPRESSURE_REARRIGHT] = false;
	validity[MAINTENANCE_TIREPRESSURESTATUS] = false;
	validity[MAINTENANCE_TIREPRESSURESTATUS_FRONTLEFT] = false;
	validity[MAINTENANCE_TIREPRESSURESTATUS_FRONTRIGHT] = false;
	validity[MAINTENANCE_TIREPRESSURESTATUS_REARLEFT] = false;
	validity[MAINTENANCE_TIREPRESSURESTATUS_REARRIGHT] = false;
}

MaintenanceEvent::~MaintenanceEvent()
{

}

void MaintenanceEvent::updateValidity(VehicleEventType type)
{
	std::string str = type;
	size_t pos;

	while ((pos = str.find_last_of('_')) != std::string::npos)
	{
		str = str.substr(0, pos);

		if (str.compare(MAINTENANCE) == 0)
		{
			setValidity(MAINTENANCE, isValid(MAINTENANCE_ODOMETER)
						& isValid(MAINTENANCE_TRANSMISSIONOIL)
						& isValid(MAINTENANCE_BRAKEFLUIDLEVEL)
						& isValid(MAINTENANCE_WASHERFLUIDLEVEL)
						& isValid(MAINTENANCE_MALFUNCTIONINDICATORLAMP)
						& isValid(MAINTENANCE_BATTERY)
						& isValid(MAINTENANCE_TIREPRESSURE)
						& isValid(MAINTENANCE_TIREPRESSURESTATUS));
		}
		else if (str.compare(MAINTENANCE_TRANSMISSIONOIL) == 0)
		{
			setValidity(MAINTENANCE_TRANSMISSIONOIL, isValid(MAINTENANCE_TRANSMISSIONOIL_LIFELEVEL)
						& isValid(MAINTENANCE_TRANSMISSIONOIL_TEMP));
		}
		else if (str.compare(MAINTENANCE_BATTERY) == 0)
		{
			setValidity(MAINTENANCE_BATTERY, isValid(MAINTENANCE_BATTERY_VOLTAGE)
						& isValid(MAINTENANCE_BATTERY_CURRENT));
		}
		else if (str.compare(MAINTENANCE_TIREPRESSURE) == 0)
		{
			setValidity(MAINTENANCE_TIREPRESSURE, isValid(MAINTENANCE_TIREPRESSURE_FRONTLEFT)
						& isValid(MAINTENANCE_TIREPRESSURE_FRONTRIGHT)
						& isValid(MAINTENANCE_TIREPRESSURE_REARLEFT)
						&isValid(MAINTENANCE_TIREPRESSURE_REARRIGHT));
		}
		else if (str.compare(MAINTENANCE_TIREPRESSURESTATUS) == 0)
		{
			setValidity(MAINTENANCE_TIREPRESSURESTATUS, isValid(MAINTENANCE_TIREPRESSURESTATUS_FRONTLEFT)
						& isValid(MAINTENANCE_TIREPRESSURESTATUS_FRONTRIGHT)
						& isValid(MAINTENANCE_TIREPRESSURESTATUS_REARLEFT)
						& isValid(MAINTENANCE_TIREPRESSURESTATUS_REARRIGHT));
		}
	}
}

VehicleEventType MaintenanceEvent::getType()
{
	return type;
}

unsigned long MaintenanceEvent::getOdometer()
{
	return odometer;
}

bool MaintenanceEvent::getTransmissionOilLifeLevel()
{
	return transmissionOilLifeLevel;
}

short MaintenanceEvent::getTransmissionOilTemp()
{
	return transmissionOilTemp;
}

bool MaintenanceEvent::getBrakeFluidLevel()
{
	return brakeFluidLevel;
}

bool MaintenanceEvent::getWasherFluidLevel()
{
	return washerFluidLevel;
}

bool MaintenanceEvent::getMalfunctionIndicatorLamp()
{
	return malfunctionIndicatorLamp;
}

unsigned short MaintenanceEvent::getBatteryVoltage()
{
	return batteryVoltage;
}

unsigned short MaintenanceEvent::getBatteryCurrent()
{
	return batteryCurrent;
}

unsigned short MaintenanceEvent::getTirePressureFrontLeft()
{
	return tirePressureFrontLeft;
}

unsigned short MaintenanceEvent::getTirePressureFrontRight()
{
	return tirePressureFrontRight;
}

unsigned short MaintenanceEvent::getTirePressureRearLeft()
{
	return tirePressureRearLeft;
}

unsigned short MaintenanceEvent::getTirePressureRearRight()
{
	return tirePressureRearRight;
}

unsigned short MaintenanceEvent::getTirePressureStatusFrontLeft()
{
	return tirePressureStatusFrontLeft;
}

unsigned short MaintenanceEvent::getTirePressureStatusFrontRight()
{
	return tirePressureStatusFrontRight;
}

unsigned short MaintenanceEvent::getTirePressureStatusRearLeft()
{
	return tirePressureStatusRearLeft;
}

unsigned short MaintenanceEvent::getTirePressureStatusRearRight()
{
	return tirePressureStatusRearRight;
}

void MaintenanceEvent::setType(VehicleEventType type)
{
	this->type = type;
}

void MaintenanceEvent::setOdometer(unsigned long odometer)
{
	this->odometer = odometer;
}

void MaintenanceEvent::setTransmissionOilLifeLevel(bool transmissionOilLifeLevel)
{
	this->transmissionOilLifeLevel = transmissionOilLifeLevel;
}

void MaintenanceEvent::setTransmissionOilTemp(short transmissionOilTemp)
{
	this->transmissionOilTemp = transmissionOilTemp;
}

void MaintenanceEvent::setBrakeFluidLevel(bool brakeFluidLevel)
{
	this->brakeFluidLevel = brakeFluidLevel;
}

void MaintenanceEvent::setWasherFluidLevel(bool washerFluidLevel)
{
	this->washerFluidLevel = washerFluidLevel;
}

void MaintenanceEvent::setMalfunctionIndicatorLamp(bool malfunctionIndicatorLamp)
{
	this->malfunctionIndicatorLamp = malfunctionIndicatorLamp;
}

void MaintenanceEvent::setBatteryVoltage(unsigned short batteryVoltage)
{
	this->batteryVoltage = batteryVoltage;
}

void MaintenanceEvent::setBatteryCurrent(unsigned short batteryCurrent)
{
	this->batteryCurrent = batteryCurrent;
}

void MaintenanceEvent::setTirePressureFrontLeft(unsigned short tirePressureFrontLeft)
{
	this->tirePressureFrontLeft = tirePressureFrontLeft;
}

void MaintenanceEvent::setTirePressureFrontRight(unsigned short tirePressureFrontRight)
{
	this->tirePressureFrontRight = tirePressureFrontRight;
}

void MaintenanceEvent::setTirePressureRearLeft(unsigned short tirePressureRearLeft)
{
	this->tirePressureRearLeft = tirePressureRearRight;
}

void MaintenanceEvent::setTirePressureRearRight(unsigned short tirePressureRearRight)
{
	this->tirePressureRearRight = tirePressureRearRight;
}

void MaintenanceEvent::setTirePressureStatusFrontLeft(unsigned short tirePressureStatusFrontLeft)
{
	this->tirePressureStatusFrontLeft = tirePressureStatusFrontLeft;
}

void MaintenanceEvent::setTirePressureStatusFrontRight(unsigned short tirePressureStatusFrontRight)
{
	this->tirePressureStatusFrontRight = tirePressureStatusFrontRight;
}

void MaintenanceEvent::setTirePressureStatusRearLeft(unsigned short tirePressureStatusRearLeft)
{
	this->tirePressureStatusRearLeft = tirePressureStatusRearLeft;
}

void MaintenanceEvent::setTirePressureStatusRearRight(unsigned short tirePressureStatusRearRight)
{
	this->tirePressureStatusRearRight = tirePressureStatusRearRight;
}

