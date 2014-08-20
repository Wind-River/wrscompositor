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
 * @file VehicleEvents.cpp 
 * For further information, see http://www.genivi.org/
 */

#include "JSObject.h"
#include <boost/assign.hpp>
#include "DOM/Document.h"

#include "VehicleEvents.h"

VehicleEvents::VehicleEvents(const FB::BrowserHostPtr& host) : m_host(host)
{
	m_vehicleInfoEvent = boost::make_shared<VehicleInfoEvent>(m_host);
	m_runningStatusEvent = boost::make_shared<RunningStatusEvent>(m_host);
	m_personalizationEvent = boost::make_shared<PersonalizationEvent>(m_host);
	m_maintenanceEvent = boost::make_shared<MaintenanceEvent>(m_host);
	m_drivingSafetyEvent = boost::make_shared<DrivingSafetyEvent>(m_host);
	m_visionSystemEvent = boost::make_shared<VisionSystemEvent>(m_host);
	m_parkingEvent = boost::make_shared<ParkingEvent>(m_host);
	m_climateEnvironmentEvent = boost::make_shared<ClimateEnvironmentEvent>(m_host);
}

VehicleEvents::~VehicleEvents()
{

}

VehicleEventPtr VehicleEvents::getVehicleEvent(VehicleEventType type)
{
	if (type.compare(0, VehicleInfoEvent::VEHICLEINFO.length(), VehicleInfoEvent::VEHICLEINFO) == 0)
	{
		return m_vehicleInfoEvent;
	}
	else if (type.compare(0, RunningStatusEvent::RUNNINGSTATUS.length(), RunningStatusEvent::RUNNINGSTATUS) == 0)
	{
		return m_runningStatusEvent;
	}
	else if (type.compare(0, MaintenanceEvent::MAINTENANCE.length(), MaintenanceEvent::MAINTENANCE) == 0)
	{
		return m_maintenanceEvent;
	}
	else if (type.compare(0, PersonalizationEvent::PERSONALIZATION.length(), PersonalizationEvent::PERSONALIZATION) == 0)
	{
		return m_personalizationEvent;
	}
	else if (type.compare(0, DrivingSafetyEvent::DRIVINGSAFETY.length(), DrivingSafetyEvent::DRIVINGSAFETY) == 0)
	{
		return m_drivingSafetyEvent;
	}
	else if (type.compare(0, VisionSystemEvent::VISIONSYSTEM.length(), VisionSystemEvent::VISIONSYSTEM) == 0)
	{
		return m_visionSystemEvent;
	}
	else if (type.compare(0, ParkingEvent::PARKING.length(), ParkingEvent::PARKING) == 0)
	{
		return m_parkingEvent;
	}
	else if (type.compare(0, ClimateEnvironmentEvent::CLIMATEENVIRONMENT.length(), ClimateEnvironmentEvent::CLIMATEENVIRONMENT) == 0)
	{
		return m_climateEnvironmentEvent;
	}
	else
	{
		VehicleEventPtr nullPtr;
		return nullPtr;
	}
}
