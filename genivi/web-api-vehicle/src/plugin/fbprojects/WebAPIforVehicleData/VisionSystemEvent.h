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
 * @file VisionSystemEvent.h 
 * For further information, see http://www.genivi.org/
 */

#ifndef H_VisionSystemEvent
#define H_VisionSystemEvent

#include <string>
#include <sstream>
#include "VehicleEvent.h"
#include "TypeDef.h"

class VisionSystemEvent : public VehicleEvent
{
public:
	VisionSystemEvent(const FB::BrowserHostPtr& host);
	virtual ~VisionSystemEvent();

	VehicleEventType getType();
	bool getLaneDepartureDetectionStatus();
	bool getLaneDeparted();

	void setType(VehicleEventType type);
	void setLaneDepartureDetectionStatus(bool laneDepartureDetectionStatus);
	void setLaneDeparted(bool laneDeparted);

	static const VehicleEventType VISIONSYSTEM;
	static const VehicleEventType VISIONSYSTEM_LANEDEPARTUREDETECTIONSTATUS;
	static const VehicleEventType VISIONSYSTEM_LANEDEPARTED;

private:
	virtual void updateValidity(VehicleEventType type);

	FB::BrowserHostPtr m_host;

	VehicleEventType type;
	bool laneDepartureDetectionStatus;
	bool laneDeparted;
};

#endif // H_VisionSystemEvent
