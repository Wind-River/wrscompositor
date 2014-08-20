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
 * @file WebAPIforVehicleDataAPI.h 
 * For further information, see http://www.genivi.org/
 */

#include <string>
#include <sstream>
#include <boost/weak_ptr.hpp>
#include "JSAPIAuto.h"
#include "BrowserHost.h"
#include "WebAPIforVehicleData.h"

#include "FBPointers.h"
#include "VehicleEvents.h"
#include "VehicleInterface.h"
#include "VehicleInterfaceAdapter.h"

#ifndef H_WebAPIforVehicleDataAPI
#define H_WebAPIforVehicleDataAPI

FB_FORWARD_PTR(VehicleEvents);
FB_FORWARD_PTR(VehicleInterface);
FB_FORWARD_PTR(VehicleInterfaceAdapter);

class WebAPIforVehicleDataAPI : public FB::JSAPIAuto
{
public:
    WebAPIforVehicleDataAPI(const WebAPIforVehicleDataPtr& plugin, 
							const FB::BrowserHostPtr& host);
    virtual ~WebAPIforVehicleDataAPI();

    WebAPIforVehicleDataPtr getPlugin();
	VehicleInterfacePtr getVehicleInterface();

private:
    WebAPIforVehicleDataWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;

	VehicleEventsPtr m_vehicleEvents;
	VehicleInterfacePtr m_vehicleInterface;
	VehicleInterfaceAdapterPtr m_vehicleInterfaceAdapter;
};

#endif // H_WebAPIforVehicleDataAPI

