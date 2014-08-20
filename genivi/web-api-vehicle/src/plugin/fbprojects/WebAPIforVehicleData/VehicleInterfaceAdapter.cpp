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
 * @file VehicleInterfaceAdapter.cpp 
 * For further information, see http://www.genivi.org/
 */

#include "JSObject.h"
#include <boost/assign.hpp>
#include "DOM/Document.h"

#include "VehicleInterfaceAdapter.h"
#include "def-dbus.h"

VehicleEventsPtr VehicleInterfaceAdapter::m_vehicleEvents;
VehicleInterfacePtr VehicleInterfaceAdapter::m_vehicleInterface;

VehicleInterfaceAdapter::VehicleInterfaceAdapter(const FB::BrowserHostPtr& host) : m_host(host)
{
	dbus_sender.init_dbus();

	DBUS_PATH = "/org/genivi/vna";
	DBUS_INTERFACE = "org.genivi.vna";

	signal_list_get_vehicleinfo[0] = genivi::wavd::dbus::get::VEHICLEINFO_WMI;
	signal_list_get_vehicleinfo[1] = genivi::wavd::dbus::get::VEHICLEINFO_VIN;
	signal_list_get_vehicleinfo[2] = genivi::wavd::dbus::get::VEHICLEINFO_VEHICLETYPE;
	signal_list_get_vehicleinfo[3] = genivi::wavd::dbus::get::VEHICLEINFO_DOORTYPE_1STROW;
	signal_list_get_vehicleinfo[4] = genivi::wavd::dbus::get::VEHICLEINFO_DOORTYPE_2NDROW;
	signal_list_get_vehicleinfo[5] = genivi::wavd::dbus::get::VEHICLEINFO_DOORTYPE_3RDROW;
	signal_list_get_vehicleinfo[6] = genivi::wavd::dbus::get::VEHICLEINFO_FUELTYPE;
	signal_list_get_vehicleinfo[7] = genivi::wavd::dbus::get::VEHICLEINFO_TRANSMISSIONGEARTYPE;
	signal_list_get_vehicleinfo[8] = genivi::wavd::dbus::get::VEHICLEINFO_WHEELINFO_RADIUS;
	signal_list_get_vehicleinfo[9] = genivi::wavd::dbus::get::VEHICLEINFO_WHEELINFO_TRACK;
	signal_list_get_vehicleinfo[10] = NULL;

	signal_list_get_runningstatus[0] = genivi::wavd::dbus::get::RUNNINGSTATUS_VEHICLEPOWERMODE; 
	signal_list_get_runningstatus[1] = genivi::wavd::dbus::get::RUNNINGSTATUS_SPEEDOMETER;
	signal_list_get_runningstatus[2] = genivi::wavd::dbus::get::RUNNINGSTATUS_ENGINESPEED; 
	signal_list_get_runningstatus[3] = genivi::wavd::dbus::get::RUNNINGSTATUS_TRIPMETER_1_MILEAGE; 
	signal_list_get_runningstatus[4] = genivi::wavd::dbus::get::RUNNINGSTATUS_TRIPMETER_2_MILEAGE; 
	signal_list_get_runningstatus[5] = genivi::wavd::dbus::get::RUNNINGSTATUS_TRIPMETER_1_AVERAGESPEED; 
	signal_list_get_runningstatus[6] = genivi::wavd::dbus::get::RUNNINGSTATUS_TRIPMETER_2_AVERAGESPEED; 
	signal_list_get_runningstatus[7] = genivi::wavd::dbus::get::RUNNINGSTATUS_TRIPMETER_1_FUELCONSUMPTION; 
	signal_list_get_runningstatus[8] = genivi::wavd::dbus::get::RUNNINGSTATUS_TRIPMETER_2_FUELCONSUMPTION; 
	signal_list_get_runningstatus[9] = genivi::wavd::dbus::get::RUNNINGSTATUS_TRANSMISSIONGEARSTATUS; 
	signal_list_get_runningstatus[10] = genivi::wavd::dbus::get::RUNNINGSTATUS_CRUISECONTROL_STATUS; 
	signal_list_get_runningstatus[11] = genivi::wavd::dbus::get::RUNNINGSTATUS_CRUISECONTROL_SPEED; 
	signal_list_get_runningstatus[12] = genivi::wavd::dbus::get::RUNNINGSTATUS_WHEELBRAKE; 
	signal_list_get_runningstatus[13] = genivi::wavd::dbus::get::RUNNINGSTATUS_LIGHTSSTATUS_HEAD; 
	signal_list_get_runningstatus[14] = genivi::wavd::dbus::get::RUNNINGSTATUS_LIGHTSSTATUS_HIGHBEAM; 
	signal_list_get_runningstatus[15] = genivi::wavd::dbus::get::RUNNINGSTATUS_LIGHTSSTATUS_TURNLEFT; 
	signal_list_get_runningstatus[16] = genivi::wavd::dbus::get::RUNNINGSTATUS_LIGHTSSTATUS_TURNRIGHT; 
	signal_list_get_runningstatus[17] = genivi::wavd::dbus::get::RUNNINGSTATUS_LIGHTSSTATUS_BRAKE; 
	signal_list_get_runningstatus[18] = genivi::wavd::dbus::get::RUNNINGSTATUS_LIGHTSSTATUS_FOGFRONT; 
	signal_list_get_runningstatus[19] = genivi::wavd::dbus::get::RUNNINGSTATUS_LIGHTSSTATUS_FOGREAR; 
	signal_list_get_runningstatus[20] = genivi::wavd::dbus::get::RUNNINGSTATUS_LIGHTSSTATUS_HAZARD; 
	signal_list_get_runningstatus[21] = genivi::wavd::dbus::get::RUNNINGSTATUS_LIGHTSSTATUS_PARKING; 
	signal_list_get_runningstatus[22] = genivi::wavd::dbus::get::RUNNINGSTATUS_INTERIORLIGHTSSTATUS_DRIVER; 
	signal_list_get_runningstatus[23] = genivi::wavd::dbus::get::RUNNINGSTATUS_INTERIORLIGHTSSTATUS_PASSENGER; 
	signal_list_get_runningstatus[24] = genivi::wavd::dbus::get::RUNNINGSTATUS_INTERIORLIGHTSSTATUS_CENTER; 
	signal_list_get_runningstatus[25] = genivi::wavd::dbus::get::RUNNINGSTATUS_AUTOMATICHEADLIGHTS; 
	signal_list_get_runningstatus[26] = genivi::wavd::dbus::get::RUNNINGSTATUS_DYNAMICHIGHBEAM; 
	signal_list_get_runningstatus[27] = genivi::wavd::dbus::get::RUNNINGSTATUS_HORN; 
	signal_list_get_runningstatus[28] = genivi::wavd::dbus::get::RUNNINGSTATUS_CHIME; 
	signal_list_get_runningstatus[29] = genivi::wavd::dbus::get::RUNNINGSTATUS_FUEL; 
	signal_list_get_runningstatus[30] = genivi::wavd::dbus::get::RUNNINGSTATUS_ESTIMATEDRANGE; 
	signal_list_get_runningstatus[31] = genivi::wavd::dbus::get::RUNNINGSTATUS_ENGINEOIL_REMAINING; 
	signal_list_get_runningstatus[32] = genivi::wavd::dbus::get::RUNNINGSTATUS_ENGINEOIL_CHANGE; 
	signal_list_get_runningstatus[33] = genivi::wavd::dbus::get::RUNNINGSTATUS_ENGINEOIL_TEMP; 
	signal_list_get_runningstatus[34] = genivi::wavd::dbus::get::RUNNINGSTATUS_ENGINECOOLANT_LEVEL; 
	signal_list_get_runningstatus[35] = genivi::wavd::dbus::get::RUNNINGSTATUS_ENGINECOOLANT_TEMP; 
	signal_list_get_runningstatus[36] = genivi::wavd::dbus::get::RUNNINGSTATUS_STEERINGWHEELANGLE;
	signal_list_get_runningstatus[37] = NULL;

	signal_list_get_maintenance[0] = genivi::wavd::dbus::get::MAINTENANCE_ODOMETER; 
	signal_list_get_maintenance[1] = genivi::wavd::dbus::get::MAINTENANCE_TRANSMISSIONOIL_LIFELEVEL; 
	signal_list_get_maintenance[2] = genivi::wavd::dbus::get::MAINTENANCE_TRANSMISSIONOIL_TEMP; 
	signal_list_get_maintenance[3] = genivi::wavd::dbus::get::MAINTENANCE_BRAKEFLUIDLEVEL; 
	signal_list_get_maintenance[4] = genivi::wavd::dbus::get::MAINTENANCE_WASHERFLUIDLEVEL; 
	signal_list_get_maintenance[5] = genivi::wavd::dbus::get::MAINTENANCE_MALFUNCTIONINDICATORLAMP; 
	signal_list_get_maintenance[6] = genivi::wavd::dbus::get::MAINTENANCE_BATTERY_VOLTAGE; 
	signal_list_get_maintenance[7] = genivi::wavd::dbus::get::MAINTENANCE_BATTERY_CURRENT; 
	signal_list_get_maintenance[8] = genivi::wavd::dbus::get::MAINTENANCE_TIREPRESSURE_FRONTLEFT; 
	signal_list_get_maintenance[9] = genivi::wavd::dbus::get::MAINTENANCE_TIREPRESSURE_FRONTRIGHT; 
	signal_list_get_maintenance[10] = genivi::wavd::dbus::get::MAINTENANCE_TIREPRESSURE_REARLEFT; 
	signal_list_get_maintenance[11] = genivi::wavd::dbus::get::MAINTENANCE_TIREPRESSURE_REARRIGHT; 
	signal_list_get_maintenance[12] = genivi::wavd::dbus::get::MAINTENANCE_TIREPRESSURESTATUS_FRONTLEFT; 
	signal_list_get_maintenance[13] = genivi::wavd::dbus::get::MAINTENANCE_TIREPRESSURESTATUS_FRONTRIGHT; 
	signal_list_get_maintenance[14] = genivi::wavd::dbus::get::MAINTENANCE_TIREPRESSURESTATUS_REARLEFT; 
	signal_list_get_maintenance[15] = genivi::wavd::dbus::get::MAINTENANCE_TIREPRESSURESTATUS_REARRIGHT; 
	signal_list_get_maintenance[16] = NULL;

	signal_list_get_personalization[0] = genivi::wavd::dbus::get::PERSONALIZATION_KEYID; 
	signal_list_get_personalization[1] = genivi::wavd::dbus::get::PERSONALIZATION_LANGUAGE; 
	signal_list_get_personalization[2] = genivi::wavd::dbus::get::PERSONALIZATION_MEASUREMENTSYSTEM; 
	signal_list_get_personalization[3] = genivi::wavd::dbus::get::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_FUEL; 
	signal_list_get_personalization[4] = genivi::wavd::dbus::get::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_DISTANCE; 
	signal_list_get_personalization[5] = genivi::wavd::dbus::get::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_SPEED; 
	signal_list_get_personalization[6] = genivi::wavd::dbus::get::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_CONSUMPTION; 
	signal_list_get_personalization[7] = genivi::wavd::dbus::get::PERSONALIZATION_MIRROR_DRIVER; 
	signal_list_get_personalization[8] = genivi::wavd::dbus::get::PERSONALIZATION_MIRROR_PASSENGER; 
	signal_list_get_personalization[9] = genivi::wavd::dbus::get::PERSONALIZATION_MIRROR_INSIDE; 
	signal_list_get_personalization[10] = genivi::wavd::dbus::get::PERSONALIZATION_STEERINGWHEELPOSITION_SLIDE; 
	signal_list_get_personalization[11] = genivi::wavd::dbus::get::PERSONALIZATION_STEERINGWHEELPOSITION_TILT; 
	signal_list_get_personalization[12] = genivi::wavd::dbus::get::PERSONALIZATION_DRIVINGMODE; 
	signal_list_get_personalization[13] = genivi::wavd::dbus::get::PERSONALIZATION_DRIVERSEATPOSITION_RECLINESEATBACK; 
	signal_list_get_personalization[14] = genivi::wavd::dbus::get::PERSONALIZATION_DRIVERSEATPOSITION_SLIDE; 
	signal_list_get_personalization[15] = genivi::wavd::dbus::get::PERSONALIZATION_DRIVERSEATPOSITION_CUSHIONHEIGHT; 
	signal_list_get_personalization[16] = genivi::wavd::dbus::get::PERSONALIZATION_DRIVERSEATPOSITION_HEADREST; 
	signal_list_get_personalization[17] = genivi::wavd::dbus::get::PERSONALIZATION_DRIVERSEATPOSITION_BACKCUSHION; 
	signal_list_get_personalization[18] = genivi::wavd::dbus::get::PERSONALIZATION_DRIVERSEATPOSITION_SIDECUSHION; 
	signal_list_get_personalization[19] = genivi::wavd::dbus::get::PERSONALIZATION_PASSENGERSEATPOSITION_RECLINESEATBACK; 
	signal_list_get_personalization[20] = genivi::wavd::dbus::get::PERSONALIZATION_PASSENGERSEATPOSITION_SLIDE; 
	signal_list_get_personalization[21] = genivi::wavd::dbus::get::PERSONALIZATION_PASSENGERSEATPOSITION_CUSHIONHEIGHT; 
	signal_list_get_personalization[22] = genivi::wavd::dbus::get::PERSONALIZATION_PASSENGERSEATPOSITION_HEADREST; 
	signal_list_get_personalization[23] = genivi::wavd::dbus::get::PERSONALIZATION_PASSENGERSEATPOSITION_BACKCUSHION; 
	signal_list_get_personalization[24] = genivi::wavd::dbus::get::PERSONALIZATION_PASSENGERSEATPOSITION_SIDECUSHION; 
	signal_list_get_personalization[25] = genivi::wavd::dbus::get::PERSONALIZATION_DASHBOARDILLUMINATION; 
	signal_list_get_personalization[26] = genivi::wavd::dbus::get::PERSONALIZATION_GENERATEDVEHICLESOUNDMODE; 
	signal_list_get_personalization[27] = NULL;
	
	signal_list_get_drivingsafety[0] = genivi::wavd::dbus::get::DRIVINGSAFETY_ANTILOCKBRAKINGSYSTEM;
	signal_list_get_drivingsafety[1] = genivi::wavd::dbus::get::DRIVINGSAFETY_TRACTIONCONTROLSYSTEM;
	signal_list_get_drivingsafety[2] = genivi::wavd::dbus::get::DRIVINGSAFETY_ELECTRONICSTABILITYCONTROL;
	signal_list_get_drivingsafety[3] = genivi::wavd::dbus::get::DRIVINGSAFETY_VEHICLETOPSPEEDLIMIT;
	signal_list_get_drivingsafety[4] = genivi::wavd::dbus::get::DRIVINGSAFETY_AIRBAGSTATUS_DRIVER;
	signal_list_get_drivingsafety[5] = genivi::wavd::dbus::get::DRIVINGSAFETY_AIRBAGSTATUS_PASSENGER;
	signal_list_get_drivingsafety[6] = genivi::wavd::dbus::get::DRIVINGSAFETY_AIRBAGSTATUS_SIDE;
	signal_list_get_drivingsafety[7] = genivi::wavd::dbus::get::DRIVINGSAFETY_DOOROPENSTATUS_DRIVER;
	signal_list_get_drivingsafety[8] = genivi::wavd::dbus::get::DRIVINGSAFETY_DOOROPENSTATUS_PASSENGER;
	signal_list_get_drivingsafety[9] = genivi::wavd::dbus::get::DRIVINGSAFETY_DOOROPENSTATUS_REARLEFT;
	signal_list_get_drivingsafety[10] = genivi::wavd::dbus::get::DRIVINGSAFETY_DOOROPENSTATUS_REARRIGHT;
	signal_list_get_drivingsafety[11] = genivi::wavd::dbus::get::DRIVINGSAFETY_DOOROPENSTATUS_TRUNK;
	signal_list_get_drivingsafety[12] = genivi::wavd::dbus::get::DRIVINGSAFETY_DOOROPENSTATUS_FUELFILTERCAP;
	signal_list_get_drivingsafety[13] = genivi::wavd::dbus::get::DRIVINGSAFETY_DOOROPENSTATUS_HOOD;
	signal_list_get_drivingsafety[14] = genivi::wavd::dbus::get::DRIVINGSAFETY_DOORLOCKSTATUS_DRIVER;
	signal_list_get_drivingsafety[15] = genivi::wavd::dbus::get::DRIVINGSAFETY_DOORLOCKSTATUS_PASSENGER;
	signal_list_get_drivingsafety[16] = genivi::wavd::dbus::get::DRIVINGSAFETY_DOORLOCKSTATUS_REARLEFT;
	signal_list_get_drivingsafety[17] = genivi::wavd::dbus::get::DRIVINGSAFETY_DOORLOCKSTATUS_REARRIGHT;
	signal_list_get_drivingsafety[18] = genivi::wavd::dbus::get::DRIVINGSAFETY_CHILDSAFETYLOCK;
	signal_list_get_drivingsafety[19] = genivi::wavd::dbus::get::DRIVINGSAFETY_OCCUPANTSSTATUS_DRIVER;
	signal_list_get_drivingsafety[20] = genivi::wavd::dbus::get::DRIVINGSAFETY_OCCUPANTSSTATUS_PASSENGER;
	signal_list_get_drivingsafety[21] = genivi::wavd::dbus::get::DRIVINGSAFETY_OCCUPANTSSTATUS_REARLEFT;
	signal_list_get_drivingsafety[22] = genivi::wavd::dbus::get::DRIVINGSAFETY_OCCUPANTSSTATUS_REARRIGHT;
	signal_list_get_drivingsafety[23] = genivi::wavd::dbus::get::DRIVINGSAFETY_SEATBELT_DRIVER;
	signal_list_get_drivingsafety[24] = genivi::wavd::dbus::get::DRIVINGSAFETY_SEATBELT_PASSENGER;
	signal_list_get_drivingsafety[25] = genivi::wavd::dbus::get::DRIVINGSAFETY_SEATBELT_REARLEFT;
	signal_list_get_drivingsafety[26] = genivi::wavd::dbus::get::DRIVINGSAFETY_SEATBELT_REARRIGHT;
	signal_list_get_drivingsafety[27] = genivi::wavd::dbus::get::DRIVINGSAFETY_WINDOWLOCK_DRIVER;
	signal_list_get_drivingsafety[28] = genivi::wavd::dbus::get::DRIVINGSAFETY_WINDOWLOCK_PASSENGER;
	signal_list_get_drivingsafety[29] = genivi::wavd::dbus::get::DRIVINGSAFETY_WINDOWLOCK_REARLEFT;
	signal_list_get_drivingsafety[30] = genivi::wavd::dbus::get::DRIVINGSAFETY_WINDOWLOCK_REARRIGHT;
	signal_list_get_drivingsafety[31] = genivi::wavd::dbus::get::DRIVINGSAFETY_OBSTACLEDISTANCE_SENSORSTATUS;
	signal_list_get_drivingsafety[32] = genivi::wavd::dbus::get::DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTCENTER;
	signal_list_get_drivingsafety[33] = genivi::wavd::dbus::get::DRIVINGSAFETY_OBSTACLEDISTANCE_REARCENTER;
	signal_list_get_drivingsafety[34] = genivi::wavd::dbus::get::DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTLEFT;
	signal_list_get_drivingsafety[35] = genivi::wavd::dbus::get::DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTRIGHT;
	signal_list_get_drivingsafety[36] = genivi::wavd::dbus::get::DRIVINGSAFETY_OBSTACLEDISTANCE_MIDDLELEFT;
	signal_list_get_drivingsafety[37] = genivi::wavd::dbus::get::DRIVINGSAFETY_OBSTACLEDISTANCE_MIDDLERIGHT;
	signal_list_get_drivingsafety[38] = genivi::wavd::dbus::get::DRIVINGSAFETY_OBSTACLEDISTANCE_REARLEFT;
	signal_list_get_drivingsafety[39] = genivi::wavd::dbus::get::DRIVINGSAFETY_OBSTACLEDISTANCE_REARRIGHT;
	signal_list_get_drivingsafety[40] = genivi::wavd::dbus::get::DRIVINGSAFETY_FRONTCOLLISIONDETECTION_STATUS;
	signal_list_get_drivingsafety[41] = genivi::wavd::dbus::get::DRIVINGSAFETY_FRONTCOLLISIONDETECTION_DISTANCE;
	signal_list_get_drivingsafety[42] = genivi::wavd::dbus::get::DRIVINGSAFETY_FRONTCOLLISIONDETECTION_TIME;
	signal_list_get_drivingsafety[43] = NULL;
	
	signal_list_get_visionsystem[0] = genivi::wavd::dbus::get::VISIONSYSTEM_LANEDEPARTUREDETECTIONSTATUS;
	signal_list_get_visionsystem[1] = genivi::wavd::dbus::get::VISIONSYSTEM_LANEDEPARTED;
	signal_list_get_visionsystem[2] = NULL; 

	signal_list_get_parking[0] = genivi::wavd::dbus::get::PARKING_SECURITYALERT;
	signal_list_get_parking[1] = genivi::wavd::dbus::get::PARKING_PARKINGBRAKE;
	signal_list_get_parking[2] = genivi::wavd::dbus::get::PARKING_PARKINGLIGHTS;
	signal_list_get_parking[3] = NULL;

	signal_list_get_climateenvironment[0] = genivi::wavd::dbus::get::CLIMATEENVIRONMENT_INTERIORTEMP;
	signal_list_get_climateenvironment[1] = genivi::wavd::dbus::get::CLIMATEENVIRONMENT_EXTERIORTEMP;
	signal_list_get_climateenvironment[2] = genivi::wavd::dbus::get::CLIMATEENVIRONMENT_EXTERIORBRIGHTNESS;
	signal_list_get_climateenvironment[3] = genivi::wavd::dbus::get::CLIMATEENVIRONMENT_RAINSENSOR;
	signal_list_get_climateenvironment[4] = genivi::wavd::dbus::get::CLIMATEENVIRONMENT_WINDSHIELDWIPER;
	signal_list_get_climateenvironment[5] = genivi::wavd::dbus::get::CLIMATEENVIRONMENT_REARWIPER;
	signal_list_get_climateenvironment[6] = genivi::wavd::dbus::get::CLIMATEENVIRONMENT_HVACFAN_DIRECTION;
	signal_list_get_climateenvironment[7] = genivi::wavd::dbus::get::CLIMATEENVIRONMENT_HVACFAN_SPEED;
	signal_list_get_climateenvironment[8] = genivi::wavd::dbus::get::CLIMATEENVIRONMENT_HVACFAN_TARGETTEMP;
	signal_list_get_climateenvironment[9] = genivi::wavd::dbus::get::CLIMATEENVIRONMENT_AIRCONDITIONING;
	signal_list_get_climateenvironment[10] = genivi::wavd::dbus::get::CLIMATEENVIRONMENT_AIRRECIRCULATION;
	signal_list_get_climateenvironment[11] = genivi::wavd::dbus::get::CLIMATEENVIRONMENT_HEATER;
	signal_list_get_climateenvironment[12] = genivi::wavd::dbus::get::CLIMATEENVIRONMENT_DEFROST_WINDSHIELD;
	signal_list_get_climateenvironment[13] = genivi::wavd::dbus::get::CLIMATEENVIRONMENT_DEFROST_REARWINDOW;
	signal_list_get_climateenvironment[14] = genivi::wavd::dbus::get::CLIMATEENVIRONMENT_DEFROST_SIDEMIRRORS;
	signal_list_get_climateenvironment[15] = genivi::wavd::dbus::get::CLIMATEENVIRONMENT_STEERINGWHEELHEATER;
	signal_list_get_climateenvironment[16] = genivi::wavd::dbus::get::CLIMATEENVIRONMENT_SEATHEATER;
	signal_list_get_climateenvironment[17] = genivi::wavd::dbus::get::CLIMATEENVIRONMENT_SEATCOOLER;
	signal_list_get_climateenvironment[18] = genivi::wavd::dbus::get::CLIMATEENVIRONMENT_WINDOW_DRIVER;
	signal_list_get_climateenvironment[19] = genivi::wavd::dbus::get::CLIMATEENVIRONMENT_WINDOW_PASSENGER;
	signal_list_get_climateenvironment[20] = genivi::wavd::dbus::get::CLIMATEENVIRONMENT_WINDOW_REARLEFT;
	signal_list_get_climateenvironment[21] = genivi::wavd::dbus::get::CLIMATEENVIRONMENT_WINDOW_REARRIGHT;
	signal_list_get_climateenvironment[22] = genivi::wavd::dbus::get::CLIMATEENVIRONMENT_SUNROOF_OPENNESS;
	signal_list_get_climateenvironment[23] = genivi::wavd::dbus::get::CLIMATEENVIRONMENT_SUNROOF_TILT;
	signal_list_get_climateenvironment[24] = genivi::wavd::dbus::get::CLIMATEENVIRONMENT_CONVERTIBLEROOF;
	signal_list_get_climateenvironment[25] = NULL; 
}

VehicleInterfaceAdapter::~VehicleInterfaceAdapter()
{

}

void VehicleInterfaceAdapter::setVehicleEventsPtr(const VehicleEventsPtr& m_vehicleEvents)
{
	this->m_vehicleEvents = m_vehicleEvents;
}

void VehicleInterfaceAdapter::setVehicleInterfacePtr(const VehicleInterfacePtr& m_vehicleInterface)
{
	this->m_vehicleInterface = m_vehicleInterface;
}

void VehicleInterfaceAdapter::startDBusReceiver()
{
	dbus_receiver.init_dbus();
	dbus_receiver.init_signal_filter(8);
	dbus_receiver.add_signal_filter(0, DBUS_PATH, DBUS_INTERFACE, signal_list_get_vehicleinfo, processSignal);
	dbus_receiver.add_signal_filter(1, DBUS_PATH, DBUS_INTERFACE, signal_list_get_runningstatus, processSignal);
	dbus_receiver.add_signal_filter(2, DBUS_PATH, DBUS_INTERFACE, signal_list_get_maintenance, processSignal);
	dbus_receiver.add_signal_filter(3, DBUS_PATH, DBUS_INTERFACE, signal_list_get_personalization, processSignal);
	dbus_receiver.add_signal_filter(4, DBUS_PATH, DBUS_INTERFACE, signal_list_get_drivingsafety, processSignal);
	dbus_receiver.add_signal_filter(5, DBUS_PATH, DBUS_INTERFACE, signal_list_get_visionsystem, processSignal);
	dbus_receiver.add_signal_filter(6, DBUS_PATH, DBUS_INTERFACE, signal_list_get_parking, processSignal);
	dbus_receiver.add_signal_filter(7, DBUS_PATH, DBUS_INTERFACE, signal_list_get_climateenvironment, processSignal);
	dbus_receiver.listen_signal();
}

void VehicleInterfaceAdapter::stopDBusReceiver()
{
	dbus_receiver.stop_listening();
}

void VehicleInterfaceAdapter::processSignal(DBusMessage* message,
											const char* signal_name,
											int index1,
											int index2)
{
	DBusError error;
	dbus_error_init(&error);

	switch (index1)
	{
	case 0:
		{
			VehicleInfoEventPtr m_event = boost::dynamic_pointer_cast<VehicleInfoEvent>(m_vehicleEvents->getVehicleEvent(VehicleInfoEvent::VEHICLEINFO));

			switch (index2)
			{
			case 0:
				{
					DOMString type = VehicleInfoEvent::VEHICLEINFO_WMI;
					char* value;
					dbus_message_get_args(message, &error, DBUS_TYPE_STRING, &value, DBUS_TYPE_INVALID);
					DOMString valueStr(value);

					m_event->setType(type);
					m_event->setWmi(valueStr);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 1:
				{
					DOMString type = VehicleInfoEvent::VEHICLEINFO_VIN;
					char* value;
					dbus_message_get_args(message, &error, DBUS_TYPE_STRING, &value, DBUS_TYPE_INVALID);
					DOMString valueStr(value);

					m_event->setType(type);
					m_event->setVin(valueStr);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 2:
				{
					DOMString type = VehicleInfoEvent::VEHICLEINFO_VEHICLETYPE;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setVehicleType(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 3:
				{
					DOMString type = VehicleInfoEvent::VEHICLEINFO_DOORTYPE_1STROW;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDoorType1stRow(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 4:
				{
					DOMString type = VehicleInfoEvent::VEHICLEINFO_DOORTYPE_2NDROW;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDoorType2ndRow(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 5:
				{
					DOMString type = VehicleInfoEvent::VEHICLEINFO_DOORTYPE_3RDROW;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDoorType3rdRow(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 6:
				{
					DOMString type = VehicleInfoEvent::VEHICLEINFO_FUELTYPE;
					octet value;
					dbus_message_get_args(message, &error, DBUS_TYPE_BYTE, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setFuelType(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 7:
				{
					DOMString type = VehicleInfoEvent::VEHICLEINFO_TRANSMISSIONGEARTYPE;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setTransmissionGearType(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 8:
				{
					DOMString type = VehicleInfoEvent::VEHICLEINFO_WHEELINFO_RADIUS;
					double value;
					dbus_message_get_args(message, &error, DBUS_TYPE_DOUBLE, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setWheelInfoRadius(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 9:
				{
					DOMString type = VehicleInfoEvent::VEHICLEINFO_WHEELINFO_TRACK;
					double value;
					dbus_message_get_args(message, &error, DBUS_TYPE_DOUBLE, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setWheelInfoTrack(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			}
			break;
		}
	case 1:
		{
			RunningStatusEventPtr m_event = boost::dynamic_pointer_cast<RunningStatusEvent>(m_vehicleEvents->getVehicleEvent(RunningStatusEvent::RUNNINGSTATUS));
	
			switch (index2)
			{
			case 0:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_VEHICLEPOWERMODE;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setVehiclePowerMode(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 1:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_SPEEDOMETER;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setSpeedometer(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 2:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_ENGINESPEED;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setEngineSpeed(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 3:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_TRIPMETER_1_MILEAGE;
					unsigned long value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT64, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setTripMeter1Mileage(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 4:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_TRIPMETER_2_MILEAGE;
					unsigned long value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT64, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setTripMeter2Mileage(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 5:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_TRIPMETER_1_AVERAGESPEED;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setTripMeter1AverageSpeed(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 6:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_TRIPMETER_2_AVERAGESPEED;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setTripMeter2AverageSpeed(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 7:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_TRIPMETER_1_FUELCONSUMPTION;
					unsigned long value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT64, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setTripMeter1FuelConsumption(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 8:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_TRIPMETER_2_FUELCONSUMPTION;
					unsigned long value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT64, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setTripMeter2FuelConsumption(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 9:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_TRANSMISSIONGEARSTATUS;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setTransmissionGearStatus(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 10:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_CRUISECONTROL_STATUS;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setCruiseControlStatus(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 11:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_CRUISECONTROL_SPEED;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setCruiseControlSpeed(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 12:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_WHEELBRAKE;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setWheelBrake(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 13:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_LIGHTSSTATUS_HEAD;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setLightsStatusHead(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 14:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_LIGHTSSTATUS_HIGHBEAM;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setLightsStatusHighBeam(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 15:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_LIGHTSSTATUS_TURNLEFT;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setLightsStatusTurnLeft(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 16:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_LIGHTSSTATUS_TURNRIGHT;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setLightsStatusTurnRight(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 17:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_LIGHTSSTATUS_BRAKE;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setLightsStatusBrake(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 18:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_LIGHTSSTATUS_FOGFRONT;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setLightsStatusFogFront(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 19:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_LIGHTSSTATUS_FOGREAR;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setLightsStatusFogRear(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 20:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_LIGHTSSTATUS_HAZARD;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setLightsStatusHazard(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 21:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_LIGHTSSTATUS_PARKING;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setLightsStatusParking(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 22:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_INTERIORLIGHTSSTATUS_DRIVER;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setInteriorLightsStatusDriver(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 23:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_INTERIORLIGHTSSTATUS_PASSENGER;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setInteriorLightsStatusPassenger(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 24:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_INTERIORLIGHTSSTATUS_CENTER;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setInteriorLightsStatusCenter(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 25:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_AUTOMATICHEADLIGHTS;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setAutomaticHeadlights(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 26:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_DYNAMICHIGHBEAM;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDynamicHighBeam(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 27:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_HORN;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setHorn(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 28:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_CHIME;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setChime(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 29:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_FUEL;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setFuel(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 30:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_ESTIMATEDRANGE;
					unsigned long value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT64, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setEstimatedRange(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 31:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_ENGINEOIL_REMAINING;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setEngineOilRemaining(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 32:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_ENGINEOIL_CHANGE;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setEngineOilChange(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 33:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_ENGINEOIL_TEMP;
					short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_INT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setEngineOilTemp(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 34:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_ENGINECOOLANT_LEVEL;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setEngineCoolantLevel(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 35:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_ENGINECOOLANT_TEMP;
					short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_INT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setEngineCoolantTemp(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 36:
				{
					DOMString type = RunningStatusEvent::RUNNINGSTATUS_STEERINGWHEELANGLE;
					short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setSteeringWheelAngle(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			}
			break;
		}
	case 2:
		{
			MaintenanceEventPtr m_event = boost::dynamic_pointer_cast<MaintenanceEvent>(m_vehicleEvents->getVehicleEvent(MaintenanceEvent::MAINTENANCE));

			switch (index2)
			{
			case 0:
				{
					DOMString type = MaintenanceEvent::MAINTENANCE_ODOMETER;
					unsigned long value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT64, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setOdometer(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 1:
				{
					DOMString type = MaintenanceEvent::MAINTENANCE_TRANSMISSIONOIL_LIFELEVEL;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setTransmissionOilLifeLevel(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 2:
				{
					DOMString type = MaintenanceEvent::MAINTENANCE_TRANSMISSIONOIL_TEMP;
					short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_INT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setTransmissionOilTemp(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 3:
				{
					DOMString type = MaintenanceEvent::MAINTENANCE_BRAKEFLUIDLEVEL;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setBrakeFluidLevel(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 4:
				{
					DOMString type = MaintenanceEvent::MAINTENANCE_WASHERFLUIDLEVEL;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setWasherFluidLevel(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 5:
				{
					DOMString type = MaintenanceEvent::MAINTENANCE_MALFUNCTIONINDICATORLAMP;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setMalfunctionIndicatorLamp(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 6:
				{
					DOMString type = MaintenanceEvent::MAINTENANCE_BATTERY_VOLTAGE;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setBatteryVoltage(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 7:
				{
					DOMString type = MaintenanceEvent::MAINTENANCE_BATTERY_CURRENT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setBatteryCurrent(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 8:
				{
					DOMString type = MaintenanceEvent::MAINTENANCE_TIREPRESSURE_FRONTLEFT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setTirePressureFrontLeft(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 9:
				{
					DOMString type = MaintenanceEvent::MAINTENANCE_TIREPRESSURE_FRONTRIGHT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setTirePressureFrontRight(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 10:
				{
					DOMString type = MaintenanceEvent::MAINTENANCE_TIREPRESSURE_REARLEFT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setTirePressureRearLeft(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 11:
				{
					DOMString type = MaintenanceEvent::MAINTENANCE_TIREPRESSURE_REARRIGHT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setTirePressureRearRight(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 12:
				{
					DOMString type = MaintenanceEvent::MAINTENANCE_TIREPRESSURESTATUS_FRONTLEFT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setTirePressureStatusFrontLeft(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 13:
				{
					DOMString type = MaintenanceEvent::MAINTENANCE_TIREPRESSURESTATUS_FRONTRIGHT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setTirePressureStatusFrontRight(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 14:
				{
					DOMString type = MaintenanceEvent::MAINTENANCE_TIREPRESSURESTATUS_REARLEFT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setTirePressureStatusRearLeft(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 15:
				{
					DOMString type = MaintenanceEvent::MAINTENANCE_TIREPRESSURESTATUS_REARRIGHT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setTirePressureStatusRearRight(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			}
			break;
		}
	case 3:
		{
			PersonalizationEventPtr m_event = boost::dynamic_pointer_cast<PersonalizationEvent>(m_vehicleEvents->getVehicleEvent(PersonalizationEvent::PERSONALIZATION));
	
			switch (index2)
			{
			case 0:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_KEYID;
					char* value;
					dbus_message_get_args(message, &error, DBUS_TYPE_STRING, &value, DBUS_TYPE_INVALID);
					DOMString valueStr(value);

					m_event->setType(type);
					m_event->setKeyId(valueStr);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 1:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_LANGUAGE;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setLanguage(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 2:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_MEASUREMENTSYSTEM;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setMeasurementSystem(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 3:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_FUEL;
					char* value;
					dbus_message_get_args(message, &error, DBUS_TYPE_STRING, &value, DBUS_TYPE_INVALID);
					DOMString valueStr(value);

					m_event->setType(type);
					m_event->setMeasurementSystemStringFuel(valueStr);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 4:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_DISTANCE;
					char* value;
					dbus_message_get_args(message, &error, DBUS_TYPE_STRING, &value, DBUS_TYPE_INVALID);
					DOMString valueStr(value);

					m_event->setType(type);
					m_event->setMeasurementSystemStringDistance(valueStr);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 5:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_SPEED;
					char* value;
					dbus_message_get_args(message, &error, DBUS_TYPE_STRING, &value, DBUS_TYPE_INVALID);
					DOMString valueStr(value);

					m_event->setType(type);
					m_event->setMeasurementSystemStringSpeed(valueStr);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 6:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_CONSUMPTION;
					char* value;
					dbus_message_get_args(message, &error, DBUS_TYPE_STRING, &value, DBUS_TYPE_INVALID);
					DOMString valueStr(value);

					m_event->setType(type);
					m_event->setMeasurementSystemStringConsumption(valueStr);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 7:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_MIRROR_DRIVER;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setMirrorDriver(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 8:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_MIRROR_PASSENGER;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setMirrorPassenger(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 9:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_MIRROR_INSIDE;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setMirrorInside(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 10:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_STEERINGWHEELPOSITION_SLIDE;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setSteeringWheelPositionSlide(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 11:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_STEERINGWHEELPOSITION_TILT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setSteeringWheelPositionTilt(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 12:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_DRIVINGMODE;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDrivingMode(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 13:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION_RECLINESEATBACK;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDriverSeatPositionReclineSeatback(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 14:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION_SLIDE;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDriverSeatPositionSlide(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 15:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION_CUSHIONHEIGHT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDriverSeatPositionCushionHeight(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 16:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION_HEADREST;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDriverSeatPositionHeadrest(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 17:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION_BACKCUSHION;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDriverSeatPositionBackCushion(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 18:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION_SIDECUSHION;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDriverSeatPositionSideCushion(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 19:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION_RECLINESEATBACK;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setPassengerSeatPositionReclineSeatback(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 20:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION_SLIDE;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setPassengerSeatPositionSlide(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 21:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION_CUSHIONHEIGHT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setPassengerSeatPositionCushionHeight(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 22:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION_HEADREST;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setPassengerSeatPositionHeadrest(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 23:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION_BACKCUSHION;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setPassengerSeatPositionBackCushion(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 24:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION_SIDECUSHION;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setPassengerSeatPositionSideCushion(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 25:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_DASHBOARDILLUMINATION;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDashboardIllumination(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 26:
				{
					DOMString type = PersonalizationEvent::PERSONALIZATION_GENERATEDVEHICLESOUNDMODE;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setGeneratedVehicleSoundMode(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			}
			break;
		}
	case 4:
		{
			DrivingSafetyEventPtr m_event = boost::dynamic_pointer_cast<DrivingSafetyEvent>(m_vehicleEvents->getVehicleEvent(DrivingSafetyEvent::DRIVINGSAFETY));

			switch (index2)
			{
			case 0:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_ANTILOCKBRAKINGSYSTEM;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setAntilockBrakingSystem(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 1:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_TRACTIONCONTROLSYSTEM;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setTractionControlSystem(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 2:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_ELECTRONICSTABILITYCONTROL;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setElectronicStabilityControl(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 3:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_VEHICLETOPSPEEDLIMIT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setVehicleTopSpeedLimit(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 4:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_AIRBAGSTATUS_DRIVER;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setAirbagStatusDriver(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 5:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_AIRBAGSTATUS_PASSENGER;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setAirbagStatusPassenger(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 6:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_AIRBAGSTATUS_SIDE;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setAirbagStatusSide(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 7:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_DOOROPENSTATUS_DRIVER;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDoorOpenStatusDriver(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 8:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_DOOROPENSTATUS_PASSENGER;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDoorOpenStatusPassenger(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 9:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_DOOROPENSTATUS_REARLEFT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDoorOpenStatusRearLeft(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 10:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_DOOROPENSTATUS_REARRIGHT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDoorOpenStatusRearRight(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 11:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_DOOROPENSTATUS_TRUNK;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDoorOpenStatusTrunk(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 12:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_DOOROPENSTATUS_FUELFILTERCAP;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDoorOpenStatusFuelFilterCap(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 13:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_DOOROPENSTATUS_HOOD;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDoorOpenStatusHood(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 14:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_DOORLOCKSTATUS_DRIVER;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDoorLockStatusDriver(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 15:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_DOORLOCKSTATUS_PASSENGER;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDoorLockStatusPassenger(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 16:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_DOORLOCKSTATUS_REARLEFT;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDoorLockStatusRearLeft(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 17:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_DOORLOCKSTATUS_REARRIGHT;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDoorLockStatusRearRight(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 18:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_CHILDSAFETYLOCK;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setChildSafetyLock(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 19:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_OCCUPANTSSTATUS_DRIVER;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setOccupantsStatusDriver(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 20:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_OCCUPANTSSTATUS_PASSENGER;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setOccupantsStatusPassenger(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 21:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_OCCUPANTSSTATUS_REARLEFT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setOccupantsStatusRearLeft(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 22:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_OCCUPANTSSTATUS_REARRIGHT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setOccupantsStatusRearRight(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 23:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_SEATBELT_DRIVER;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setSeatBeltDriver(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 24:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_SEATBELT_PASSENGER;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setSeatBeltPassenger(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 25:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_SEATBELT_REARLEFT;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setSeatBeltRearLeft(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 26:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_SEATBELT_REARRIGHT;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setSeatBeltRearRight(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 27:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_WINDOWLOCK_DRIVER;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setWindowLockDriver(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 28:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_WINDOWLOCK_PASSENGER;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setWindowLockPassenger(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 29:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_WINDOWLOCK_REARLEFT;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setWindowLockRearLeft(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 30:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_WINDOWLOCK_REARRIGHT;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setWindowLockRearRight(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 31:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_OBSTACLEDISTANCE_SENSORSTATUS;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setObstacleDistanceSensorStatus(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 32:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTCENTER;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setObstacleDistanceFrontCenter(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 33:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_OBSTACLEDISTANCE_REARCENTER;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setObstacleDistanceRearCenter(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 34:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTLEFT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setObstacleDistanceFrontLeft(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 35:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTRIGHT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setObstacleDistanceFrontRight(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 36:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_OBSTACLEDISTANCE_MIDDLELEFT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setObstacleDistanceMiddleLeft(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 37:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_OBSTACLEDISTANCE_MIDDLERIGHT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setObstacleDistanceMiddleRight(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 38:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_OBSTACLEDISTANCE_REARLEFT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setObstacleDistanceRearLeft(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 39:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_OBSTACLEDISTANCE_REARRIGHT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setObstacleDistanceRearRight(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 40:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_FRONTCOLLISIONDETECTION_STATUS;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setFrontCollisionDetectionStatus(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 41:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_FRONTCOLLISIONDETECTION_DISTANCE;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setFrontCollisionDetectionDistance(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 42:
				{
					DOMString type = DrivingSafetyEvent::DRIVINGSAFETY_FRONTCOLLISIONDETECTION_TIME;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setFrontCollisionDetectionTime(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			}
			break;
		}
	case 5:
		{
			VisionSystemEventPtr m_event = boost::dynamic_pointer_cast<VisionSystemEvent>(m_vehicleEvents->getVehicleEvent(VisionSystemEvent::VISIONSYSTEM));

			switch (index2)
			{
			case 0:
				{
					DOMString type = VisionSystemEvent::VISIONSYSTEM_LANEDEPARTUREDETECTIONSTATUS;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setLaneDepartureDetectionStatus(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 1:
				{
					DOMString type = VisionSystemEvent::VISIONSYSTEM_LANEDEPARTED;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setLaneDeparted(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			}
		}
	case 6:
		{
			ParkingEventPtr m_event = boost::dynamic_pointer_cast<ParkingEvent>(m_vehicleEvents->getVehicleEvent(ParkingEvent::PARKING));

			switch (index2)
			{
			case 0:
				{
					DOMString type = ParkingEvent::PARKING_SECURITYALERT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setSecurityAlert(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 1:
				{
					DOMString type = ParkingEvent::PARKING_PARKINGBRAKE;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setParkingBrake(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 2:
				{
					DOMString type = ParkingEvent::PARKING_PARKINGLIGHTS;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setParkingLights(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			}
		}
	case 7:
		{
			ClimateEnvironmentEventPtr m_event = boost::dynamic_pointer_cast<ClimateEnvironmentEvent>(m_vehicleEvents->getVehicleEvent(ClimateEnvironmentEvent::CLIMATEENVIRONMENT));

			switch (index2)
			{
			case 0:
				{
					DOMString type = ClimateEnvironmentEvent::CLIMATEENVIRONMENT_INTERIORTEMP;
					short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_INT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setInteriorTemp(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 1:
				{
					DOMString type = ClimateEnvironmentEvent::CLIMATEENVIRONMENT_EXTERIORTEMP;
					short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_INT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setExteriorTemp(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 2:
				{
					DOMString type = ClimateEnvironmentEvent::CLIMATEENVIRONMENT_EXTERIORBRIGHTNESS;
					unsigned long value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT64, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setExteriorBrightness(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 3:
				{
					DOMString type = ClimateEnvironmentEvent::CLIMATEENVIRONMENT_RAINSENSOR;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setRainSensor(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 4:
				{
					DOMString type = ClimateEnvironmentEvent::CLIMATEENVIRONMENT_WINDSHIELDWIPER;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setWindshieldWiper(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 5:
				{
					DOMString type = ClimateEnvironmentEvent::CLIMATEENVIRONMENT_REARWIPER;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setRearWiper(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 6:
				{
					DOMString type = ClimateEnvironmentEvent::CLIMATEENVIRONMENT_HVACFAN_DIRECTION;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setHvacFanDirection(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 7:
				{
					DOMString type = ClimateEnvironmentEvent::CLIMATEENVIRONMENT_HVACFAN_SPEED;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setHvacFanSpeed(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 8:
				{
					DOMString type = ClimateEnvironmentEvent::CLIMATEENVIRONMENT_HVACFAN_TARGETTEMP;
					short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_INT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setHvacFanTargetTemp(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 9:
				{
					DOMString type = ClimateEnvironmentEvent::CLIMATEENVIRONMENT_AIRCONDITIONING;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setAirConditioning(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 10:
				{
					DOMString type = ClimateEnvironmentEvent::CLIMATEENVIRONMENT_AIRRECIRCULATION;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setAirRecirculation(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 11:
				{
					DOMString type = ClimateEnvironmentEvent::CLIMATEENVIRONMENT_HEATER;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setHeater(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 12:
				{
					DOMString type = ClimateEnvironmentEvent::CLIMATEENVIRONMENT_DEFROST_WINDSHIELD;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDefrostWindshield(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 13:
				{
					DOMString type = ClimateEnvironmentEvent::CLIMATEENVIRONMENT_DEFROST_REARWINDOW;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDefrostRearWindow(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 14:
				{
					DOMString type = ClimateEnvironmentEvent::CLIMATEENVIRONMENT_DEFROST_SIDEMIRRORS;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setDefrostSideMirrors(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 15:
				{
					DOMString type = ClimateEnvironmentEvent::CLIMATEENVIRONMENT_STEERINGWHEELHEATER;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setSteeringWheelHeater(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 16:
				{
					DOMString type = ClimateEnvironmentEvent::CLIMATEENVIRONMENT_SEATHEATER;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setSeatHeater(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 17:
				{
					DOMString type = ClimateEnvironmentEvent::CLIMATEENVIRONMENT_SEATCOOLER;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setSeatCooler(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 18:
				{
					DOMString type = ClimateEnvironmentEvent::CLIMATEENVIRONMENT_WINDOW_DRIVER;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setWindowDriver(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 19:
				{
					DOMString type = ClimateEnvironmentEvent::CLIMATEENVIRONMENT_WINDOW_PASSENGER;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setWindowPassenger(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 20:
				{
					DOMString type = ClimateEnvironmentEvent::CLIMATEENVIRONMENT_WINDOW_REARLEFT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setWindowRearLeft(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 21:
				{
					DOMString type = ClimateEnvironmentEvent::CLIMATEENVIRONMENT_WINDOW_REARRIGHT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setWindowRearRight(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 22:
				{
					DOMString type = ClimateEnvironmentEvent::CLIMATEENVIRONMENT_SUNROOF_OPENNESS;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setSunroofOpenness(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 23:
				{
					DOMString type = ClimateEnvironmentEvent::CLIMATEENVIRONMENT_SUNROOF_TILT;
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setSunroofTilt(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			case 24:
				{
					DOMString type = ClimateEnvironmentEvent::CLIMATEENVIRONMENT_CONVERTIBLEROOF;
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);

					m_event->setType(type);
					m_event->setConvertibleRoof(value);
					m_event->setValidity(type, true);

					fireAllEvents(type, m_event);
					break;
				}
			}
		}
	}

	dbus_error_free(&error);
}

void VehicleInterfaceAdapter::fireAllEvents(const std::string& type, VehicleEventPtr ptr)
{
	std::string str = type;
	size_t pos;

	while ((pos = str.find_last_of('_')) != std::string::npos)
	{
		str = str.substr(0, pos);
		m_vehicleInterface->fireEvent(str, FB::variant_list_of(ptr));
	}
}
