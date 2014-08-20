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
 * @file VehicleInfoEvent.h 
 * For further information, see http://www.genivi.org/
 */

#ifndef H_VehicleInfoEvent
#define H_VehicleInfoEvent

#include <string>
#include <sstream>
#include "VehicleEvent.h"
#include "TypeDef.h"

class VehicleInfoEvent : public VehicleEvent
{
public:
	VehicleInfoEvent(const FB::BrowserHostPtr& host);
	virtual ~VehicleInfoEvent();

	VehicleEventType getType();
	DOMString getWmi();
	DOMString getVin();
	unsigned short getVehicleType();
	unsigned short getDoorType1stRow();
	unsigned short getDoorType2ndRow();
	unsigned short getDoorType3rdRow();
	octet getFuelType();
	unsigned short getTransmissionGearType();
	double getWheelInfoRadius();
	double getWheelInfoTrack();

	void setType(VehicleEventType type);
	void setWmi(DOMString wmi);
	void setVin(DOMString vin);
	void setVehicleType(unsigned short vehicleType);
	void setDoorType1stRow(unsigned short doorType1stRow);
	void setDoorType2ndRow(unsigned short doorType2ndRow);
	void setDoorType3rdRow(unsigned short doorType3rdRow);
	void setFuelType(octet fuelType);
	void setTransmissionGearType(unsigned short transmissionGearType);
	void setWheelInfoRadius(double wheelInfoRadius);
	void setWheelInfoTrack(double wheelInfoTrack);

	static const VehicleEventType VEHICLEINFO;
	static const VehicleEventType VEHICLEINFO_WMI;
	static const VehicleEventType VEHICLEINFO_VIN;
	static const VehicleEventType VEHICLEINFO_VEHICLETYPE;
	static const VehicleEventType VEHICLEINFO_DOORTYPE;
	static const VehicleEventType VEHICLEINFO_DOORTYPE_1STROW;
	static const VehicleEventType VEHICLEINFO_DOORTYPE_2NDROW;
	static const VehicleEventType VEHICLEINFO_DOORTYPE_3RDROW;
	static const VehicleEventType VEHICLEINFO_FUELTYPE;
	static const VehicleEventType VEHICLEINFO_TRANSMISSIONGEARTYPE;
	static const VehicleEventType VEHICLEINFO_WHEELINFO;
	static const VehicleEventType VEHICLEINFO_WHEELINFO_RADIUS;
	static const VehicleEventType VEHICLEINFO_WHEELINFO_TRACK;

	static const unsigned short VEHICLETYPE_SEDAN = 1;
	static const unsigned short VEHICLETYPE_COUPE = 2;
	static const unsigned short VEHICLETYPE_CABRIOLET = 3;
	static const unsigned short VEHICLETYPE_ROADSTER = 4;
	static const unsigned short VEHICLETYPE_SUV = 5;
	static const unsigned short VEHICLETYPE_TRUCK = 6;

	static const octet FUELTYPE_GASOLINE = 0x01;
	static const octet FUELTYPE_METHANOL= 0x02;
	static const octet FUELTYPE_ETHANOL = 0x03;
	static const octet FUELTYPE_DIESEL= 0x04;
	static const octet FUELTYPE_LPG = 0x05;
	static const octet FUELTYPE_CNG = 0x06;
	static const octet FUELTYPE_PROPANE = 0x07;
	static const octet FUELTYPE_ELECTRIC = 0x08;
	static const octet FUELTYPE_BIFUELRUNNINGGASOLINE = 0x09;
	static const octet FUELTYPE_BIFUELRUNNINGMETHANOL = 0x0A;
	static const octet FUELTYPE_BIFUELRUNNINGETHANOL = 0x0B;
	static const octet FUELTYPE_BIFUELRUNNINGLPG = 0x0C;
	static const octet FUELTYPE_BIFUELRUNNINGCNG = 0x0D;
	static const octet FUELTYPE_BIFUELRUNNINGPROP = 0x0E;
	static const octet FUELTYPE_BIFUELRUNNINGELECTRICITY = 0x0F;
	static const octet FUELTYPE_BIFUELMIXEDGASELECTRIC= 0x10;
	static const octet FUELTYPE_HYBRIDGASOLINE = 0x11;
	static const octet FUELTYPE_HYBRIDETHANOL = 0x12;
	static const octet FUELTYPE_HYBRIDDIESEL = 0x13;
	static const octet FUELTYPE_HYBRIDELECTRIC = 0x14;
	static const octet FUELTYPE_HYBRIDMIXEDFUEL = 0x15;
	static const octet FUELTYPE_HYBRIDREGENERATIVE = 0x16;
		
	static const unsigned short TRANSMISSIONGEARTYPE_AUTO = 1;
	static const unsigned short TRANSMISSIONGEARTYPE_MANUAL = 2;
	static const unsigned short TRANSMISSIONGEARTYPE_CVT = 3;

private:
	virtual void updateValidity(VehicleEventType type);

	FB::BrowserHostPtr m_host;

	VehicleEventType type;
	DOMString wmi;
	DOMString vin;
	unsigned short vehicleType;
	unsigned short doorType1stRow;
	unsigned short doorType2ndRow;
	unsigned short doorType3rdRow;
	octet fuelType;
	unsigned short transmissionGearType;
	double wheelInfoRadius;
	double wheelInfoTrack;
};

#endif // H_VehicleInfoEvent
