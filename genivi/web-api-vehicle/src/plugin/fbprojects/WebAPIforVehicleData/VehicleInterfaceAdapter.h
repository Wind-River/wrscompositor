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
 * @file VehicleInterfaceAdapter.h 
 * For further information, see http://www.genivi.org/
 */

#ifndef H_VehicleInterfaceAdapter
#define H_VehicleInterfaceAdapter

#include <string>
#include <sstream>
#include "JSAPIAuto.h"
#include "BrowserHost.h"

#include "FBPointers.h"
#include "TypeDef.h"
#include "VehicleEvents.h"
#include "VehicleInterface.h"
#include "dbus-sender.h"
#include "dbus-receiver.h"

FB_FORWARD_PTR(VehicleEvents);
FB_FORWARD_PTR(VehicleInterface);

class VehicleInterfaceAdapter : public FB::JSAPIAuto
{
public:
	VehicleInterfaceAdapter(const FB::BrowserHostPtr& host);
	virtual ~VehicleInterfaceAdapter();
	
	void setVehicleEventsPtr(const VehicleEventsPtr& m_vehicleEvents);
	void setVehicleInterfacePtr(const VehicleInterfacePtr& m_vehicleInterface);

	template<typename Data> bool setVehicleData(VehicleEventType type, Data data)
	{
		std::string name = typeid(data).name();
		int dbus_type;

		if (name.compare(typeid(bool).name()) == 0) 
		{
			dbus_type = DBUS_TYPE_UINT16;
		}
		else if (name.compare(typeid(unsigned short).name()) == 0)
		{
			dbus_type = DBUS_TYPE_UINT16;
		}
		else if (name.compare(typeid(short).name()) == 0)
		{
			dbus_type = DBUS_TYPE_INT16;
		}
		else if (name.compare(typeid(unsigned long).name()) == 0)
		{
			dbus_type = DBUS_TYPE_UINT32;;
		}
		else if (name.compare(typeid(std::string).name()) == 0)
		{
			dbus_type = DBUS_TYPE_STRING;
		}

		return dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, ("set_" + type).c_str(), dbus_type, &data, DBUS_TYPE_INVALID);
	}

	void startDBusReceiver();
	void stopDBusReceiver();

private:
	static void processSignal(DBusMessage* message,
							  const char* signal_name,
							  int index1,
							  int index2);
	static void fireAllEvents(const std::string& type, VehicleEventPtr ptr);

	FB::BrowserHostPtr m_host;
	static VehicleEventsPtr m_vehicleEvents;
	static VehicleInterfacePtr m_vehicleInterface;

	DBusSender dbus_sender;
	DBusReceiver dbus_receiver;

	const char* DBUS_PATH;
	const char* DBUS_INTERFACE;

	const char* signal_list_get_vehicleinfo[11];
	const char* signal_list_get_runningstatus[38];
	const char* signal_list_get_maintenance[17];
	const char* signal_list_get_personalization[28];
	const char* signal_list_get_drivingsafety[44];
	const char* signal_list_get_visionsystem[3];
	const char* signal_list_get_parking[4];
	const char* signal_list_get_climateenvironment[26];
};

#endif // H_VehicleInterfaceAdapter
