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
 * @file PersonalizationEvent.h 
 * For further information, see http://www.genivi.org/
 */

#ifndef H_PersonalizationEvent
#define H_PersonalizationEvent

#include <string>
#include <sstream>
#include "VehicleEvent.h"
#include "TypeDef.h"

class PersonalizationEvent : public VehicleEvent
{
public:
	PersonalizationEvent(const FB::BrowserHostPtr& host);
	virtual ~PersonalizationEvent();

	VehicleEventType getType();
	DOMString getKeyId();
	unsigned short getLanguage();
	bool getMeasurementSystem();
	DOMString getMeasurementSystemStringFuel();
	DOMString getMeasurementSystemStringDistance();
	DOMString getMeasurementSystemStringSpeed();
	DOMString getMeasurementSystemStringConsumption();
	unsigned short getMirrorDriver();
	unsigned short getMirrorPassenger();
	unsigned short getMirrorInside();
	unsigned short getSteeringWheelPositionSlide();
	unsigned short getSteeringWheelPositionTilt();
	unsigned short getDrivingMode();
	unsigned short getDriverSeatPositionReclineSeatback();
	unsigned short getDriverSeatPositionSlide();
	unsigned short getDriverSeatPositionCushionHeight();
	unsigned short getDriverSeatPositionHeadrest();
	unsigned short getDriverSeatPositionBackCushion();
	unsigned short getDriverSeatPositionSideCushion();
	unsigned short getPassengerSeatPositionReclineSeatback();
	unsigned short getPassengerSeatPositionSlide();
	unsigned short getPassengerSeatPositionCushionHeight();
	unsigned short getPassengerSeatPositionHeadrest();
	unsigned short getPassengerSeatPositionBackCushion();
	unsigned short getPassengerSeatPositionSideCushion();
	unsigned short getDashboardIllumination();
	unsigned short getGeneratedVehicleSoundMode();

	void setType(VehicleEventType type);
	void setKeyId(DOMString keyId);
	void setLanguage(unsigned short language);
	void setMeasurementSystem(bool measurementSystem);
	void setMeasurementSystemStringFuel(DOMString measurementSystemStringFuel);
	void setMeasurementSystemStringDistance(DOMString measurementSystemStringDistance);
	void setMeasurementSystemStringSpeed(DOMString measurementSystemStringSpeed);
	void setMeasurementSystemStringConsumption(DOMString measurementSystemStringConsumption);
	void setMirrorDriver(unsigned short mirrorDriver);
	void setMirrorPassenger(unsigned short mirrorPassenger);
	void setMirrorInside(unsigned short mirrorInside);
	void setSteeringWheelPositionSlide(unsigned short steeringWheelPositionSlide);
	void setSteeringWheelPositionTilt(unsigned short steeringWheelPositionTilt);
	void setDrivingMode(unsigned short drivingMode);
	void setDriverSeatPositionReclineSeatback(unsigned short driverSeatPositionReclineSeatback);
	void setDriverSeatPositionSlide(unsigned short driverSeatPositionSlide);
	void setDriverSeatPositionCushionHeight(unsigned short driverSeatPositionCushionHeight);
	void setDriverSeatPositionHeadrest(unsigned short driverSeatPositionHeadrest);
	void setDriverSeatPositionBackCushion(unsigned short driverSeatPositionBackCushion);
	void setDriverSeatPositionSideCushion(unsigned short driverSeatPositionSideCushion);
	void setPassengerSeatPositionReclineSeatback(unsigned short passengerSeatPositionReclineSeatback);
	void setPassengerSeatPositionSlide(unsigned short passengerSeatPositionSlide);
	void setPassengerSeatPositionCushionHeight(unsigned short passengerSeatPositionCushionHeight);
	void setPassengerSeatPositionHeadrest(unsigned short passengerSeatPositionHeadrest);
	void setPassengerSeatPositionBackCushion(unsigned short passengerSeatPositionBackCushion);
	void setPassengerSeatPositionSideCushion(unsigned short passengerSeatPositionSideCushion);
	void setDashboardIllumination(unsigned short dashboardIllumiation);
	void setGeneratedVehicleSoundMode(unsigned short generatedVehicleSoundMode);

	static const VehicleEventType PERSONALIZATION;
	static const VehicleEventType PERSONALIZATION_KEYID;
	static const VehicleEventType PERSONALIZATION_LANGUAGE;
	static const VehicleEventType PERSONALIZATION_MEASUREMENTSYSTEM;
	static const VehicleEventType PERSONALIZATION_MEASUREMENTSYSTEMSTRING;
	static const VehicleEventType PERSONALIZATION_MEASUREMENTSYSTEMSTRING_FUEL;
	static const VehicleEventType PERSONALIZATION_MEASUREMENTSYSTEMSTRING_DISTANCE;
	static const VehicleEventType PERSONALIZATION_MEASUREMENTSYSTEMSTRING_SPEED;
	static const VehicleEventType PERSONALIZATION_MEASUREMENTSYSTEMSTRING_CONSUMPTION;
	static const VehicleEventType PERSONALIZATION_MIRROR;
	static const VehicleEventType PERSONALIZATION_MIRROR_DRIVER;
	static const VehicleEventType PERSONALIZATION_MIRROR_PASSENGER;
	static const VehicleEventType PERSONALIZATION_MIRROR_INSIDE;
	static const VehicleEventType PERSONALIZATION_STEERINGWHEELPOSITION;
	static const VehicleEventType PERSONALIZATION_STEERINGWHEELPOSITION_SLIDE;
	static const VehicleEventType PERSONALIZATION_STEERINGWHEELPOSITION_TILT;
	static const VehicleEventType PERSONALIZATION_DRIVINGMODE;
	static const VehicleEventType PERSONALIZATION_DRIVERSEATPOSITION;
	static const VehicleEventType PERSONALIZATION_DRIVERSEATPOSITION_RECLINESEATBACK;
	static const VehicleEventType PERSONALIZATION_DRIVERSEATPOSITION_SLIDE;
	static const VehicleEventType PERSONALIZATION_DRIVERSEATPOSITION_CUSHIONHEIGHT;
	static const VehicleEventType PERSONALIZATION_DRIVERSEATPOSITION_HEADREST;
	static const VehicleEventType PERSONALIZATION_DRIVERSEATPOSITION_BACKCUSHION;
	static const VehicleEventType PERSONALIZATION_DRIVERSEATPOSITION_SIDECUSHION;
	static const VehicleEventType PERSONALIZATION_PASSENGERSEATPOSITION;
	static const VehicleEventType PERSONALIZATION_PASSENGERSEATPOSITION_RECLINESEATBACK;
	static const VehicleEventType PERSONALIZATION_PASSENGERSEATPOSITION_SLIDE;
	static const VehicleEventType PERSONALIZATION_PASSENGERSEATPOSITION_CUSHIONHEIGHT;
	static const VehicleEventType PERSONALIZATION_PASSENGERSEATPOSITION_HEADREST;
	static const VehicleEventType PERSONALIZATION_PASSENGERSEATPOSITION_BACKCUSHION;
	static const VehicleEventType PERSONALIZATION_PASSENGERSEATPOSITION_SIDECUSHION;
	static const VehicleEventType PERSONALIZATION_DASHBOARDILLUMINATION;
	static const VehicleEventType PERSONALIZATION_GENERATEDVEHICLESOUNDMODE;

	static const unsigned short LANGUAGE_ENGLISH = 1;
	static const unsigned short LANGUAGE_SPANISH = 2;
	static const unsigned short LANGUAGE_FRENCH = 3;
		
	static const unsigned short DRIVINGMODE_COMFORT = 1;
	static const unsigned short DRIVINGMODE_AUTO = 2;
	static const unsigned short DRIVINGMODE_SPORT = 3;
	static const unsigned short DRIVINGMODE_ECO = 4;
	static const unsigned short DRIVINGMODE_MANUAL = 5;
		
	static const unsigned short GENERATEDVEHICLESOUNDMODE_NORMAL = 1;
	static const unsigned short GENERATEDVEHICLESOUNDMODE_QUIET = 2;
	static const unsigned short GENERATEDVEHICLESOUNDMODE_SPORTIVE = 3;

private:
	virtual void updateValidity(VehicleEventType type);

	FB::BrowserHostPtr m_host;

	VehicleEventType type;
	DOMString keyId;
	unsigned short language;
	bool measurementSystem;
	DOMString measurementSystemStringFuel;
	DOMString measurementSystemStringDistance;
	DOMString measurementSystemStringSpeed;
	DOMString measurementSystemStringConsumption;
	unsigned short mirrorDriver;
	unsigned short mirrorPassenger;
	unsigned short mirrorInside;
	unsigned short steeringWheelPositionSlide;
	unsigned short steeringWheelPositionTilt;
	unsigned short drivingMode;
	unsigned short driverSeatPositionReclineSeatback;
	unsigned short driverSeatPositionSlide;
	unsigned short driverSeatPositionCushionHeight;
	unsigned short driverSeatPositionHeadrest;
	unsigned short driverSeatPositionBackCushion;
	unsigned short driverSeatPositionSideCushion;
	unsigned short passengerSeatPositionReclineSeatback;
	unsigned short passengerSeatPositionSlide;
	unsigned short passengerSeatPositionCushionHeight;
	unsigned short passengerSeatPositionHeadrest;
	unsigned short passengerSeatPositionBackCushion;
	unsigned short passengerSeatPositionSideCushion;
	unsigned short dashboardIllumination;
	unsigned short generatedVehicleSoundMode;
};

#endif // H_PersonalizationEvent
