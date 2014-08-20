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
 * @file VehicleInterface.h
 * For further information, see http://www.genivi.org/
 */

#ifndef H_VehicleInterface
#define H_VehicleInterface

#include <string>
#include <sstream>
#include "JSAPIAuto.h"
#include "BrowserHost.h"

#include "FBPointers.h"
#include "TypeDef.h"
#include "VehicleEvents.h"
#include "VehicleInterfaceAdapter.h"
#include "VehicleError.h"

FB_FORWARD_PTR(VehicleEvents);
FB_FORWARD_PTR(VehicleInterfaceAdapter);

class VehicleInterface : public FB::JSAPIAuto
{
public:
	VehicleInterface(const FB::BrowserHostPtr& host);
	virtual ~VehicleInterface();

	void setVehicleEventsPtr(const VehicleEventsPtr& m_vehicleEvents);
	void setVehicleInterfaceAdapterPtr(const VehicleInterfaceAdapterPtr& m_vehicleInterfaceAdapter);

	void get(VehicleEventType type,
			 const FB::JSObjectPtr& handler,
			 const FB::JSObjectPtr& errorCB);
	void set(VehicleEventType type,
			 const FB::JSObjectPtr& data,
			 const FB::JSObjectPtr& successCB,
			 const FB::JSObjectPtr& errorCB);
	FB::VariantList getSupportedEventTypes(VehicleEventType type, bool writable);

	void fireEvent(const std::string& eventName, FB::detail::VariantListInserter inserter);

	FB_JSAPI_EVENT(vehicleinfo, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(vehicleinfo_wmi, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(vehicleinfo_vin, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(vehicleinfo_vehicletype, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(vehicleinfo_doortype, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(vehicleinfo_doortype_1strow, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(vehicleinfo_doortype_2ndrow, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(vehicleinfo_doortype_3rdrow, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(vehicleinfo_fueltype, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(vehicleinfo_transmissiongeartype, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(vehicleinfo_wheelinfo, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(vehicleinfo_wheelinfo_radius, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(vehicleinfo_wheelinfo_track, 1, (FB::JSAPIPtr));

	FB_JSAPI_EVENT(runningstatus, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_vehiclepowermode, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_speedometer, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_enginespeed, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_tripmeter, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_tripmeter_1, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_tripmeter_2, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_tripmeter_1_mileage, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_tripmeter_2_mileage, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_tripmeter_1_averagespeed, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_tripmeter_2_averagespeed, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_tripmeter_1_fuelconsumption, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_tripmeter_2_fuelconsumption, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_transmissiongearstatus, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_cruisecontrol, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_cruisecontrol_status, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_cruisecontrol_speed, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_wheelbrake, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_lightsstatus, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_lightsstatus_head, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_lightsstatus_highbeam, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_lightsstatus_turnleft, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_lightsstatus_turnright, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_lightsstatus_brake, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_lightsstatus_fogfront, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_lightsstatus_fogrear, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_lightsstatus_hazard, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_lightsstatus_parking, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_interiorlightsstatus, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_interiorlightsstatus_driver, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_interiorlightsstatus_passenger, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_interiorlightsstatus_center, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_automaticheadlights, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_dynamichighbeam, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_horn, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_chime, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_fuel, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_estimatedrange, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_engineoil, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_engineoil_remaining, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_engineoil_change, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_engineoil_temp, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_enginecoolant_level, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_enginecoolant_temp, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(runningstatus_steeringwheelangle, 1, (FB::JSAPIPtr));

	FB_JSAPI_EVENT(maintenance, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(maintenance_odometer, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(maintenance_transmissionoil, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(maintenance_transmissionoil_lifelevel, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(maintenance_transmissionoil_temp, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(maintenance_brakefluidlevel, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(maintenance_washerfluidlevel, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(maintenance_malfunctionindicatorlamp, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(maintenance_battery, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(maintenance_battery_voltage, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(maintenance_battery_current, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(maintenance_tirepressure, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(maintenance_tirepressure_frontleft, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(maintenance_tirepressure_frontright, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(maintenance_tirepressure_rearleft, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(maintenance_tirepressure_rearright, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(maintenance_tirepressurestatus, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(maintenance_tirepressurestatus_frontleft, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(maintenance_tirepressurestatus_frontright, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(maintenance_tirepressurestatus_rearleft, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(maintenance_tirepressurestatus_rearright, 1, (FB::JSAPIPtr));

	FB_JSAPI_EVENT(personalization, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_keyid, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_language, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_measurementsystem, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_measurementsystemstring, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_measurementsystemstring_fuel, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_measurementsystemstring_distance, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_measurementsystemstring_speed, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_measurementsystemstring_consumption, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_mirror, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_mirror_driver, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_mirror_passenger, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_mirror_inside, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_steeringwheelposition, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_steeringwheelposition_slide, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_steeringwheelposition_tilt, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_driving_mode, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_driverseatposition, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_driverseatposition_reclineseatback, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_driverseatposition_slide, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_driverseatposition_cushionheight, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_driverseatposition_headrest, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_driverseatposition_backcushion, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_driverseatposition_sidecushion, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_passengerseatposition, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_passengerseatposition_reclineseatback, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_passengerseatposition_slide, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_passengerseatposition_cushionheight, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_passengerseatposition_headrest, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_passengerseatposition_backcushion, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_passengerseatposition_sidecushion, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_dashboardillumination, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(personalization_generatedvehiclesoundmode, 1, (FB::JSAPIPtr));

	FB_JSAPI_EVENT(drivingsafety, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_antilockbrakingsystem, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_tractioncontrolsystem, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_electronicstabilitycontrol, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_vehicletopspeedlimit, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_airbagstatus, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_airbagstatus_driver, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_airbagstatus_passenger, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_airbagstatus_side, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_dooropenstatus, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_dooropenstatus_driver, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_dooropenstatus_passenger, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_dooropenstatus_rearleft, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_dooropenstatus_rearright, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_dooropenstatus_trunk, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_dooropenstatus_fuelfiltercap, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_dooropenstatus_hood, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_doorlockstatus, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_doorlockstatus_driver, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_doorlockstatus_passenger, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_doorlockstatus_rearleft, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_doorlockstatus_rearright, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_childsafetylock, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_occupantsstatus, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_occupantsstatus_driver, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_occupantsstatus_passenger, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_occupantsstatus_rearleft, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_occupantsstatus_rearright, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_seatbelt, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_seatbelt_driver, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_seatbelt_passenger, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_seatbelt_rearleft, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_seatbelt_rearright, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_windowlock, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_windowlock_driver, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_windowlock_passenger, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_windowlock_rearleft, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_windowlock_rearright, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_obstacledistance, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_obstacledistance_sensorstatus, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_obstacledistance_frontcenter, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_obstacledistance_rearcenter, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_obstacledistance_frontleft, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_obstacledistance_frontright, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_obstacledistance_middleleft, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_obstacledistance_middleright, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_obstacledistance_rearleft, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_obstacledistance_rearright, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_frontcollisiondetection, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_frontcollisiondetection_status, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_frontcollisiondetection_distance, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(drivingsafety_frontcollisiondetection_time, 1, (FB::JSAPIPtr));

	FB_JSAPI_EVENT(visionsystem, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(visionsystem_lanedeparturedetectionstatus, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(visionsystem_lanedeparted, 1, (FB::JSAPIPtr));

	FB_JSAPI_EVENT(parking, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(parking_securityalert, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(parking_parkingbrake, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(parking_parkinglights, 1, (FB::JSAPIPtr));

	FB_JSAPI_EVENT(climateenvironment, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_interiortemp, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_exteriortemp, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_exteriorbrightness, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_rainsensor, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_windshield_wiper, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_rearwiper, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_hvacfan, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_hvacfan_direction, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_hvacfan_speed, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_hvacfan_targettemp, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_airconditioning, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_airrecirculation, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_heater, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_defrost, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_defrost_windshield, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_defrost_rearwindow, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_defrost_sidemirrors, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_steeringwheelheater, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_seatheater, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_seatcooler, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_window, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_window_driver, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_window_passenger, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_window_rearleft, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_window_rearright, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_sunroof, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_sunroof_openness, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_sunroof_tilt, 1, (FB::JSAPIPtr));
	FB_JSAPI_EVENT(climateenvironment_convertibleroof, 1, (FB::JSAPIPtr));

private:
	FB::BrowserHostPtr m_host;
	VehicleEventsPtr m_vehicleEvents;
	VehicleInterfaceAdapterPtr m_vehicleInterfaceAdapter;
};

#endif // H_VehicleInterface
