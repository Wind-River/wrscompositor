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
 * @file VehicleEvent.cpp 
 * For further information, see http://www.genivi.org/
 */

#include "VehicleEvent.h"

std::vector<DOMString> VehicleEvent::getSupportedEventTypes(VehicleEventType type, bool writable)
{
	std::vector<DOMString> allSupportedEventTypes;
	std::vector<DOMString> supportedEventTypes;
	std::vector<DOMString>::iterator it;

	if (writable)
	{
		allSupportedEventTypes = allSupportedEventTypesW;
	}
	else
	{
		allSupportedEventTypes = allSupportedEventTypesR;
	}

	for (it = allSupportedEventTypes.begin(); it != allSupportedEventTypes.end(); it++)
	{
		if ((*it).compare(0, type.length(), type) == 0)
		{
			supportedEventTypes.push_back((*it));
		}
	}

	return supportedEventTypes;
}

bool VehicleEvent::isSupportedEventType(VehicleEventType type, bool writable)
{
	std::vector<DOMString> allSupportedEventTypes;
	std::vector<DOMString>::iterator it;

	if (writable)
	{
		allSupportedEventTypes = allSupportedEventTypesW;
	}
	else
	{
		allSupportedEventTypes = allSupportedEventTypesR;
	}

	for (it = allSupportedEventTypes.begin(); it != allSupportedEventTypes.end(); it++)
	{
		if (type.compare(*it) == 0)
		{
			return true;
		}
	}

	return false;
}

void VehicleEvent::initVehicleEvent(VehicleEventType vehicleEventType, bool bubbles, bool cancelable) 
{
	Event::initEvent(vehicleEventType, bubbles, cancelable);
}
