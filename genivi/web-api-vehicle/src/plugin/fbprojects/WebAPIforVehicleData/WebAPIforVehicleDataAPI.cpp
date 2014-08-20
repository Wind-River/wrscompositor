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
 * @file WebAPIforVehicleDataAPI.cpp 
 * For further information, see http://www.genivi.org/
 */

#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"
#include "global/config.h"

#include "WebAPIforVehicleDataAPI.h"

WebAPIforVehicleDataAPI::WebAPIforVehicleDataAPI(const WebAPIforVehicleDataPtr& plugin, 
													   const FB::BrowserHostPtr& host) : 
	m_plugin(plugin), m_host(host)
{
	registerProperty("vehicle",
					 make_property(this,
								   &WebAPIforVehicleDataAPI::getVehicleInterface));

	m_vehicleEvents = boost::make_shared<VehicleEvents>(m_host);
	m_vehicleInterface = boost::make_shared<VehicleInterface>(m_host);
	m_vehicleInterfaceAdapter = boost::make_shared<VehicleInterfaceAdapter>(m_host);

	m_vehicleInterface->setVehicleEventsPtr(m_vehicleEvents);
	m_vehicleInterface->setVehicleInterfaceAdapterPtr(m_vehicleInterfaceAdapter);
	m_vehicleInterfaceAdapter->setVehicleEventsPtr(m_vehicleEvents);
	m_vehicleInterfaceAdapter->setVehicleInterfacePtr(m_vehicleInterface);

	m_vehicleInterfaceAdapter->startDBusReceiver();
}

WebAPIforVehicleDataAPI::~WebAPIforVehicleDataAPI() 
{

}

WebAPIforVehicleDataPtr WebAPIforVehicleDataAPI::getPlugin()
{
    WebAPIforVehicleDataPtr plugin(m_plugin.lock());
    if (!plugin) {
        throw FB::script_error("The plugin is invalid");
    }
    return plugin;
}

VehicleInterfacePtr WebAPIforVehicleDataAPI::getVehicleInterface()
{
	return m_vehicleInterface;
}
