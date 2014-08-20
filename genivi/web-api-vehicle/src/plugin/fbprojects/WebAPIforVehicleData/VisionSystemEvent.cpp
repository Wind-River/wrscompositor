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
 * @file VisionSystemEvent.cpp 
 * For further information, see http://www.genivi.org/
 */

#include "JSObject.h"
#include <boost/assign.hpp>
#include "DOM/Document.h"

#include "VisionSystemEvent.h"

const VehicleEventType VisionSystemEvent::VISIONSYSTEM = "visionsystem";
const VehicleEventType VisionSystemEvent::VISIONSYSTEM_LANEDEPARTUREDETECTIONSTATUS = "visionsystem_lanedeparturedetectionstatus";
const VehicleEventType VisionSystemEvent::VISIONSYSTEM_LANEDEPARTED = "visionsystem_lanedeparted";

VisionSystemEvent::VisionSystemEvent(const FB::BrowserHostPtr& host)
{
	registerProperty("type",
					 make_property(this,
								   &VisionSystemEvent::getType));
	registerProperty("laneDepartureDetectionStatus",
					 make_property(this,
								   &VisionSystemEvent::getLaneDepartureDetectionStatus));
	registerProperty("laneDeparted",
					 make_property(this,
								   &VisionSystemEvent::getLaneDeparted));

	allSupportedEventTypesR.push_back(VISIONSYSTEM);
	allSupportedEventTypesR.push_back(VISIONSYSTEM_LANEDEPARTUREDETECTIONSTATUS);
	allSupportedEventTypesR.push_back(VISIONSYSTEM_LANEDEPARTED);

	validity[VISIONSYSTEM] = false;
	validity[VISIONSYSTEM_LANEDEPARTUREDETECTIONSTATUS] = false;
	validity[VISIONSYSTEM_LANEDEPARTED] = false;
}

VisionSystemEvent::~VisionSystemEvent()
{

}

void VisionSystemEvent::updateValidity(VehicleEventType type)
{
	std::string str = type;
	size_t pos;

	while ((pos = str.find_last_of('_')) != std::string::npos)
	{
		str = str.substr(0, pos);

		if (str.compare(VISIONSYSTEM) == 0)
		{
			setValidity(VISIONSYSTEM, isValid(VISIONSYSTEM_LANEDEPARTUREDETECTIONSTATUS)
						& isValid(VISIONSYSTEM_LANEDEPARTED));
		}
	}
}

VehicleEventType VisionSystemEvent::getType()
{
	return type;
}

bool VisionSystemEvent::getLaneDepartureDetectionStatus()
{
	return laneDepartureDetectionStatus;
}

bool VisionSystemEvent::getLaneDeparted()
{
	return laneDeparted;
}

void VisionSystemEvent::setType(VehicleEventType type)
{
	this->type = type;
}

void VisionSystemEvent::setLaneDepartureDetectionStatus(bool laneDepartureDetectionStatus)
{
	this->laneDepartureDetectionStatus = laneDepartureDetectionStatus;
}

void VisionSystemEvent::setLaneDeparted(bool laneDeparted)
{
	this->laneDeparted = laneDeparted;
}
