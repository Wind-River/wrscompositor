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
 * @file ParkingEvent.h 
 * For further information, see http://www.genivi.org/
 */

#ifndef H_ParkingEvent
#define H_ParkingEvent

#include <string>
#include <sstream>
#include "VehicleEvent.h"
#include "TypeDef.h"

class ParkingEvent : public VehicleEvent
{
public:
	ParkingEvent(const FB::BrowserHostPtr& host);
	virtual ~ParkingEvent();

	VehicleEventType getType();
	unsigned short getSecurityAlert();
	bool getParkingBrake();
	bool getParkingLights();

	void setType(VehicleEventType type);
	void setSecurityAlert(unsigned short securityAlert);
	void setParkingBrake(bool parkingBrake);
	void setParkingLights(bool parkingLights);

	static const VehicleEventType PARKING;
	static const VehicleEventType PARKING_SECURITYALERT;
	static const VehicleEventType PARKING_PARKINGBRAKE;
	static const VehicleEventType PARKING_PARKINGLIGHTS;

private:
	virtual void updateValidity(VehicleEventType type);

	FB::BrowserHostPtr m_host;

	VehicleEventType type;
	unsigned short securityAlert;
	bool parkingBrake;
	bool parkingLights;
};

#endif // H_ParkingEvent
