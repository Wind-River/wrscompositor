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
 * @file PersonalizationEvent.cpp 
 * For further information, see http://www.genivi.org/
 */

#include "JSObject.h"
#include <boost/assign.hpp>
#include "DOM/Document.h"

#include "PersonalizationEvent.h"

const VehicleEventType PersonalizationEvent::PERSONALIZATION = "personalization";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_KEYID = "personalization_keyid";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_LANGUAGE = "personalization_language";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_MEASUREMENTSYSTEM = "personalization_measurementsystem";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_MEASUREMENTSYSTEMSTRING = "personalization_measurementsystemstring";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_FUEL = "personalization_measurementsystemstring_fuel";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_DISTANCE = "personalization_measurementsystemstring_distance";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_SPEED = "personalization_measurementsystemstring_speed";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_CONSUMPTION = "personalization_measurementsystemstring_consumption";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_MIRROR = "personalization_mirror";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_MIRROR_DRIVER = "personalization_mirror_driver";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_MIRROR_PASSENGER = "personalization_mirror_passenger";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_MIRROR_INSIDE = "personalization_mirror_inside";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_STEERINGWHEELPOSITION = "personalization_steeringwheelposition";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_STEERINGWHEELPOSITION_SLIDE = "personalization_steeringwheelposition_slide";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_STEERINGWHEELPOSITION_TILT = "personalization_steeringwheelposition_tilt";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_DRIVINGMODE = "personalization_drivingmode";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION = "personalization_driverseatposition";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION_RECLINESEATBACK = "personalization_driverseatposition_reclineseatback";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION_SLIDE = "personalization_driverseatposition_slide";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION_CUSHIONHEIGHT = "personalization_driverseatposition_cushionheight";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION_HEADREST = "personalization_driverseatposition_headrest";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION_BACKCUSHION = "personalization_driverseatposition_backcushion";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_DRIVERSEATPOSITION_SIDECUSHION = "personalization_driverseatposition_sidecushion";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION = "personalization_passengerseatposition";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION_RECLINESEATBACK = "personalization_passengerseatposition_reclineseatback";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION_SLIDE = "personalization_passengerseatposition_slide";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION_CUSHIONHEIGHT = "personalization_passengerseatposition_cushionheight";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION_HEADREST = "personalization_passengerseatposition_headrest";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION_BACKCUSHION = "personalization_passengerseatposition_backcushion";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_PASSENGERSEATPOSITION_SIDECUSHION = "personalization_passengerseatposition_sidecushion";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_DASHBOARDILLUMINATION = "personalization_dashboardillumination";
const VehicleEventType PersonalizationEvent::PERSONALIZATION_GENERATEDVEHICLESOUNDMODE = "personalization_generatedvehiclesoundmode";

PersonalizationEvent::PersonalizationEvent(const FB::BrowserHostPtr& host)
{
	registerProperty("type",
					 make_property(this,
								   &PersonalizationEvent::getType));
	registerProperty("keyId",
					 make_property(this,
								   &PersonalizationEvent::getKeyId));
	registerProperty("language",
					 make_property(this,
								   &PersonalizationEvent::getLanguage));
	registerProperty("measurementSystem",
					 make_property(this,
								   &PersonalizationEvent::getMeasurementSystem));
	registerProperty("measurementSystemStringFuel",
					 make_property(this,
								   &PersonalizationEvent::getMeasurementSystemStringFuel));
	registerProperty("measurementSystemStringDistance",
					 make_property(this,
								   &PersonalizationEvent::getMeasurementSystemStringDistance));
	registerProperty("measurementSystemStringSpeed",
					 make_property(this,
								   &PersonalizationEvent::getMeasurementSystemStringSpeed));
	registerProperty("measurementSystemStringConsumption",
					 make_property(this,
								   &PersonalizationEvent::getMeasurementSystemStringConsumption));
	registerProperty("mirrorDriver",
					 make_property(this,
								   &PersonalizationEvent::getMirrorDriver));
	registerProperty("mirrorPassenger",
					 make_property(this,
								   &PersonalizationEvent::getMirrorPassenger));
	registerProperty("mirrorInside",
					 make_property(this,
								   &PersonalizationEvent::getMirrorInside));
	registerProperty("steeringWheelPositionSlide",
					 make_property(this,
								   &PersonalizationEvent::getSteeringWheelPositionSlide));
	registerProperty("steeringWheelPositionTilt",
					 make_property(this,
								   &PersonalizationEvent::getSteeringWheelPositionTilt));
	registerProperty("drivingMode",
					 make_property(this,
								   &PersonalizationEvent::getDrivingMode));
	registerProperty("driverSeatPositionReclineSeatback",
					 make_property(this,
								   &PersonalizationEvent::getDriverSeatPositionReclineSeatback));
	registerProperty("driverSeatPositionSlide",
					 make_property(this,
								   &PersonalizationEvent::getDriverSeatPositionSlide));
	registerProperty("driverSeatPositionCushionHeight",
					 make_property(this,
								   &PersonalizationEvent::getDriverSeatPositionCushionHeight));
	registerProperty("driverSeatPositionHeadrest",
					 make_property(this,
								   &PersonalizationEvent::getDriverSeatPositionHeadrest));
	registerProperty("driverSeatPositionBackCushion",
					 make_property(this,
								   &PersonalizationEvent::getDriverSeatPositionBackCushion));
	registerProperty("driverSeatPositionSideCushion",
					 make_property(this,
								   &PersonalizationEvent::getDriverSeatPositionSideCushion));
	registerProperty("passengerSeatPositionReclineSeatback",
					 make_property(this,
								   &PersonalizationEvent::getPassengerSeatPositionReclineSeatback));
	registerProperty("passengerSeatPositionSlide",
					 make_property(this,
								   &PersonalizationEvent::getPassengerSeatPositionSlide));
	registerProperty("passengerSeatPositionCushionHeight",
					 make_property(this,
								   &PersonalizationEvent::getPassengerSeatPositionCushionHeight));
	registerProperty("passengerSeatPositionHeadrest",
					 make_property(this,
								   &PersonalizationEvent::getPassengerSeatPositionHeadrest));
	registerProperty("passengerSeatPositionBackCushion",
					 make_property(this,
								   &PersonalizationEvent::getPassengerSeatPositionBackCushion));
	registerProperty("passengerSeatPositionSideCushion",
					 make_property(this,
								   &PersonalizationEvent::getPassengerSeatPositionSideCushion));
	registerProperty("dashboardIllumination",
					 make_property(this,
								   &PersonalizationEvent::getDashboardIllumination));
	registerProperty("generatedVehicleSoundMode",
					 make_property(this,
								   &PersonalizationEvent::getGeneratedVehicleSoundMode));

	allSupportedEventTypesR.push_back(PERSONALIZATION);
	allSupportedEventTypesR.push_back(PERSONALIZATION_KEYID);
	allSupportedEventTypesR.push_back(PERSONALIZATION_LANGUAGE);
	allSupportedEventTypesR.push_back(PERSONALIZATION_MEASUREMENTSYSTEM);
	allSupportedEventTypesR.push_back(PERSONALIZATION_MEASUREMENTSYSTEMSTRING);
	allSupportedEventTypesR.push_back(PERSONALIZATION_MEASUREMENTSYSTEMSTRING_FUEL);
	allSupportedEventTypesR.push_back(PERSONALIZATION_MEASUREMENTSYSTEMSTRING_DISTANCE);
	allSupportedEventTypesR.push_back(PERSONALIZATION_MEASUREMENTSYSTEMSTRING_SPEED);
	allSupportedEventTypesR.push_back(PERSONALIZATION_MEASUREMENTSYSTEMSTRING_CONSUMPTION);
	allSupportedEventTypesR.push_back(PERSONALIZATION_MIRROR);
	allSupportedEventTypesR.push_back(PERSONALIZATION_MIRROR_DRIVER);
	allSupportedEventTypesR.push_back(PERSONALIZATION_MIRROR_PASSENGER);
	allSupportedEventTypesR.push_back(PERSONALIZATION_MIRROR_INSIDE);
	allSupportedEventTypesR.push_back(PERSONALIZATION_STEERINGWHEELPOSITION);
	allSupportedEventTypesR.push_back(PERSONALIZATION_STEERINGWHEELPOSITION_SLIDE);
	allSupportedEventTypesR.push_back(PERSONALIZATION_STEERINGWHEELPOSITION_TILT);
	allSupportedEventTypesR.push_back(PERSONALIZATION_DRIVINGMODE);
	allSupportedEventTypesR.push_back(PERSONALIZATION_DRIVERSEATPOSITION);
	allSupportedEventTypesR.push_back(PERSONALIZATION_DRIVERSEATPOSITION_RECLINESEATBACK);
	allSupportedEventTypesR.push_back(PERSONALIZATION_DRIVERSEATPOSITION_SLIDE);
	allSupportedEventTypesR.push_back(PERSONALIZATION_DRIVERSEATPOSITION_CUSHIONHEIGHT);
	allSupportedEventTypesR.push_back(PERSONALIZATION_DRIVERSEATPOSITION_HEADREST);
	allSupportedEventTypesR.push_back(PERSONALIZATION_DRIVERSEATPOSITION_BACKCUSHION);
	allSupportedEventTypesR.push_back(PERSONALIZATION_DRIVERSEATPOSITION_SIDECUSHION);
	allSupportedEventTypesR.push_back(PERSONALIZATION_PASSENGERSEATPOSITION);
	allSupportedEventTypesR.push_back(PERSONALIZATION_PASSENGERSEATPOSITION_RECLINESEATBACK);
	allSupportedEventTypesR.push_back(PERSONALIZATION_PASSENGERSEATPOSITION_SLIDE);
	allSupportedEventTypesR.push_back(PERSONALIZATION_PASSENGERSEATPOSITION_CUSHIONHEIGHT);
	allSupportedEventTypesR.push_back(PERSONALIZATION_PASSENGERSEATPOSITION_HEADREST);
	allSupportedEventTypesR.push_back(PERSONALIZATION_PASSENGERSEATPOSITION_BACKCUSHION);
	allSupportedEventTypesR.push_back(PERSONALIZATION_PASSENGERSEATPOSITION_SIDECUSHION);
	allSupportedEventTypesR.push_back(PERSONALIZATION_DASHBOARDILLUMINATION);
	allSupportedEventTypesR.push_back(PERSONALIZATION_GENERATEDVEHICLESOUNDMODE);

	allSupportedEventTypesW.push_back(PERSONALIZATION_LANGUAGE);
	allSupportedEventTypesW.push_back(PERSONALIZATION_MEASUREMENTSYSTEM);
	allSupportedEventTypesW.push_back(PERSONALIZATION_MIRROR);
	allSupportedEventTypesW.push_back(PERSONALIZATION_MIRROR_DRIVER);
	allSupportedEventTypesW.push_back(PERSONALIZATION_MIRROR_PASSENGER);
	allSupportedEventTypesW.push_back(PERSONALIZATION_MIRROR_INSIDE);
	allSupportedEventTypesW.push_back(PERSONALIZATION_STEERINGWHEELPOSITION);
	allSupportedEventTypesW.push_back(PERSONALIZATION_STEERINGWHEELPOSITION_SLIDE);
	allSupportedEventTypesW.push_back(PERSONALIZATION_STEERINGWHEELPOSITION_TILT);
	allSupportedEventTypesW.push_back(PERSONALIZATION_DRIVINGMODE);
	allSupportedEventTypesW.push_back(PERSONALIZATION_DRIVERSEATPOSITION);
	allSupportedEventTypesW.push_back(PERSONALIZATION_DRIVERSEATPOSITION_RECLINESEATBACK);
	allSupportedEventTypesW.push_back(PERSONALIZATION_DRIVERSEATPOSITION_SLIDE);
	allSupportedEventTypesW.push_back(PERSONALIZATION_DRIVERSEATPOSITION_CUSHIONHEIGHT);
	allSupportedEventTypesW.push_back(PERSONALIZATION_DRIVERSEATPOSITION_HEADREST);
	allSupportedEventTypesW.push_back(PERSONALIZATION_DRIVERSEATPOSITION_BACKCUSHION);
	allSupportedEventTypesW.push_back(PERSONALIZATION_DRIVERSEATPOSITION_SIDECUSHION);
	allSupportedEventTypesW.push_back(PERSONALIZATION_PASSENGERSEATPOSITION);
	allSupportedEventTypesW.push_back(PERSONALIZATION_PASSENGERSEATPOSITION_RECLINESEATBACK);
	allSupportedEventTypesW.push_back(PERSONALIZATION_PASSENGERSEATPOSITION_SLIDE);
	allSupportedEventTypesW.push_back(PERSONALIZATION_PASSENGERSEATPOSITION_CUSHIONHEIGHT);
	allSupportedEventTypesW.push_back(PERSONALIZATION_PASSENGERSEATPOSITION_HEADREST);
	allSupportedEventTypesW.push_back(PERSONALIZATION_PASSENGERSEATPOSITION_BACKCUSHION);
	allSupportedEventTypesW.push_back(PERSONALIZATION_PASSENGERSEATPOSITION_SIDECUSHION);
	allSupportedEventTypesW.push_back(PERSONALIZATION_DASHBOARDILLUMINATION);
	allSupportedEventTypesW.push_back(PERSONALIZATION_GENERATEDVEHICLESOUNDMODE);

	validity[PERSONALIZATION] = false;
	validity[PERSONALIZATION_KEYID] = false;
	validity[PERSONALIZATION_LANGUAGE] = false;
	validity[PERSONALIZATION_MEASUREMENTSYSTEM] = false;
	validity[PERSONALIZATION_MEASUREMENTSYSTEMSTRING] = false;
	validity[PERSONALIZATION_MEASUREMENTSYSTEMSTRING_FUEL] = false;
	validity[PERSONALIZATION_MEASUREMENTSYSTEMSTRING_DISTANCE] = false;
	validity[PERSONALIZATION_MEASUREMENTSYSTEMSTRING_SPEED] = false;
	validity[PERSONALIZATION_MEASUREMENTSYSTEMSTRING_CONSUMPTION] = false;
	validity[PERSONALIZATION_MIRROR] = false;
	validity[PERSONALIZATION_MIRROR_DRIVER] = false;
	validity[PERSONALIZATION_MIRROR_PASSENGER] = false;
	validity[PERSONALIZATION_MIRROR_INSIDE] = false;
	validity[PERSONALIZATION_STEERINGWHEELPOSITION] = false;
	validity[PERSONALIZATION_STEERINGWHEELPOSITION_SLIDE] = false;
	validity[PERSONALIZATION_STEERINGWHEELPOSITION_TILT] = false;
	validity[PERSONALIZATION_DRIVINGMODE] = false;
	validity[PERSONALIZATION_DRIVERSEATPOSITION] = false;
	validity[PERSONALIZATION_DRIVERSEATPOSITION_RECLINESEATBACK] = false;
	validity[PERSONALIZATION_DRIVERSEATPOSITION_SLIDE] = false;
	validity[PERSONALIZATION_DRIVERSEATPOSITION_CUSHIONHEIGHT] = false;
	validity[PERSONALIZATION_DRIVERSEATPOSITION_HEADREST] = false;
	validity[PERSONALIZATION_DRIVERSEATPOSITION_BACKCUSHION] = false;
	validity[PERSONALIZATION_DRIVERSEATPOSITION_SIDECUSHION] = false;
	validity[PERSONALIZATION_PASSENGERSEATPOSITION] = false;
	validity[PERSONALIZATION_PASSENGERSEATPOSITION_RECLINESEATBACK] = false;
	validity[PERSONALIZATION_PASSENGERSEATPOSITION_SLIDE] = false;
	validity[PERSONALIZATION_PASSENGERSEATPOSITION_CUSHIONHEIGHT] = false;
	validity[PERSONALIZATION_PASSENGERSEATPOSITION_HEADREST] = false;
	validity[PERSONALIZATION_PASSENGERSEATPOSITION_BACKCUSHION] = false;
	validity[PERSONALIZATION_PASSENGERSEATPOSITION_SIDECUSHION] = false;
	validity[PERSONALIZATION_DASHBOARDILLUMINATION] = false;
	validity[PERSONALIZATION_GENERATEDVEHICLESOUNDMODE] = false;
}

PersonalizationEvent::~PersonalizationEvent()
{

}

void PersonalizationEvent::updateValidity(VehicleEventType type)
{
	std::string str = type;
	size_t pos;

	while ((pos = str.find_last_of('_')) != std::string::npos)
	{
		str = str.substr(0, pos);

		if (str.compare(PERSONALIZATION) == 0)
		{
			setValidity(PERSONALIZATION, isValid(PERSONALIZATION_KEYID)
						& isValid(PERSONALIZATION_LANGUAGE)
						& isValid(PERSONALIZATION_MEASUREMENTSYSTEM)
						& isValid(PERSONALIZATION_MEASUREMENTSYSTEMSTRING)
						& isValid(PERSONALIZATION_MIRROR)
						& isValid(PERSONALIZATION_STEERINGWHEELPOSITION)
						& isValid(PERSONALIZATION_DRIVINGMODE)
						& isValid(PERSONALIZATION_DRIVERSEATPOSITION)
						& isValid(PERSONALIZATION_PASSENGERSEATPOSITION)
						& isValid(PERSONALIZATION_DASHBOARDILLUMINATION)
						& isValid(PERSONALIZATION_GENERATEDVEHICLESOUNDMODE));
		}
		else if (str.compare(PERSONALIZATION_MEASUREMENTSYSTEMSTRING) == 0)
		{
			setValidity(PERSONALIZATION_MEASUREMENTSYSTEMSTRING, isValid(PERSONALIZATION_MEASUREMENTSYSTEMSTRING_FUEL)
						& isValid(PERSONALIZATION_MEASUREMENTSYSTEMSTRING_DISTANCE)
						& isValid(PERSONALIZATION_MEASUREMENTSYSTEMSTRING_SPEED)
						& isValid(PERSONALIZATION_MEASUREMENTSYSTEMSTRING_CONSUMPTION));
		}
		else if (str.compare(PERSONALIZATION_MIRROR) == 0)
		{
			setValidity(PERSONALIZATION_MIRROR, isValid(PERSONALIZATION_MIRROR_DRIVER)
						& isValid(PERSONALIZATION_MIRROR_PASSENGER)
						& isValid(PERSONALIZATION_MIRROR_INSIDE));
		}
		else if (str.compare(PERSONALIZATION_STEERINGWHEELPOSITION) == 0)
		{
			setValidity(PERSONALIZATION_STEERINGWHEELPOSITION, isValid(PERSONALIZATION_STEERINGWHEELPOSITION_SLIDE)
						& isValid(PERSONALIZATION_STEERINGWHEELPOSITION_TILT));
		}
		else if (str.compare(PERSONALIZATION_DRIVERSEATPOSITION) == 0)
		{
			setValidity(PERSONALIZATION_DRIVERSEATPOSITION, isValid(PERSONALIZATION_DRIVERSEATPOSITION_RECLINESEATBACK)
						& isValid(PERSONALIZATION_DRIVERSEATPOSITION_SLIDE)
						& isValid(PERSONALIZATION_DRIVERSEATPOSITION_CUSHIONHEIGHT)
						& isValid(PERSONALIZATION_DRIVERSEATPOSITION_HEADREST)
						& isValid(PERSONALIZATION_DRIVERSEATPOSITION_BACKCUSHION)
						& isValid(PERSONALIZATION_DRIVERSEATPOSITION_SIDECUSHION));
		}
		else if (str.compare(PERSONALIZATION_PASSENGERSEATPOSITION) == 0)
		{
			setValidity(PERSONALIZATION_PASSENGERSEATPOSITION, isValid(PERSONALIZATION_PASSENGERSEATPOSITION_RECLINESEATBACK)
						& isValid(PERSONALIZATION_PASSENGERSEATPOSITION_SLIDE)
						& isValid(PERSONALIZATION_PASSENGERSEATPOSITION_CUSHIONHEIGHT)
						& isValid(PERSONALIZATION_PASSENGERSEATPOSITION_HEADREST)
						& isValid(PERSONALIZATION_PASSENGERSEATPOSITION_BACKCUSHION)
						& isValid(PERSONALIZATION_PASSENGERSEATPOSITION_SIDECUSHION));
		}
	}
}

VehicleEventType PersonalizationEvent::getType()
{
	return type;
}

DOMString PersonalizationEvent::getKeyId()
{
	return keyId;
}

unsigned short PersonalizationEvent::getLanguage()
{
	return language;
}

bool PersonalizationEvent::getMeasurementSystem()
{
	return measurementSystem;
}

DOMString PersonalizationEvent::getMeasurementSystemStringFuel()
{
	return measurementSystemStringFuel;
}

DOMString PersonalizationEvent::getMeasurementSystemStringDistance()
{
	return measurementSystemStringDistance;
}

DOMString PersonalizationEvent::getMeasurementSystemStringSpeed()
{
	return measurementSystemStringSpeed;
}

DOMString PersonalizationEvent::getMeasurementSystemStringConsumption()
{
	return measurementSystemStringConsumption;
}

unsigned short PersonalizationEvent::getMirrorDriver()
{
	return mirrorDriver;
}

unsigned short PersonalizationEvent::getMirrorPassenger()
{
	return mirrorPassenger;
}

unsigned short PersonalizationEvent::getMirrorInside()
{
	return mirrorInside;
}

unsigned short PersonalizationEvent::getSteeringWheelPositionSlide()
{
	return steeringWheelPositionSlide;
}

unsigned short PersonalizationEvent::getSteeringWheelPositionTilt()
{
	return steeringWheelPositionTilt;
}

unsigned short PersonalizationEvent::getDrivingMode()
{
	return drivingMode;
}

unsigned short PersonalizationEvent::getDriverSeatPositionReclineSeatback()
{
	return driverSeatPositionReclineSeatback;
}

unsigned short PersonalizationEvent::getDriverSeatPositionSlide()
{
	return driverSeatPositionSlide;
}

unsigned short PersonalizationEvent::getDriverSeatPositionCushionHeight()
{
	return driverSeatPositionCushionHeight;
}

unsigned short PersonalizationEvent::getDriverSeatPositionHeadrest()
{
	return driverSeatPositionHeadrest;
}

unsigned short PersonalizationEvent::getDriverSeatPositionBackCushion()
{
	return driverSeatPositionBackCushion;
}

unsigned short PersonalizationEvent::getDriverSeatPositionSideCushion()
{
	return driverSeatPositionSideCushion;
}

unsigned short PersonalizationEvent::getPassengerSeatPositionReclineSeatback()
{
	return passengerSeatPositionReclineSeatback;
}

unsigned short PersonalizationEvent::getPassengerSeatPositionSlide()
{
	return passengerSeatPositionSlide;
}

unsigned short PersonalizationEvent::getPassengerSeatPositionCushionHeight()
{
	return passengerSeatPositionCushionHeight;
}

unsigned short PersonalizationEvent::getPassengerSeatPositionHeadrest()
{
	return passengerSeatPositionHeadrest;
}

unsigned short PersonalizationEvent::getPassengerSeatPositionBackCushion()
{
	return passengerSeatPositionBackCushion;
}

unsigned short PersonalizationEvent::getPassengerSeatPositionSideCushion()
{
	return passengerSeatPositionSideCushion;
}

unsigned short PersonalizationEvent::getDashboardIllumination()
{
	return dashboardIllumination;
}

unsigned short PersonalizationEvent::getGeneratedVehicleSoundMode()
{
	return generatedVehicleSoundMode;
}

void PersonalizationEvent::setType(VehicleEventType type)
{
	this->type = type;
}

void PersonalizationEvent::setKeyId(DOMString keyId)
{
	this->keyId = keyId;
}

void PersonalizationEvent::setLanguage(unsigned short language)
{
	this->language = language;
}

void PersonalizationEvent::setMeasurementSystem(bool measurementSystem)
{
	this->measurementSystem = measurementSystem;
}

void PersonalizationEvent::setMeasurementSystemStringFuel(DOMString measurementSystemStringFuel)
{
	this->measurementSystemStringFuel = measurementSystemStringFuel;
}

void PersonalizationEvent::setMeasurementSystemStringDistance(DOMString measurementSystemStringDistance)
{
	this->measurementSystemStringDistance = measurementSystemStringDistance;
}

void PersonalizationEvent::setMeasurementSystemStringSpeed(DOMString measurementSystemStringSpeed)
{
	this->measurementSystemStringSpeed = measurementSystemStringSpeed;
}

void PersonalizationEvent::setMeasurementSystemStringConsumption(DOMString measurementSystemStringConsumption)
{
	this->measurementSystemStringConsumption = measurementSystemStringConsumption;
}

void PersonalizationEvent::setMirrorDriver(unsigned short mirrorDriver)
{
	this->mirrorDriver = mirrorDriver;
}

void PersonalizationEvent::setMirrorPassenger(unsigned short mirrorPassenger)
{
	this->mirrorPassenger = mirrorPassenger;
}

void PersonalizationEvent::setMirrorInside(unsigned short mirrorInside)
{
	this->mirrorInside = mirrorInside;
}

void PersonalizationEvent::setSteeringWheelPositionSlide(unsigned short steeringWheelPositionSlide)
{
	this->steeringWheelPositionSlide = steeringWheelPositionSlide;
}

void PersonalizationEvent::setSteeringWheelPositionTilt(unsigned short steeringWheelPositionTilt)
{
	this->steeringWheelPositionTilt = steeringWheelPositionTilt;
}

void PersonalizationEvent::setDrivingMode(unsigned short drivingMode)
{
	this->drivingMode = drivingMode;
}

void PersonalizationEvent::setDriverSeatPositionReclineSeatback(unsigned short driverSeatPositionReclineSeatback)
{
	this->driverSeatPositionReclineSeatback = driverSeatPositionReclineSeatback;
}

void PersonalizationEvent::setDriverSeatPositionSlide(unsigned short driverSeatPositionSlide)
{
	this->driverSeatPositionSlide = driverSeatPositionSlide;
}

void PersonalizationEvent::setDriverSeatPositionCushionHeight(unsigned short driverSeatPositionCushionHeight)
{
	this->driverSeatPositionCushionHeight = driverSeatPositionCushionHeight;
}

void PersonalizationEvent::setDriverSeatPositionHeadrest(unsigned short driverSeatPositionHeadrest)
{
	this->driverSeatPositionHeadrest = driverSeatPositionHeadrest;
}

void PersonalizationEvent::setDriverSeatPositionBackCushion(unsigned short driverSeatPositionBackCushion)
{
	this->driverSeatPositionBackCushion = driverSeatPositionBackCushion;
}

void PersonalizationEvent::setDriverSeatPositionSideCushion(unsigned short driverSeatPositionSideCushion)
{
	this->driverSeatPositionSideCushion = driverSeatPositionSideCushion;
}

void PersonalizationEvent::setPassengerSeatPositionReclineSeatback(unsigned short passengerSeatPositionReclineSeatback)
{
	this->passengerSeatPositionReclineSeatback = passengerSeatPositionReclineSeatback;
}

void PersonalizationEvent::setPassengerSeatPositionSlide(unsigned short passengerSeatPositionSlide)
{
	this->passengerSeatPositionSlide = passengerSeatPositionSlide;
}

void PersonalizationEvent::setPassengerSeatPositionCushionHeight(unsigned short passengerSeatPositionCushionHeight)
{
	this->passengerSeatPositionCushionHeight = passengerSeatPositionCushionHeight;
}

void PersonalizationEvent::setPassengerSeatPositionHeadrest(unsigned short passengerSeatPositionHeadrest)
{
	this->passengerSeatPositionHeadrest = passengerSeatPositionHeadrest;
}

void PersonalizationEvent::setPassengerSeatPositionBackCushion(unsigned short passengerSeatPositionBackCushion)
{
	this->passengerSeatPositionBackCushion = passengerSeatPositionBackCushion;
}

void PersonalizationEvent::setPassengerSeatPositionSideCushion(unsigned short passengerSeatPositionSideCushion)
{
	this->passengerSeatPositionSideCushion = passengerSeatPositionSideCushion;
}

void PersonalizationEvent::setDashboardIllumination(unsigned short dashboardIllumiation)
{
	this->dashboardIllumination = dashboardIllumination;
}

void PersonalizationEvent::setGeneratedVehicleSoundMode(unsigned short generatedVehicleSoundMode)
{
	this->generatedVehicleSoundMode = generatedVehicleSoundMode;
}

