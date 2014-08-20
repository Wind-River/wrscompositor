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
 * @file def-dbus.h 
 * For further information, see http://www.genivi.org/
 */

#ifndef H_DefDBus
#define H_DefDBus

namespace genivi
{
	namespace wavd
	{
		namespace dbus
		{
			namespace get
			{
				/* VehicleInfoEvent (1xx) */
				const char* VEHICLEINFO_WMI = "get_vehicleinfo_wmi";
				const char* VEHICLEINFO_VIN = "get_vehicleinfo_vin";
				const char* VEHICLEINFO_VEHICLETYPE = "get_vehicleinfo_vehicletype";
				const char* VEHICLEINFO_DOORTYPE_1STROW = "get_vehicleinfo_doortype_1strow";
				const char* VEHICLEINFO_DOORTYPE_2NDROW = "get_vehicleinfo_doortype_2ndrow";
				const char* VEHICLEINFO_DOORTYPE_3RDROW = "get_vehicleinfo_doortype_3rdrow";
				const char* VEHICLEINFO_FUELTYPE = "get_vehicleinfo_fueltype";
				const char* VEHICLEINFO_TRANSMISSIONGEARTYPE = "get_vehicleinfo_transmissiongeartype";
				const char* VEHICLEINFO_WHEELINFO_RADIUS = "get_vehicleinfo_wheelinfo_radius";
				const char* VEHICLEINFO_WHEELINFO_TRACK = "get_vehicleinfo_wheelinfo_track";

				/* RunningStatusEvent (2xx) */
				const char* RUNNINGSTATUS_VEHICLEPOWERMODE = "get_runningstatus_vehiclepowermode";
				const char* RUNNINGSTATUS_SPEEDOMETER = "get_runningstatus_speedometer";
				const char* RUNNINGSTATUS_ENGINESPEED = "get_runningstatus_enginespeed";
				const char* RUNNINGSTATUS_TRIPMETER_1_MILEAGE = "get_runningstatus_tripmeter_1_mileage";
				const char* RUNNINGSTATUS_TRIPMETER_2_MILEAGE = "get_runningstatus_tripmeter_2_mileage";
				const char* RUNNINGSTATUS_TRIPMETER_1_AVERAGESPEED = "get_runningstatus_tripmeter_1_averagespeed";
				const char* RUNNINGSTATUS_TRIPMETER_2_AVERAGESPEED = "get_runningstatus_tripmeter_2_averagespeed";
				const char* RUNNINGSTATUS_TRIPMETER_1_FUELCONSUMPTION = "get_runningstatus_tripmeter_1_fuelconsumption";
				const char* RUNNINGSTATUS_TRIPMETER_2_FUELCONSUMPTION = "get_runningstatus_tripmeter_2_fuelconsumption";
				const char* RUNNINGSTATUS_TRANSMISSIONGEARSTATUS = "get_runningstatus_transmissiongearstatus";
				const char* RUNNINGSTATUS_CRUISECONTROL_STATUS = "get_runningstatus_cruisecontrol_status";
				const char* RUNNINGSTATUS_CRUISECONTROL_SPEED = "get_runningstatus_cruisecontrol_speed";
				const char* RUNNINGSTATUS_WHEELBRAKE = "get_runningstatus_wheelbrake";
				const char* RUNNINGSTATUS_LIGHTSSTATUS_HEAD = "get_runningstatus_lightsstatus_head";
				const char* RUNNINGSTATUS_LIGHTSSTATUS_HIGHBEAM = "get_runningstatus_lightsstatus_highbeam";
				const char* RUNNINGSTATUS_LIGHTSSTATUS_TURNLEFT = "get_runningstatus_lightsstatus_turnleft";
				const char* RUNNINGSTATUS_LIGHTSSTATUS_TURNRIGHT = "get_runningstatus_lightsstatus_turnright";
				const char* RUNNINGSTATUS_LIGHTSSTATUS_BRAKE = "get_runningstatus_lightsstatus_brake";
				const char* RUNNINGSTATUS_LIGHTSSTATUS_FOGFRONT = "get_runningstatus_lightsstatus_fogfront";
				const char* RUNNINGSTATUS_LIGHTSSTATUS_FOGREAR = "get_runningstatus_lightsstatus_fogrear";
				const char* RUNNINGSTATUS_LIGHTSSTATUS_HAZARD = "get_runningstatus_lightsstatus_hazard";
				const char* RUNNINGSTATUS_LIGHTSSTATUS_PARKING = "get_runningstatus_lightsstatus_parking";
				const char* RUNNINGSTATUS_INTERIORLIGHTSSTATUS_DRIVER = "get_runningstatus_interiorlightsstatus_driver";
				const char* RUNNINGSTATUS_INTERIORLIGHTSSTATUS_PASSENGER = "get_runningstatus_interiorlightsstatus_passenger";
				const char* RUNNINGSTATUS_INTERIORLIGHTSSTATUS_CENTER = "get_runningstatus_interiorlightsstatus_center";
				const char* RUNNINGSTATUS_AUTOMATICHEADLIGHTS = "get_runningstatus_automaticheadlights";
				const char* RUNNINGSTATUS_DYNAMICHIGHBEAM = "get_runningstatus_dynamichighbeam";
				const char* RUNNINGSTATUS_HORN = "get_runningstatus_horn";
				const char* RUNNINGSTATUS_CHIME = "get_runningstatus_chime";
				const char* RUNNINGSTATUS_FUEL = "get_runningstatus_fuel";
				const char* RUNNINGSTATUS_ESTIMATEDRANGE = "get_runningstatus_estimatedrange";
				const char* RUNNINGSTATUS_ENGINEOIL_REMAINING = "get_runningstatus_engineoil_remaining";
				const char* RUNNINGSTATUS_ENGINEOIL_CHANGE = "get_runningstatus_engineoil_change";
				const char* RUNNINGSTATUS_ENGINEOIL_TEMP = "get_runningstatus_engineoil_temp";
				const char* RUNNINGSTATUS_ENGINECOOLANT_LEVEL = "get_runningstatus_enginecoolant_level";
				const char* RUNNINGSTATUS_ENGINECOOLANT_TEMP = "get_runningstatus_enginecoolant_temp";
				const char* RUNNINGSTATUS_STEERINGWHEELANGLE = "get_runningstatus_steeringwheelangle";

				/* MaintenanceEvent (3xx) */
				const char* MAINTENANCE_ODOMETER = "get_maintenance_odometer";
				const char* MAINTENANCE_TRANSMISSIONOIL_LIFELEVEL = "get_maintenance_transmissionoil_lifelevel";
				const char* MAINTENANCE_TRANSMISSIONOIL_TEMP = "get_maintenance_transmissionoil_temp";
				const char* MAINTENANCE_BRAKEFLUIDLEVEL = "get_maintenance_brakefluid_level";
				const char* MAINTENANCE_WASHERFLUIDLEVEL = "get_maintenance_washerfluid_level";
				const char* MAINTENANCE_MALFUNCTIONINDICATORLAMP = "get_maintenance_malfunctionindicatorlamp";
				const char* MAINTENANCE_BATTERY_VOLTAGE = "get_maintenance_battery_voltage";
				const char* MAINTENANCE_BATTERY_CURRENT = "get_maintenance_battery_current";
				const char* MAINTENANCE_TIREPRESSURE_FRONTLEFT = "get_maintenance_tirepressure_frontleft";
				const char* MAINTENANCE_TIREPRESSURE_FRONTRIGHT = "get_maintenance_tirepressure_frontright";
				const char* MAINTENANCE_TIREPRESSURE_REARLEFT = "get_maintenance_tirepressure_rearleft";
				const char* MAINTENANCE_TIREPRESSURE_REARRIGHT = "get_maintenance_tirepressure_rearright";
				const char* MAINTENANCE_TIREPRESSURESTATUS_FRONTLEFT = "get_maintenance_tirepressurestatus_frontleft";
				const char* MAINTENANCE_TIREPRESSURESTATUS_FRONTRIGHT = "get_maintenance_tirepressurestatus_frontright";
				const char* MAINTENANCE_TIREPRESSURESTATUS_REARLEFT = "get_maintenance_tirepressurestatus_rearleft";
				const char* MAINTENANCE_TIREPRESSURESTATUS_REARRIGHT = "get_maintenance_tirepressurestatus_rearright";

				/* PersonalizationEvent (4xx) */
				const char* PERSONALIZATION_KEYID = "get_personalization_keyid";
				const char* PERSONALIZATION_LANGUAGE = "get_personalization_language";
				const char* PERSONALIZATION_MEASUREMENTSYSTEM = "get_personalization_measurementsystem";
				const char* PERSONALIZATION_MEASUREMENTSYSTEMSTRING_FUEL = "get_personalization_measurementsystemstring_fuel";
				const char* PERSONALIZATION_MEASUREMENTSYSTEMSTRING_DISTANCE = "get_personalization_measurementsystemstring_distance";
				const char* PERSONALIZATION_MEASUREMENTSYSTEMSTRING_SPEED = "get_personalization_measurementsystemstring_speed";
				const char* PERSONALIZATION_MEASUREMENTSYSTEMSTRING_CONSUMPTION = "get_personalization_measurementsystemstring_consumption";
				const char* PERSONALIZATION_MIRROR_DRIVER = "get_personalization_mirror_driver";
				const char* PERSONALIZATION_MIRROR_PASSENGER = "get_personalization_mirror_passenger";
				const char* PERSONALIZATION_MIRROR_INSIDE = "get_personalization_mirror_inside";
				const char* PERSONALIZATION_STEERINGWHEELPOSITION_SLIDE = "get_personalization_steeringwheelposition_slide";
				const char* PERSONALIZATION_STEERINGWHEELPOSITION_TILT = "get_personalization_steeringwheelposition_tilt";
				const char* PERSONALIZATION_DRIVINGMODE = "get_personalization_drivingmode";
				const char* PERSONALIZATION_DRIVERSEATPOSITION_RECLINESEATBACK = "get_personalization_driverseatposition_reclineseatback";
				const char* PERSONALIZATION_DRIVERSEATPOSITION_SLIDE = "get_personalization_driverseatposition_slide";
				const char* PERSONALIZATION_DRIVERSEATPOSITION_CUSHIONHEIGHT = "get_personalization_driverseatposition_cushionheight";
				const char* PERSONALIZATION_DRIVERSEATPOSITION_HEADREST = "get_personalization_driverseatposition_headrest";
				const char* PERSONALIZATION_DRIVERSEATPOSITION_BACKCUSHION = "get_personalization_driverseatposition_backcushion";
				const char* PERSONALIZATION_DRIVERSEATPOSITION_SIDECUSHION = "get_personalization_driverseatposition_sidecushion";
				const char* PERSONALIZATION_PASSENGERSEATPOSITION_RECLINESEATBACK = "get_personalization_passengerseatposition_reclineseatback";
				const char* PERSONALIZATION_PASSENGERSEATPOSITION_SLIDE = "get_personalization_passengerseatposition_slide";
				const char* PERSONALIZATION_PASSENGERSEATPOSITION_CUSHIONHEIGHT = "get_personalization_passengerseatposition_cushionheight";
				const char* PERSONALIZATION_PASSENGERSEATPOSITION_HEADREST = "get_personalization_passengerseatposition_headrest";
				const char* PERSONALIZATION_PASSENGERSEATPOSITION_BACKCUSHION = "get_personalization_passengerseatposition_backcushion";
				const char* PERSONALIZATION_PASSENGERSEATPOSITION_SIDECUSHION = "get_personalization_passengerseatposition_sidecushion";
				const char* PERSONALIZATION_DASHBOARDILLUMINATION = "get_personalization_dashboardillumination";
				const char* PERSONALIZATION_GENERATEDVEHICLESOUNDMODE = "get_personalization_generatedvehiclesoundmode";

				/* DrivingSafetyEvent (5xx) */
				const char* DRIVINGSAFETY_ANTILOCKBRAKINGSYSTEM = "get_drivingsafety_antilockbrakingsystem";
				const char* DRIVINGSAFETY_TRACTIONCONTROLSYSTEM = "get_drivingsafety_tractioncontrolsystem";
				const char* DRIVINGSAFETY_ELECTRONICSTABILITYCONTROL = "get_drivingsafety_electronicstabilitycontrol";
				const char* DRIVINGSAFETY_VEHICLETOPSPEEDLIMIT = "get_drivingsafety_vehicletopspeedlimit";
				const char* DRIVINGSAFETY_AIRBAGSTATUS_DRIVER = "get_drivingsafety_airbagstatus_driver";
				const char* DRIVINGSAFETY_AIRBAGSTATUS_PASSENGER = "get_drivingsafety_airbagstatus_passenger";
				const char* DRIVINGSAFETY_AIRBAGSTATUS_SIDE = "get_drivingsafety_airbagstatus_side";
				const char* DRIVINGSAFETY_DOOROPENSTATUS_DRIVER = "get_drivingsafety_dooropenstatus_driver";
				const char* DRIVINGSAFETY_DOOROPENSTATUS_PASSENGER = "get_drivingsafety_dooropenstatus_passenger";
				const char* DRIVINGSAFETY_DOOROPENSTATUS_REARLEFT = "get_drivingsafety_dooropenstatus_rearleft";
				const char* DRIVINGSAFETY_DOOROPENSTATUS_REARRIGHT = "get_drivingsafety_dooropenstatus_rearright";
				const char* DRIVINGSAFETY_DOOROPENSTATUS_TRUNK = "get_drivingsafety_dooropenstatus_trunk";
				const char* DRIVINGSAFETY_DOOROPENSTATUS_FUELFILTERCAP = "get_drivingsafety_dooropenstatus_fuel_filter_cap";
				const char* DRIVINGSAFETY_DOOROPENSTATUS_HOOD = "get_drivingsafety_dooropenstatus_hood";
				const char* DRIVINGSAFETY_DOORLOCKSTATUS_DRIVER = "get_drivingsafety_doorlockstatus_driver";
				const char* DRIVINGSAFETY_DOORLOCKSTATUS_PASSENGER = "get_drivingsafety_doorlockstatus_passenger";
				const char* DRIVINGSAFETY_DOORLOCKSTATUS_REARLEFT = "get_drivingsafety_doorlockstatus_rearleft";
				const char* DRIVINGSAFETY_DOORLOCKSTATUS_REARRIGHT = "get_drivingsafety_doorlockstatus_rearright";
				const char* DRIVINGSAFETY_CHILDSAFETYLOCK = "get_drivingsafety_childsafetylock";
				const char* DRIVINGSAFETY_OCCUPANTSSTATUS_DRIVER = "get_drivingsafety_occupantsstatus_driver";
				const char* DRIVINGSAFETY_OCCUPANTSSTATUS_PASSENGER = "get_drivingsafety_occupantsstatus_passenger";
				const char* DRIVINGSAFETY_OCCUPANTSSTATUS_REARLEFT = "get_drivingsafety_occupantsstatus_rearleft";
				const char* DRIVINGSAFETY_OCCUPANTSSTATUS_REARRIGHT = "get_drivingsafety_occupantsstatus_rearright";
				const char* DRIVINGSAFETY_SEATBELT_DRIVER = "get_drivingsafety_seatbelt_driver";
				const char* DRIVINGSAFETY_SEATBELT_PASSENGER = "get_drivingsafety_seatbelt_passenger";
				const char* DRIVINGSAFETY_SEATBELT_REARLEFT = "get_drivingsafety_seatbelt_rearleft";
				const char* DRIVINGSAFETY_SEATBELT_REARRIGHT = "get_drivingsafety_seatbelt_rearright";
				const char* DRIVINGSAFETY_WINDOWLOCK_DRIVER = "get_drivingsafety_windowlock_driver";
				const char* DRIVINGSAFETY_WINDOWLOCK_PASSENGER = "get_drivingsafety_windowlock_passenger";
				const char* DRIVINGSAFETY_WINDOWLOCK_REARLEFT = "get_drivingsafety_windowlock_rearleft";
				const char* DRIVINGSAFETY_WINDOWLOCK_REARRIGHT = "get_drivingsafety_windowlock_rearright";
				const char* DRIVINGSAFETY_OBSTACLEDISTANCE_SENSORSTATUS = "get_drivingsafety_obstacledistance_sensorstatus";
				const char* DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTCENTER = "get_drivingsafety_obstacledistance_frontcenter";
				const char* DRIVINGSAFETY_OBSTACLEDISTANCE_REARCENTER = "get_drivingsafety_obstacledistance_rearcenter";
				const char* DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTLEFT = "get_drivingsafety_obstacledistance_frontleft";
				const char* DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTRIGHT = "get_drivingsafety_obstacledistance_frontright";
				const char* DRIVINGSAFETY_OBSTACLEDISTANCE_MIDDLELEFT = "get_drivingsafety_obstacledistance_middleleft";
				const char* DRIVINGSAFETY_OBSTACLEDISTANCE_MIDDLERIGHT = "get_drivingsafety_obstacledistance_middleright";
				const char* DRIVINGSAFETY_OBSTACLEDISTANCE_REARLEFT = "get_drivingsafety_obstacledistance_rearleft";
				const char* DRIVINGSAFETY_OBSTACLEDISTANCE_REARRIGHT = "get_drivingsafety_obstacledistance_rearright";
				const char* DRIVINGSAFETY_FRONTCOLLISIONDETECTION_STATUS = "get_drivingsafety_frontcollisiondetection_status";
				const char* DRIVINGSAFETY_FRONTCOLLISIONDETECTION_DISTANCE = "get_drivingsafety_frontcollisiondetection_distance";
				const char* DRIVINGSAFETY_FRONTCOLLISIONDETECTION_TIME = "get_drivingsafety_frontcollisiondetection_time";

				/* VisionSystemEvent (6xx) */
				const char* VISIONSYSTEM_LANEDEPARTUREDETECTIONSTATUS = "get_visionsystem_lanedeparturedetectionstatus";
				const char* VISIONSYSTEM_LANEDEPARTED = "get_visionsystem_lanedeparted";

				/* ParkingEvent (7xx) */
				const char* PARKING_SECURITYALERT = "get_parking_securityalert";
				const char* PARKING_PARKINGBRAKE = "get_parking_parkingbrake";
				const char* PARKING_PARKINGLIGHTS = "get_parking_parkinglights";

				/* ClimateEnvironmentEvent (8xx) */
				const char* CLIMATEENVIRONMENT_INTERIORTEMP = "get_climateenvironment_interiortemp";
				const char* CLIMATEENVIRONMENT_EXTERIORTEMP = "get_climateenvironment_exteriortemp";
				const char* CLIMATEENVIRONMENT_EXTERIORBRIGHTNESS = "get_climateenvironment_exteriorbrightness";
				const char* CLIMATEENVIRONMENT_RAINSENSOR = "get_climateenvironment_rainsensor";
				const char* CLIMATEENVIRONMENT_WINDSHIELDWIPER = "get_climateenvironment_windshieldwiper";
				const char* CLIMATEENVIRONMENT_REARWIPER = "get_climateenvironment_rearwiper";
				const char* CLIMATEENVIRONMENT_HVACFAN_DIRECTION = "get_climateenvironment_hvacfan_direction";
				const char* CLIMATEENVIRONMENT_HVACFAN_SPEED = "get_climateenvironment_hvacfan_speed";
				const char* CLIMATEENVIRONMENT_HVACFAN_TARGETTEMP = "get_climateenvironment_hvacfan_targettemp";
				const char* CLIMATEENVIRONMENT_AIRCONDITIONING = "get_climateenvironment_airconditioning";
				const char* CLIMATEENVIRONMENT_AIRRECIRCULATION = "get_climateenvironment_airrecirculation";
				const char* CLIMATEENVIRONMENT_HEATER = "get_climateenvironment_heater";
				const char* CLIMATEENVIRONMENT_DEFROST_WINDSHIELD = "get_climateenvironment_defrost_windshield";
				const char* CLIMATEENVIRONMENT_DEFROST_REARWINDOW = "get_climateenvironment_defrost_rearwindow";
				const char* CLIMATEENVIRONMENT_DEFROST_SIDEMIRRORS = "get_climateenvironment_defrost_sidemirrors";
				const char* CLIMATEENVIRONMENT_STEERINGWHEELHEATER = "get_climateenvironment_steeringwheelheater";
				const char* CLIMATEENVIRONMENT_SEATHEATER = "get_climateenvironment_seatheater";
				const char* CLIMATEENVIRONMENT_SEATCOOLER = "get_climateenvironment_seatcooler";
				const char* CLIMATEENVIRONMENT_WINDOW_DRIVER = "get_climateenvironment_windowdriver";
				const char* CLIMATEENVIRONMENT_WINDOW_PASSENGER = "get_climateenvironment_windowpassenger";
				const char* CLIMATEENVIRONMENT_WINDOW_REARLEFT = "get_climateenvironment_windowrearleft";
				const char* CLIMATEENVIRONMENT_WINDOW_REARRIGHT = "get_climateenvironment_windowrearright";
				const char* CLIMATEENVIRONMENT_SUNROOF_OPENNESS = "get_climateenvironment_sunroof_openness";
				const char* CLIMATEENVIRONMENT_SUNROOF_TILT = "get_climateenvironment_sunroof_tilt";
				const char* CLIMATEENVIRONMENT_CONVERTIBLEROOF = "get_climateenvironment_convertibleroof";
			}
			namespace set
			{
				/* PersonalizationEvent (4xx) */
				const char* PERSONALIZATION_LANGUAGE = "set_personalization_language";
				const char* PERSONALIZATION_MEASUREMENTSYSTEM = "set_personalization_measurementsystem";
				const char* PERSONALIZATION_MIRROR_DRIVER = "set_personalization_mirror_driver";
				const char* PERSONALIZATION_MIRROR_PASSENGER = "set_personalization_mirror_passenger";
				const char* PERSONALIZATION_MIRROR_INSIDE = "set_personalization_mirror_inside";
				const char* PERSONALIZATION_STEERINGWHEELPOSITION_SLIDE = "set_personalization_steeringwheelposition_slide";
				const char* PERSONALIZATION_STEERINGWHEELPOSITION_TILT = "set_personalization_steeringwheelposition_tilt";
				const char* PERSONALIZATION_DRIVINGMODE = "set_personalization_drivingmode";
				const char* PERSONALIZATION_DRIVERSEATPOSITION_RECLINESEATBACK = "set_personalization_driverseatposition_reclineseatback";
				const char* PERSONALIZATION_DRIVERSEATPOSITION_SLIDE = "set_personalization_driverseatposition_slide";
				const char* PERSONALIZATION_DRIVERSEATPOSITION_CUSHIONHEIGHT = "set_personalization_driverseatposition_cushionheight";
				const char* PERSONALIZATION_DRIVERSEATPOSITION_HEADREST = "set_personalization_driverseatposition_headrest";
				const char* PERSONALIZATION_DRIVERSEATPOSITION_BACKCUSHION = "set_personalization_driverseatposition_backcushion";
				const char* PERSONALIZATION_DRIVERSEATPOSITION_SIDECUSHION = "set_personalization_driverseatposition_sidecushion";
				const char* PERSONALIZATION_PASSENGERSEATPOSITION_RECLINESEATBACK = "set_personalization_passengerseatposition_reclineseatback";
				const char* PERSONALIZATION_PASSENGERSEATPOSITION_SLIDE = "set_personalization_passengerseatposition_slide";
				const char* PERSONALIZATION_PASSENGERSEATPOSITION_CUSHIONHEIGHT = "set_personalization_passengerseatposition_cushionheight";
				const char* PERSONALIZATION_PASSENGERSEATPOSITION_HEADREST = "set_personalization_passengerseatposition_headrest";
				const char* PERSONALIZATION_PASSENGERSEATPOSITION_BACKCUSHION = "set_personalization_passengerseatposition_backcushion";
				const char* PERSONALIZATION_PASSENGERSEATPOSITION_SIDECUSHION = "set_personalization_passengerseatposition_sidecushion";
				const char* PERSONALIZATION_DASHBOARDILLUMINATION = "set_personalization_dashboardillumination";
				const char* PERSONALIZATION_GENERATEDVEHICLESOUNDMODE = "set_personalization_generatedvehiclesoundmode";

				/* DrivingSafetyEvent (5xx) */
				const char* DRIVINGSAFETY_VEHICLETOPSPEEDLIMIT = "set_drivingsafety_vehicletopspeedlimit";
				const char* DRIVINGSAFETY_DOORLOCKSTATUS_DRIVER = "set_drivingsafety_doorlockstatus_driver";
				const char* DRIVINGSAFETY_DOORLOCKSTATUS_PASSENGER = "set_drivingsafety_doorlockstatus_passenger";
				const char* DRIVINGSAFETY_DOORLOCKSTATUS_REARLEFT = "set_drivingsafety_doorlockstatus_rearleft";
				const char* DRIVINGSAFETY_DOORLOCKSTATUS_REARRIGHT = "set_drivingsafety_doorlockstatus_rearright";
				const char* DRIVINGSAFETY_WINDOWLOCK_DRIVER = "set_drivingsafety_windowlock_driver";
				const char* DRIVINGSAFETY_WINDOWLOCK_PASSENGER = "set_drivingsafety_windowlock_passenger";
				const char* DRIVINGSAFETY_WINDOWLOCK_REARLEFT = "set_drivingsafety_windowlock_rearleft";
				const char* DRIVINGSAFETY_WINDOWLOCK_REARRIGHT = "set_drivingsafety_windowlock_rearright";

				/* ParkingEvent (7xx) */
				const char* PARKING_PARKINGLIGHTS = "set_parking_parkinglights";

				/* ClimateEnvironmentEvent (8xx) */
				const char* CLIMATEENVIRONMENT_WINDSHIELDWIPER = "set_climateenvironment_windshieldwiper";
				const char* CLIMATEENVIRONMENT_REARWIPER = "set_climateenvironment_rearwiper";
				const char* CLIMATEENVIRONMENT_HVACFAN_DIRECTION = "set_climateenvironment_hvacfan_direction";
				const char* CLIMATEENVIRONMENT_HVACFAN_SPEED = "set_climateenvironment_hvacfan_speed";
				const char* CLIMATEENVIRONMENT_HVACFAN_TARGETTEMP = "set_climateenvironment_hvacfan_targettemp";
				const char* CLIMATEENVIRONMENT_AIRCONDITIONING = "set_climateenvironment_airconditioning";
				const char* CLIMATEENVIRONMENT_AIRRECIRCULATION = "set_climateenvironment_airrecirculation";
				const char* CLIMATEENVIRONMENT_HEATER = "set_climateenvironment_heater";
				const char* CLIMATEENVIRONMENT_DEFROST_WINDSHIELD = "set_climateenvironment_defrost_windshield";
				const char* CLIMATEENVIRONMENT_DEFROST_REARWINDOW = "set_climateenvironment_defrost_rearwindow";
				const char* CLIMATEENVIRONMENT_DEFROST_SIDEMIRRORS = "set_climateenvironment_defrost_sidemirrors";
				const char* CLIMATEENVIRONMENT_STEERINGWHEELHEATER = "set_climateenvironment_steeringwheelheater";
				const char* CLIMATEENVIRONMENT_SEATHEATER = "set_climateenvironment_seatheater";
				const char* CLIMATEENVIRONMENT_SEATCOOLER = "set_climateenvironment_seatcooler";
				const char* CLIMATEENVIRONMENT_SUNROOF_OPENNESS = "set_climateenvironment_sunroof_openness";
				const char* CLIMATEENVIRONMENT_SUNROOF_TILT = "set_climateenvironment_sunroof_tilt";
				const char* CLIMATEENVIRONMENT_CONVERTIBLEROOF = "set_climateenvironment_convertibleroof";
			}
		}
	}
}

#endif // H_DefDBus
