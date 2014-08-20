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
 * @file VehicleInterface.cpp 
 * For further information, see http://www.genivi.org/
 */

#include "JSObject.h"
#include <boost/assign.hpp>
#include "DOM/Document.h"

#include "variant_list.h"
#include "VehicleInterface.h"

VehicleInterface::VehicleInterface(const FB::BrowserHostPtr& host) : m_host(host)
{
	registerMethod("get",
				   make_method(this, 
							   &VehicleInterface::get));
	registerMethod("set",
				   make_method(this, 
							   &VehicleInterface::set));
	registerMethod("getSupportedEventTypes",
				   make_method(this,
							   &VehicleInterface::getSupportedEventTypes));
}

VehicleInterface::~VehicleInterface()
{

}

void VehicleInterface::setVehicleEventsPtr(const VehicleEventsPtr& m_vehicleEvents)
{
	this->m_vehicleEvents = m_vehicleEvents;
}

void VehicleInterface::setVehicleInterfaceAdapterPtr(const VehicleInterfaceAdapterPtr& m_vehicleInterfaceAdapter)
{
	this->m_vehicleInterfaceAdapter = m_vehicleInterfaceAdapter;
}

void VehicleInterface::get(VehicleEventType type, 
						   const FB::JSObjectPtr& handler, 
						   const FB::JSObjectPtr& errorCB)
{
	VehicleEventPtr m_vehicleEvent = m_vehicleEvents->getVehicleEvent(type);
	
	if (m_vehicleEvent == NULL)
	{
		if (errorCB)
		{
			int errorCode = VehicleError::UNKNOWN;
			errorCB->InvokeAsync("", FB::variant_list_of(errorCode));
		}
	}
	else if (!m_vehicleEvent->isSupportedEventType(type, false))
	{
		if (errorCB)
		{
			int errorCode = VehicleError::UNKNOWN;
			errorCB->InvokeAsync("", FB::variant_list_of(errorCode));
		}
	}
	else if (!m_vehicleEvent->isValid(type))
	{
		if (errorCB)
		{
			int errorCode = VehicleError::NOT_AVAILABLE;
			errorCB->InvokeAsync("", FB::variant_list_of(errorCode));
		}
	}
	else
	{
		if (type.compare(0, VehicleInfoEvent::VEHICLEINFO.length(), VehicleInfoEvent::VEHICLEINFO) == 0)
		{
			VehicleInfoEventPtr m_event = boost::make_shared<VehicleInfoEvent>(m_host);
			m_event = boost::dynamic_pointer_cast<VehicleInfoEvent>(m_vehicleEvent);
			m_event->setType(type);
			if (handler)
			{
				handler->InvokeAsync("", FB::variant_list_of(m_event));
			}
		}
		else if (type.compare(0, RunningStatusEvent::RUNNINGSTATUS.length(), RunningStatusEvent::RUNNINGSTATUS) == 0)
		{
			RunningStatusEventPtr m_event = boost::make_shared<RunningStatusEvent>(m_host);
			m_event = boost::dynamic_pointer_cast<RunningStatusEvent>(m_vehicleEvent);
			m_event->setType(type);
			if (handler)
			{
				handler->InvokeAsync("", FB::variant_list_of(m_event));
			}
		}
		else if (type.compare(0, MaintenanceEvent::MAINTENANCE.length(), MaintenanceEvent::MAINTENANCE) == 0)
		{
			MaintenanceEventPtr m_event = boost::make_shared<MaintenanceEvent>(m_host);
			m_event = boost::dynamic_pointer_cast<MaintenanceEvent>(m_vehicleEvent);
			m_event->setType(type);
			if (handler)
			{
				handler->InvokeAsync("", FB::variant_list_of(m_event));
			}
		}
		else if (type.compare(0, PersonalizationEvent::PERSONALIZATION.length(), PersonalizationEvent::PERSONALIZATION) == 0)
		{
			PersonalizationEventPtr m_event = boost::make_shared<PersonalizationEvent>(m_host);
			m_event = boost::dynamic_pointer_cast<PersonalizationEvent>(m_vehicleEvent);
			m_event->setType(type);
			if (handler)
			{
				handler->InvokeAsync("", FB::variant_list_of(m_event));
			}
		}
		else if (type.compare(0, DrivingSafetyEvent::DRIVINGSAFETY.length(), DrivingSafetyEvent::DRIVINGSAFETY) == 0)
		{
			DrivingSafetyEventPtr m_event = boost::make_shared<DrivingSafetyEvent>(m_host);
			m_event = boost::dynamic_pointer_cast<DrivingSafetyEvent>(m_vehicleEvent);
			m_event->setType(type);
			if (handler)
			{
				handler->InvokeAsync("", FB::variant_list_of(m_event));
			}
		}
		else if (type.compare(0, VisionSystemEvent::VISIONSYSTEM.length(), VisionSystemEvent::VISIONSYSTEM) == 0)
		{
			VisionSystemEventPtr m_event = boost::make_shared<VisionSystemEvent>(m_host);
			m_event = boost::dynamic_pointer_cast<VisionSystemEvent>(m_vehicleEvent);
			m_event->setType(type);
			if (handler)
			{
				handler->InvokeAsync("", FB::variant_list_of(m_event));
			}
		}
		else if (type.compare(0, ParkingEvent::PARKING.length(), ParkingEvent::PARKING) == 0)
		{
			ParkingEventPtr m_event = boost::make_shared<ParkingEvent>(m_host);
			m_event = boost::dynamic_pointer_cast<ParkingEvent>(m_vehicleEvent);
			m_event->setType(type);
			if (handler)
			{
				handler->InvokeAsync("", FB::variant_list_of(m_event));
			}
		}
		else if (type.compare(0, ClimateEnvironmentEvent::CLIMATEENVIRONMENT.length(), ClimateEnvironmentEvent::CLIMATEENVIRONMENT) == 0)
		{
			ClimateEnvironmentEventPtr m_event = boost::make_shared<ClimateEnvironmentEvent>(m_host);
			m_event = boost::dynamic_pointer_cast<ClimateEnvironmentEvent>(m_vehicleEvent);
			m_event->setType(type);
			if (handler)
			{
				handler->InvokeAsync("", FB::variant_list_of(m_event));
			}
		}
	}
}

void VehicleInterface::set(VehicleEventType type,
						   const FB::JSObjectPtr& data,
						   const FB::JSObjectPtr& successCB,
						   const FB::JSObjectPtr& errorCB)
{
	std::map<DOMString, FB::variant> dataMap;
	data->GetObjectValues(data, dataMap);
	bool result = true; 

	if (type.compare(PersonalizationEvent::PERSONALIZATION_LANGUAGE) == 0)
	{
		if (dataMap.count("language") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["language"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(PersonalizationEvent::PERSONALIZATION_MEASUREMENTSYSTEM) == 0)
	{
		if (dataMap.count("measurementSystem") == 0)
		{
			result = false;
		}
		else
		{
			bool value = dataMap["measurementSystem"].convert_cast<bool>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(PersonalizationEvent::PERSONALIZATION_MIRROR) == 0)
	{
		if ((dataMap.count("mirrorDriver") == 0) | (dataMap.count("mirrorPassenger") == 0) | (dataMap.count("mirrorInside") == 0))
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["mirrorDriver"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(PersonalizationEvent::PERSONALIZATION_MIRROR_DRIVER, value);

			value = dataMap["mirrorPassenger"].convert_cast<unsigned short>();
			result &= m_vehicleInterfaceAdapter->setVehicleData(PersonalizationEvent::PERSONALIZATION_MIRROR_PASSENGER, value);

			value = dataMap["mirrorInside"].convert_cast<unsigned short>();
			result &= m_vehicleInterfaceAdapter->setVehicleData(PersonalizationEvent::PERSONALIZATION_MIRROR_INSIDE, value);
		}
	}
	else if (type.compare(PersonalizationEvent::PERSONALIZATION_MIRROR_DRIVER) == 0)
	{
		if (dataMap.count("mirrorDriver") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["mirrorDriver"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(PersonalizationEvent::PERSONALIZATION_MIRROR_PASSENGER) == 0)
	{
		if (dataMap.count("mirrorPassenger") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["mirrorPassenger"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(PersonalizationEvent::PERSONALIZATION_MIRROR_INSIDE) == 0)
	{
		if (dataMap.count("mirrorInside") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["mirrorInside"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(PersonalizationEvent::PERSONALIZATION_STEERINGWHEELPOSITION) == 0)
	{
		if ((dataMap.count("steeringWheelPositionSlide") == 0) | (dataMap.count("steeringWheelPositionTilt") == 0))
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["steeringWheelPositionSlide"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(PersonalizationEvent::PERSONALIZATION_STEERINGWHEELPOSITION_SLIDE, value);

			value = dataMap["steeringWheelPositionTilt"].convert_cast<unsigned short>();
			result &= m_vehicleInterfaceAdapter->setVehicleData(PersonalizationEvent::PERSONALIZATION_STEERINGWHEELPOSITION_TILT, value);
		}
	}
	else if (type.compare(PersonalizationEvent::PERSONALIZATION_STEERINGWHEELPOSITION_SLIDE) == 0)
	{
		if (dataMap.count("steeringWheelPositionSlide") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["steeringWheelPositionSlide"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(PersonalizationEvent::PERSONALIZATION_STEERINGWHEELPOSITION_TILT) == 0)
	{
		if (dataMap.count("steeringWheelPositionTilt") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["steeringWheelPositionTilt"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(PersonalizationEvent::PERSONALIZATION_DRIVINGMODE) == 0)
	{
		if (dataMap.count("drivingMode") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["drivingMode"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION) == 0)
	{
		if ((dataMap.count("driverSeatPositionReclineSeatback") == 0) | (dataMap.count("driverSeatPositionSlide") == 0)
			| (dataMap.count("driverSeatPositionCushionHeight") == 0) | (dataMap.count("driverSeatPositionHeadrest") == 0)
			| (dataMap.count("driverSeatPositionBackCushion") == 0) | (dataMap.count("driverSeatPositionSideCushion") == 0))
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["driverSeatPositionReclineSeatback"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION_RECLINESEATBACK, value);

			value = dataMap["driverSeatPositionSlide"].convert_cast<unsigned short>();
			result &= m_vehicleInterfaceAdapter->setVehicleData(PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION_SLIDE, value);

			value = dataMap["driverSeatPositionCushionHeight"].convert_cast<unsigned short>();
			result &= m_vehicleInterfaceAdapter->setVehicleData(PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION_CUSHIONHEIGHT, value);

			value = dataMap["driverSeatPositionHeadrest"].convert_cast<unsigned short>();
			result &= m_vehicleInterfaceAdapter->setVehicleData(PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION_HEADREST, value);

			value = dataMap["driverSeatPositionBackCushion"].convert_cast<unsigned short>();
			result &= m_vehicleInterfaceAdapter->setVehicleData(PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION_BACKCUSHION, value);

			value = dataMap["driverSeatPositionSideCushion"].convert_cast<unsigned short>();
			result &= m_vehicleInterfaceAdapter->setVehicleData(PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION_SIDECUSHION, value);
		}
	}
	else if (type.compare(PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION_RECLINESEATBACK) == 0)
	{
		if (dataMap.count("driverSeatPositionReclineSeatback") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["driverSeatPositionReclineSeatback"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION_SLIDE) == 0)
	{
		if (dataMap.count("driverSeatPositionSlide") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["driverSeatPositionSlide"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION_CUSHIONHEIGHT) == 0)
	{
		if (dataMap.count("driverSeatPositionCushionHeight") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["driverSeatPositionCushionHeight"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION_HEADREST) == 0)
	{
		if (dataMap.count("driverSeatPositionHeadrest") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["driverSeatPositionHeadrest"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION_BACKCUSHION) == 0)
	{
		if (dataMap.count("driverSeatPositionBackCushion") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["driverSeatPositionBackCushion"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION_SIDECUSHION) == 0)
	{
		if (dataMap.count("driverSeatPositionSideCushion") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["driverSeatPositionSideCushion"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION) == 0)
	{
		if ((dataMap.count("passengerSeatPositionReclineSeatback") == 0) | (dataMap.count("passengerSeatPositionSlide") == 0)
			| (dataMap.count("passengerSeatPositionCushionHeight") == 0) | (dataMap.count("passengerSeatPositionHeadrest") == 0)
			| (dataMap.count("passengerSeatPositionBackCushion") == 0) | (dataMap.count("passengerSeatPositionSideCushion") == 0))
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["passengerSeatPositionReclineSeatback"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION_RECLINESEATBACK, value);

			value = dataMap["passengerSeatPositionSlide"].convert_cast<unsigned short>();
			result &= m_vehicleInterfaceAdapter->setVehicleData(PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION_SLIDE, value);

			value = dataMap["passengerSeatPositionCushionHeight"].convert_cast<unsigned short>();
			result &= m_vehicleInterfaceAdapter->setVehicleData(PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION_CUSHIONHEIGHT, value);

			value = dataMap["passengerSeatPositionHeadrest"].convert_cast<unsigned short>();
			result &= m_vehicleInterfaceAdapter->setVehicleData(PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION_HEADREST, value);

			value = dataMap["passengerSeatPositionBackCushion"].convert_cast<unsigned short>();
			result &= m_vehicleInterfaceAdapter->setVehicleData(PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION_BACKCUSHION, value);

			value = dataMap["passengerSeatPositionSideCushion"].convert_cast<unsigned short>();
			result &= m_vehicleInterfaceAdapter->setVehicleData(PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION_SIDECUSHION, value);
		}
	}
	else if (type.compare(PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION_RECLINESEATBACK) == 0)
	{
		if (dataMap.count("passengerSeatPositionReclineSeatback") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["passengerSeatPositionReclineSeatback"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION_SLIDE) == 0)
	{
		if (dataMap.count("passengerSeatPositionSlide") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["passengerSeatPositionSlide"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION_CUSHIONHEIGHT) == 0)
	{
		if (dataMap.count("passengerSeatPositionCushionHeight") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["passengerSeatPositionCushionHeight"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION_HEADREST) == 0)
	{
		if (dataMap.count("passengerSeatPositionHeadrest") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["passengerSeatPositionHeadrest"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION_BACKCUSHION) == 0)
	{
		if (dataMap.count("passengerSeatPositionBackCushion") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["passengerSeatPositionBackCushion"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION_SIDECUSHION) == 0)
	{
		if (dataMap.count("passengerSeatPositionSideCushion") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["passengerSeatPositionSideCushion"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(PersonalizationEvent::PERSONALIZATION_DASHBOARDILLUMINATION) == 0)
	{
		if (dataMap.count("dashboardIllumination") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["dashboardIllumination"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(PersonalizationEvent::PERSONALIZATION_GENERATEDVEHICLESOUNDMODE) == 0)
	{
		if (dataMap.count("generatedVehicleSoundMode") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["generatedVehicleSoundMode"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(DrivingSafetyEvent::DRIVINGSAFETY_VEHICLETOPSPEEDLIMIT) == 0)
	{
		if (dataMap.count("vehicleTopSpeedLimit") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["vehicleTopSpeedLimit"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(DrivingSafetyEvent::DRIVINGSAFETY_DOORLOCKSTATUS) == 0) 
	{
		if ((dataMap.count("doorLockStatusDriver") == 0) | (dataMap.count("doorLockStatusPassenger") == 0)
			| (dataMap.count("doorLockStatusRearLeft") == 0) | (dataMap.count("doorLockStatusRearRight") == 0))
		{
			result = false;
		}
		else
		{
			bool value = dataMap["doorLockStatusDriver"].convert_cast<bool>();
			result = m_vehicleInterfaceAdapter->setVehicleData(DrivingSafetyEvent::DRIVINGSAFETY_DOORLOCKSTATUS_DRIVER, value);

			value = dataMap["doorLockStatusPassenger"].convert_cast<bool>();
			result &= m_vehicleInterfaceAdapter->setVehicleData(DrivingSafetyEvent::DRIVINGSAFETY_DOORLOCKSTATUS_PASSENGER, value);

			value = dataMap["doorLockStatusRearLeft"].convert_cast<bool>();
			result &= m_vehicleInterfaceAdapter->setVehicleData(DrivingSafetyEvent::DRIVINGSAFETY_DOORLOCKSTATUS_REARLEFT, value);

			value = dataMap["doorLockStatusRearRight"].convert_cast<bool>();
			result &= m_vehicleInterfaceAdapter->setVehicleData(DrivingSafetyEvent::DRIVINGSAFETY_DOORLOCKSTATUS_REARRIGHT, value);
		}
	}
	else if (type.compare(DrivingSafetyEvent::DRIVINGSAFETY_DOORLOCKSTATUS_DRIVER) == 0)
	{
		if (dataMap.count("doorLockStatusDriver") == 0)
		{
			result = false;
		}
		else
		{
			bool value = dataMap["doorLockStatusDriver"].convert_cast<bool>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(DrivingSafetyEvent::DRIVINGSAFETY_DOORLOCKSTATUS_PASSENGER) == 0)
	{
		if (dataMap.count("doorLockStatusPassenger") == 0)
		{
			result = false;
		}
		else
		{
			bool value = dataMap["doorLockStatusPassenger"].convert_cast<bool>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(DrivingSafetyEvent::DRIVINGSAFETY_DOORLOCKSTATUS_REARLEFT) == 0)
	{
		if (dataMap.count("doorLockStatusRearLeft") == 0)
		{
			result = false;
		}
		else
		{
			bool value = dataMap["doorLockStatusRearLeft"].convert_cast<bool>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(DrivingSafetyEvent::DRIVINGSAFETY_DOORLOCKSTATUS_REARRIGHT) == 0)
	{
		if (dataMap.count("doorLockStatusRearRight") == 0)
		{
			result = false;
		}
		else
		{
			bool value = dataMap["doorLockStatusRearRight"].convert_cast<bool>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(DrivingSafetyEvent::DRIVINGSAFETY_WINDOWLOCK) == 0) 
	{
		if ((dataMap.count("windowLockDriver") == 0) | (dataMap.count("windowLockPassenger") == 0)
			| (dataMap.count("windowLockRearLeft") == 0) | (dataMap.count("windowLockRearRight") == 0))
		{
			result = false;
		}
		else
		{
			bool value = dataMap["windowLockDriver"].convert_cast<bool>();
			result = m_vehicleInterfaceAdapter->setVehicleData(DrivingSafetyEvent::DRIVINGSAFETY_WINDOWLOCK_DRIVER, value);

			value = dataMap["windowLockPassenger"].convert_cast<bool>();
			result &= m_vehicleInterfaceAdapter->setVehicleData(DrivingSafetyEvent::DRIVINGSAFETY_WINDOWLOCK_PASSENGER, value);

			value = dataMap["windowLockRearLeft"].convert_cast<bool>();
			result &= m_vehicleInterfaceAdapter->setVehicleData(DrivingSafetyEvent::DRIVINGSAFETY_WINDOWLOCK_REARLEFT, value);

			value = dataMap["windowLockRearRight"].convert_cast<bool>();
			result &= m_vehicleInterfaceAdapter->setVehicleData(DrivingSafetyEvent::DRIVINGSAFETY_WINDOWLOCK_REARRIGHT, value);
		}
	}
	else if (type.compare(DrivingSafetyEvent::DRIVINGSAFETY_WINDOWLOCK_DRIVER) == 0)
	{
		if (dataMap.count("windowLockDriver") == 0)
		{
			result = false;
		}
		else
		{
			bool value = dataMap["windowLockDriver"].convert_cast<bool>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(DrivingSafetyEvent::DRIVINGSAFETY_WINDOWLOCK_PASSENGER) == 0)
	{
		if (dataMap.count("windowLockPassenger") == 0)
		{
			result = false;
		}
		else
		{
			bool value = dataMap["windowLockPassenger"].convert_cast<bool>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(DrivingSafetyEvent::DRIVINGSAFETY_WINDOWLOCK_REARLEFT) == 0)
	{
		if (dataMap.count("windowLockRearLeft") == 0)
		{
			result = false;
		}
		else
		{
			bool value = dataMap["windowLockRearLeft"].convert_cast<bool>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(DrivingSafetyEvent::DRIVINGSAFETY_WINDOWLOCK_REARRIGHT) == 0)
	{
		if (dataMap.count("windowLockRearRight") == 0)
		{
			result = false;
		}
		else
		{
			bool value = dataMap["windowLockRearRight"].convert_cast<bool>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(ParkingEvent::PARKING_PARKINGLIGHTS) == 0)
	{
		if (dataMap.count("parkingLights") == 0)
		{
			result = false;
		}
		else
		{
			bool value = dataMap["parkingLights"].convert_cast<bool>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_WINDSHIELDWIPER) == 0)
	{
		if (dataMap.count("windshieldWiper") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["windshieldWiper"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_REARWIPER) == 0)
	{
		if (dataMap.count("rearWiper") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["rearWiper"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_HVACFAN) == 0) 
	{
		if ((dataMap.count("hvacFanDirection") == 0) | (dataMap.count("hvacFanSpeed") == 0) | (dataMap.count("hvacFanTargetTemp") == 0))
		{
			result = false;
		}
		else
		{
			unsigned short value1 = dataMap["hvacFanDirection"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_HVACFAN_DIRECTION, value1);

			value1 = dataMap["hvacFanSpeed"].convert_cast<unsigned short>();
			result &= m_vehicleInterfaceAdapter->setVehicleData(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_HVACFAN_SPEED, value1);

			short value2 = dataMap["hvacFanTargetTemp"].convert_cast<short>();
			result &= m_vehicleInterfaceAdapter->setVehicleData(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_HVACFAN_TARGETTEMP, value2);
		}
	}
	else if (type.compare(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_HVACFAN_DIRECTION) == 0)
	{
		if (dataMap.count("hvacFanDirection") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["hvacFanDirection"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_HVACFAN_SPEED) == 0)
	{
		if (dataMap.count("hvacFanSpeed") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["hvacFanSpeed"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_HVACFAN_TARGETTEMP) == 0)
	{
		if (dataMap.count("hvacFanTargetTemp") == 0)
		{
			result = false;
		}
		else
		{
			short value = dataMap["hvacFanTargetTemp"].convert_cast<short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_AIRCONDITIONING) == 0)
	{
		if (dataMap.count("airConditioning") == 0)
		{
			result = false;
		}
		else
		{
			bool value = dataMap["airConditioning"].convert_cast<bool>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_AIRRECIRCULATION) == 0)
	{
		if (dataMap.count("airRecirculation") == 0)
		{
			result = false;
		}
		else
		{
			bool value = dataMap["airRecirculation"].convert_cast<bool>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_HEATER) == 0)
	{
		if (dataMap.count("heater") == 0)
		{
			result = false;
		}
		else
		{
			bool value = dataMap["heater"].convert_cast<bool>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_DEFROST) == 0) 
	{
		if ((dataMap.count("defrostWindshield") == 0) | (dataMap.count("defrostRearWindow") == 0) | (dataMap.count("defrostSideMirrors") == 0))
		{
			result = false;
		}
		else
		{
			bool value = dataMap["defrostWindshield"].convert_cast<bool>();
			result = m_vehicleInterfaceAdapter->setVehicleData(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_DEFROST_WINDSHIELD, value);

			value = dataMap["defrostRearWindow"].convert_cast<bool>();
			result &= m_vehicleInterfaceAdapter->setVehicleData(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_DEFROST_REARWINDOW, value);

			value = dataMap["defrostSideMirrors"].convert_cast<bool>();
			result &= m_vehicleInterfaceAdapter->setVehicleData(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_DEFROST_SIDEMIRRORS, value);
		}
	}
	else if (type.compare(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_DEFROST_WINDSHIELD) == 0)
	{
		if (dataMap.count("defrostWindshield") == 0)
		{
			result = false;
		}
		else
		{
			bool value = dataMap["defrostWindshield"].convert_cast<bool>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_DEFROST_REARWINDOW) == 0)
	{
		if (dataMap.count("defrostRearWindow") == 0)
		{
			result = false;
		}
		else
		{
			bool value = dataMap["defrostRearWindow"].convert_cast<bool>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_DEFROST_SIDEMIRRORS) == 0)
	{
		if (dataMap.count("defrostSideMirrors") == 0)
		{
			result = false;
		}
		else
		{
			bool value = dataMap["defrostSideMirrors"].convert_cast<bool>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_STEERINGWHEELHEATER) == 0)
	{
		if (dataMap.count("steeringWheelHeater") == 0)
		{
			result = false;
		}
		else
		{
			bool value = dataMap["steeringWheelHeater"].convert_cast<bool>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_SEATHEATER) == 0)
	{
		if (dataMap.count("seatHeater") == 0)
		{
			result = false;
		}
		else
		{
			bool value = dataMap["seatHeater"].convert_cast<bool>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_SEATCOOLER) == 0)
	{
		if (dataMap.count("seatCooler") == 0)
		{
			result = false;
		}
		else
		{
			bool value = dataMap["seatCooler"].convert_cast<bool>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_SUNROOF) == 0) 
	{
		if ((dataMap.count("sunroofOpenness") == 0) | (dataMap.count("sunroofTilt") == 0))
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["sunroofOpenness"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_SUNROOF_OPENNESS, value);

			value = dataMap["sunroofTilt"].convert_cast<unsigned short>();
			result &= m_vehicleInterfaceAdapter->setVehicleData(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_SUNROOF_TILT, value);
		}
	}
	else if (type.compare(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_SUNROOF_OPENNESS) == 0)
	{
		if (dataMap.count("sunroofOpenness") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["sunroofOpenness"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_SUNROOF_TILT) == 0)
	{
		if (dataMap.count("sunroofTilt") == 0)
		{
			result = false;
		}
		else
		{
			unsigned short value = dataMap["sunroofTilt"].convert_cast<unsigned short>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else if (type.compare(ClimateEnvironmentEvent::CLIMATEENVIRONMENT_CONVERTIBLEROOF) == 0)
	{
		if (dataMap.count("convertibleRoof") == 0)
		{
			result = false;
		}
		else
		{
			bool value = dataMap["convertibleRoof"].convert_cast<bool>();
			result = m_vehicleInterfaceAdapter->setVehicleData(type, value);
		}
	}
	else
	{
		if (errorCB) 
		{
			int errorCode = VehicleError::UNKNOWN;
			errorCB->InvokeAsync("", FB::variant_list_of(errorCode));
		}
	}

	if (!result)
	{
		if (errorCB) 
		{
			int errorCode = VehicleError::NOT_AVAILABLE;
			errorCB->InvokeAsync("", FB::variant_list_of(errorCode));
		}
	}
	else
	{
		if (successCB)
		{
			successCB->InvokeAsync("", FB::variant_list_of());
		}
	}
}

FB::VariantList VehicleInterface::getSupportedEventTypes(VehicleEventType type, bool writable)
{
	VehicleEventPtr m_vehicleEvent = m_vehicleEvents->getVehicleEvent(type);
	
	if (m_vehicleEvent == NULL)
	{
		FB::VariantList nullList;
		return nullList;
	}
	else if (!m_vehicleEvent->isSupportedEventType(type, false))
	{
		FB::VariantList nullList;
		return nullList;
	} 
	else
	{
		return FB::make_variant_list(m_vehicleEvent->getSupportedEventTypes(type, writable));
	}
}

void VehicleInterface::fireEvent(const std::string& eventName, FB::detail::VariantListInserter inserter)
{
	this->FireEvent("on" + eventName, inserter);
}
