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
 * @file VehicleInfoEvent.cpp 
 * For further information, see http://www.genivi.org/
 */

#include "JSObject.h"
#include <boost/assign.hpp>
#include "DOM/Document.h"

#include "VehicleInfoEvent.h"

const VehicleEventType VehicleInfoEvent::VEHICLEINFO = "vehicleinfo";
const VehicleEventType VehicleInfoEvent::VEHICLEINFO_WMI = "vehicleinfo_wmi";
const VehicleEventType VehicleInfoEvent::VEHICLEINFO_VIN = "vehicleinfo_vin";
const VehicleEventType VehicleInfoEvent::VEHICLEINFO_VEHICLETYPE = "vehicleinfo_vehicletype";
const VehicleEventType VehicleInfoEvent::VEHICLEINFO_DOORTYPE = "vehicleinfo_doortype";
const VehicleEventType VehicleInfoEvent::VEHICLEINFO_DOORTYPE_1STROW = "vehicleinfo_doortype_1strow";
const VehicleEventType VehicleInfoEvent::VEHICLEINFO_DOORTYPE_2NDROW = "vehicleinfo_doortype_2ndrow";
const VehicleEventType VehicleInfoEvent::VEHICLEINFO_DOORTYPE_3RDROW = "vehicleinfo_doortype_3rdrow";
const VehicleEventType VehicleInfoEvent::VEHICLEINFO_FUELTYPE = "vehicleinfo_fueltype";
const VehicleEventType VehicleInfoEvent::VEHICLEINFO_TRANSMISSIONGEARTYPE = "vehicleinfo_transmissiongeartype";
const VehicleEventType VehicleInfoEvent::VEHICLEINFO_WHEELINFO = "vehicleinfo_wheelinfo";
const VehicleEventType VehicleInfoEvent::VEHICLEINFO_WHEELINFO_RADIUS = "vehicleinfo_wheelinfo_radius";
const VehicleEventType VehicleInfoEvent::VEHICLEINFO_WHEELINFO_TRACK = "vehicleinfo_wheelinfo_track";

VehicleInfoEvent::VehicleInfoEvent(const FB::BrowserHostPtr& host)
{
	registerProperty("type",
					 make_property(this,
								   &VehicleInfoEvent::getType));
	registerProperty("wmi",
					 make_property(this,
								   &VehicleInfoEvent::getWmi));
	registerProperty("vin",
					 make_property(this,
								   &VehicleInfoEvent::getVin));
	registerProperty("vehicleType",
					 make_property(this,
								   &VehicleInfoEvent::getVehicleType));
	registerProperty("doorType1stRow",
					 make_property(this,
								   &VehicleInfoEvent::getDoorType1stRow));
	registerProperty("doorType2ndRow",
					 make_property(this,
								   &VehicleInfoEvent::getDoorType2ndRow));
	registerProperty("doorType3rdRow",
					 make_property(this,
								   &VehicleInfoEvent::getDoorType3rdRow));
	registerProperty("fuelType",
					 make_property(this,
								   &VehicleInfoEvent::getFuelType));
	registerProperty("transmissionGearType",
					 make_property(this,
								   &VehicleInfoEvent::getTransmissionGearType));
	registerProperty("wheelInfoRadius",
					 make_property(this,
								   &VehicleInfoEvent::getWheelInfoRadius));
	registerProperty("wheelInfoTrack",
					 make_property(this,
								   &VehicleInfoEvent::getWheelInfoTrack));

	allSupportedEventTypesR.push_back(VEHICLEINFO);
	allSupportedEventTypesR.push_back(VEHICLEINFO_WMI);
	allSupportedEventTypesR.push_back(VEHICLEINFO_VIN);
	allSupportedEventTypesR.push_back(VEHICLEINFO_VEHICLETYPE);
	allSupportedEventTypesR.push_back(VEHICLEINFO_DOORTYPE);
	allSupportedEventTypesR.push_back(VEHICLEINFO_DOORTYPE_1STROW);
	allSupportedEventTypesR.push_back(VEHICLEINFO_DOORTYPE_2NDROW);
	allSupportedEventTypesR.push_back(VEHICLEINFO_DOORTYPE_3RDROW);
	allSupportedEventTypesR.push_back(VEHICLEINFO_FUELTYPE);
	allSupportedEventTypesR.push_back(VEHICLEINFO_TRANSMISSIONGEARTYPE);
	allSupportedEventTypesR.push_back(VEHICLEINFO_WHEELINFO);
	allSupportedEventTypesR.push_back(VEHICLEINFO_WHEELINFO_RADIUS);
	allSupportedEventTypesR.push_back(VEHICLEINFO_WHEELINFO_TRACK);

	validity[VEHICLEINFO] = false;
	validity[VEHICLEINFO_WMI] = false;
	validity[VEHICLEINFO_VIN] = false;
	validity[VEHICLEINFO_VEHICLETYPE] = false;
	validity[VEHICLEINFO_DOORTYPE] = false;
	validity[VEHICLEINFO_DOORTYPE_1STROW] = false;
	validity[VEHICLEINFO_DOORTYPE_2NDROW] = false;
	validity[VEHICLEINFO_DOORTYPE_3RDROW] = false;
	validity[VEHICLEINFO_FUELTYPE] = false;
	validity[VEHICLEINFO_TRANSMISSIONGEARTYPE] = false;
	validity[VEHICLEINFO_WHEELINFO] = false;
	validity[VEHICLEINFO_WHEELINFO_RADIUS] = false;
	validity[VEHICLEINFO_WHEELINFO_TRACK] = false;
}

VehicleInfoEvent::~VehicleInfoEvent()
{

}

void VehicleInfoEvent::updateValidity(VehicleEventType type)
{
	std::string str = type;
	size_t pos;

	while ((pos = str.find_last_of('_')) != std::string::npos)
	{
		str = str.substr(0, pos);

		if (str.compare(VEHICLEINFO) == 0)
		{
			setValidity(VEHICLEINFO, isValid(VEHICLEINFO_WMI)
						& isValid(VEHICLEINFO_VIN)
						& isValid(VEHICLEINFO_VEHICLETYPE)
						& isValid(VEHICLEINFO_DOORTYPE)
						& isValid(VEHICLEINFO_FUELTYPE)
						& isValid(VEHICLEINFO_TRANSMISSIONGEARTYPE)
						& isValid(VEHICLEINFO_WHEELINFO));
		}
		else if (str.compare(VEHICLEINFO_DOORTYPE) == 0)
		{
			setValidity(VEHICLEINFO_DOORTYPE, isValid(VEHICLEINFO_DOORTYPE_1STROW)
						& isValid(VEHICLEINFO_DOORTYPE_2NDROW)
						& isValid(VEHICLEINFO_DOORTYPE_3RDROW));
		}
		else if (str.compare(VEHICLEINFO_WHEELINFO) == 0)
		{
			setValidity(VEHICLEINFO_WHEELINFO, isValid(VEHICLEINFO_WHEELINFO_RADIUS)
						& isValid(VEHICLEINFO_WHEELINFO_TRACK));
		}
	}
}

VehicleEventType VehicleInfoEvent::getType()
{
	return type;
}

DOMString VehicleInfoEvent::getWmi()
{
	return wmi;
}

DOMString VehicleInfoEvent::getVin()
{
	return vin;
}

unsigned short VehicleInfoEvent::getVehicleType()
{
	return vehicleType;
}

unsigned short VehicleInfoEvent::getDoorType1stRow()
{
	return doorType1stRow;
}

unsigned short VehicleInfoEvent::getDoorType2ndRow()
{
	return doorType2ndRow;
}

unsigned short VehicleInfoEvent::getDoorType3rdRow()
{
	return doorType3rdRow;
}

octet VehicleInfoEvent::getFuelType()
{
	return fuelType;
}

unsigned short VehicleInfoEvent::getTransmissionGearType()
{
	return transmissionGearType;
}

double VehicleInfoEvent::getWheelInfoRadius()
{
	return wheelInfoRadius;
}

double VehicleInfoEvent::getWheelInfoTrack()
{
	return wheelInfoTrack;
}

void VehicleInfoEvent::setType(VehicleEventType type)
{
	this->type = type;
}

void VehicleInfoEvent::setWmi(DOMString wmi)
{
	this->wmi = wmi;
}

void VehicleInfoEvent::setVin(DOMString vin)
{
	this->vin = vin;
}

void VehicleInfoEvent::setVehicleType(unsigned short vehicleType)
{
	this->vehicleType = vehicleType;
}

void VehicleInfoEvent::setDoorType1stRow(unsigned short doorType1stRow)
{
	this->doorType1stRow = doorType1stRow;
}

void VehicleInfoEvent::setDoorType2ndRow(unsigned short doorType2ndRow)
{
	this->doorType2ndRow = doorType2ndRow;
}

void VehicleInfoEvent::setDoorType3rdRow(unsigned short doorType3rdRow)
{
	this->doorType3rdRow = doorType3rdRow;
}

void VehicleInfoEvent::setFuelType(octet fuelType)
{
	this->fuelType = fuelType;
}

void VehicleInfoEvent::setTransmissionGearType(unsigned short transmissionGearType)
{
	this->transmissionGearType = transmissionGearType;
}

void VehicleInfoEvent::setWheelInfoRadius(double wheelInfoRadius)
{
	this->wheelInfoRadius = wheelInfoRadius;
}

void VehicleInfoEvent::setWheelInfoTrack(double wheelInfoTrack)
{
	this->wheelInfoTrack = wheelInfoTrack;
}
