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
 * @file ParkingEvent.cpp 
 * For further information, see http://www.genivi.org/
 */

#include "JSObject.h"
#include <boost/assign.hpp>
#include "DOM/Document.h"

#include "ParkingEvent.h"

const VehicleEventType ParkingEvent::PARKING= "parking";
const VehicleEventType ParkingEvent::PARKING_SECURITYALERT = "parking_securityalert";
const VehicleEventType ParkingEvent::PARKING_PARKINGBRAKE = "parking_parkingbrake";
const VehicleEventType ParkingEvent::PARKING_PARKINGLIGHTS = "parking_parkinglights";

ParkingEvent::ParkingEvent(const FB::BrowserHostPtr& host)
{
	registerProperty("type",
					 make_property(this,
								   &ParkingEvent::getType));
	registerProperty("securityAlert",
					 make_property(this,
								   &ParkingEvent::getSecurityAlert));
	registerProperty("parkingBrake",
					 make_property(this,
								   &ParkingEvent::getParkingBrake));
	registerProperty("parkingLights",
					 make_property(this,
								   &ParkingEvent::getParkingLights));

	allSupportedEventTypesR.push_back(PARKING);
	allSupportedEventTypesR.push_back(PARKING_SECURITYALERT);
	allSupportedEventTypesR.push_back(PARKING_PARKINGBRAKE);
	allSupportedEventTypesR.push_back(PARKING_PARKINGLIGHTS);

	allSupportedEventTypesW.push_back(PARKING_PARKINGLIGHTS);

	validity[PARKING] = false;
	validity[PARKING_SECURITYALERT] = false;
	validity[PARKING_PARKINGBRAKE] = false;
	validity[PARKING_PARKINGLIGHTS] = false;
}

ParkingEvent::~ParkingEvent()
{

}

void ParkingEvent::updateValidity(VehicleEventType type)
{
	std::string str = type;
	size_t pos;

	while ((pos = str.find_last_of('_')) != std::string::npos)
	{
		str = str.substr(0, pos);

		if (str.compare(PARKING) == 0)
		{
			setValidity(PARKING, isValid(PARKING_SECURITYALERT)
						& isValid(PARKING_PARKINGBRAKE)
						& isValid(PARKING_PARKINGLIGHTS));
		}
	}
}

VehicleEventType ParkingEvent::getType()
{
	return type;
}

unsigned short ParkingEvent::getSecurityAlert()
{
	return securityAlert;
}

bool ParkingEvent::getParkingBrake()
{
	return parkingBrake;
}

bool ParkingEvent::getParkingLights()
{
	return parkingLights;
}

void ParkingEvent::setType(VehicleEventType type)
{
	this->type = type;
}

void ParkingEvent::setSecurityAlert(unsigned short securityAlert)
{
	this->securityAlert = securityAlert;
}

void ParkingEvent::setParkingBrake(bool parkingBrake)
{
	this->parkingBrake = parkingBrake;
}

void ParkingEvent::setParkingLights(bool parkingLights)
{
	this->parkingLights = parkingLights;
}

