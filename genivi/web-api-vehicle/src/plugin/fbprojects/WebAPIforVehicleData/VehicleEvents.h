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
 * @file VehicleEvents.h 
 * For further information, see http://www.genivi.org/
 */

#ifndef H_VehicleEvents
#define H_VehicleEvents

#include <string>
#include <sstream>
#include "JSAPIAuto.h"
#include "BrowserHost.h"
#include "FBPointers.h"

#include "TypeDef.h"
#include "VehicleEvent.h"
#include "VehicleInfoEvent.h"
#include "RunningStatusEvent.h"
#include "MaintenanceEvent.h"
#include "PersonalizationEvent.h"
#include "DrivingSafetyEvent.h"
#include "VisionSystemEvent.h"
#include "ParkingEvent.h"
#include "ClimateEnvironmentEvent.h"

FB_FORWARD_PTR(VehicleEvent);
FB_FORWARD_PTR(VehicleInfoEvent);
FB_FORWARD_PTR(RunningStatusEvent);
FB_FORWARD_PTR(MaintenanceEvent);
FB_FORWARD_PTR(PersonalizationEvent);
FB_FORWARD_PTR(DrivingSafetyEvent);
FB_FORWARD_PTR(VisionSystemEvent);
FB_FORWARD_PTR(ParkingEvent);
FB_FORWARD_PTR(ClimateEnvironmentEvent);

class VehicleEvents : public FB::JSAPIAuto
{
public:
	VehicleEvents(const FB::BrowserHostPtr& host);
	virtual ~VehicleEvents();

	VehicleEventPtr getVehicleEvent(VehicleEventType type);

private:
	FB::BrowserHostPtr m_host;
	VehicleInfoEventPtr m_vehicleInfoEvent;
	RunningStatusEventPtr m_runningStatusEvent;
	MaintenanceEventPtr m_maintenanceEvent;
	PersonalizationEventPtr m_personalizationEvent;
	DrivingSafetyEventPtr m_drivingSafetyEvent;
	VisionSystemEventPtr m_visionSystemEvent;
	ParkingEventPtr m_parkingEvent;
	ClimateEnvironmentEventPtr m_climateEnvironmentEvent;
};

#endif // H_VehicleEvents
