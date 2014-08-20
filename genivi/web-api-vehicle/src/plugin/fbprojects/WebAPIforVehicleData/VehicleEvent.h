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
 * @file VehicleEvent.h 
 * For further information, see http://www.genivi.org/
 */

#include "JSAPIAuto.h"
#include "variant_list.h"

#include "ValidityCheck.h"
#include "TypeDef.h"

#ifndef H_Event
#define H_Event

class Event {
public:
	void initEvent(DOMString eventTypeArg, bool canBubbleArg, bool cancelableArg) {};
};

#endif // H_Event

#ifndef H_VehicleEvent
#define H_VehicleEvent

class VehicleEvent : public FB::JSAPIAuto, public Event, public ValidityCheck
{
public:
	virtual ~VehicleEvent() {};
	std::vector<DOMString> getSupportedEventTypes(VehicleEventType type, bool writable);
	bool isSupportedEventType(VehicleEventType type, bool writable);
	void initVehicleEvent(VehicleEventType vehicleEventType, bool bubbles, bool cancelable);

protected:
	virtual void updateValidity(VehicleEventType type) = 0;

	std::vector<DOMString> allSupportedEventTypesR;
	std::vector<DOMString> allSupportedEventTypesW;
};

#endif // H_VehicleEvent
