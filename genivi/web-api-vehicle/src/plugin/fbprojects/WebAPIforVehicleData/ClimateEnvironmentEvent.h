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
 * @file ClimateEnvironmentEvent.h 
 * For further information, see http://www.genivi.org/
 */

#ifndef H_ClimateEnvironmentEvent
#define H_ClimateEnvironmentEvent

#include <string>
#include <sstream>
#include "VehicleEvent.h"
#include "TypeDef.h"

class ClimateEnvironmentEvent : public VehicleEvent
{
public:
	ClimateEnvironmentEvent(const FB::BrowserHostPtr& host);
	virtual ~ClimateEnvironmentEvent();

	VehicleEventType getType();
	short getInteriorTemp();
	short getExteriorTemp();
	unsigned long getExteriorBrightness();
	unsigned short getRainSensor();
	unsigned short getWindshieldWiper();
	unsigned short getRearWiper();
	unsigned short getHvacFanDirection();
	unsigned short getHvacFanSpeed();
	short getHvacFanTargetTemp();
	bool getAirConditioning();
	bool getAirRecirculation();
	bool getHeater();
	bool getDefrostWindshield();
	bool getDefrostRearWindow();
	bool getDefrostSideMirrors();
	bool getSteeringWheelHeater();
	bool getSeatHeater();
	bool getSeatCooler();
	unsigned short getWindowDriver();
	unsigned short getWindowPassenger();
	unsigned short getWindowRearLeft();
	unsigned short getWindowRearRight();
	unsigned short getSunroofOpenness();
	unsigned short getSunroofTilt();
	bool getConvertibleRoof();

	void setType(VehicleEventType type);
	void setInteriorTemp(short interiorTemp);
	void setExteriorTemp(short exteriorTemp);
	void setExteriorBrightness(unsigned long exteriorBrightness);
	void setRainSensor(unsigned short rainSensor);
	void setWindshieldWiper(unsigned short windshieldWiper);
	void setRearWiper(unsigned short rearWiper);
	void setHvacFanDirection(unsigned short hvacFanDirection);
	void setHvacFanSpeed(unsigned short hvacFanSpeed);
	void setHvacFanTargetTemp(short hvacFanTargetTemp);
	void setAirConditioning(bool airConditioning);
	void setAirRecirculation(bool airRecirculation);
	void setHeater(bool heater);
	void setDefrostWindshield(bool defrostWindshield);
	void setDefrostRearWindow(bool defrostRearWindow);
	void setDefrostSideMirrors(bool defrostSideMirrors);
	void setSteeringWheelHeater(bool steeringWheelHeater);
	void setSeatHeater(bool seatHeater);
	void setSeatCooler(bool seatCooler);
	void setWindowDriver(unsigned short windowDriver);
	void setWindowPassenger(unsigned short windowPassenger);
	void setWindowRearLeft(unsigned short windowRearLeft);
	void setWindowRearRight(unsigned short windowRearRight);
	void setSunroofOpenness(unsigned short sunroofOpenness);
	void setSunroofTilt(unsigned short sunroofTilt);
	void setConvertibleRoof(bool convertibleRoof);

	static const VehicleEventType CLIMATEENVIRONMENT;
	static const VehicleEventType CLIMATEENVIRONMENT_INTERIORTEMP;
	static const VehicleEventType CLIMATEENVIRONMENT_EXTERIORTEMP;
	static const VehicleEventType CLIMATEENVIRONMENT_EXTERIORBRIGHTNESS;
	static const VehicleEventType CLIMATEENVIRONMENT_RAINSENSOR;
	static const VehicleEventType CLIMATEENVIRONMENT_WINDSHIELDWIPER;
	static const VehicleEventType CLIMATEENVIRONMENT_REARWIPER;
	static const VehicleEventType CLIMATEENVIRONMENT_HVACFAN;
	static const VehicleEventType CLIMATEENVIRONMENT_HVACFAN_DIRECTION;
	static const VehicleEventType CLIMATEENVIRONMENT_HVACFAN_SPEED;
	static const VehicleEventType CLIMATEENVIRONMENT_HVACFAN_TARGETTEMP;
	static const VehicleEventType CLIMATEENVIRONMENT_AIRCONDITIONING;
	static const VehicleEventType CLIMATEENVIRONMENT_AIRRECIRCULATION;
	static const VehicleEventType CLIMATEENVIRONMENT_HEATER;
	static const VehicleEventType CLIMATEENVIRONMENT_DEFROST;
	static const VehicleEventType CLIMATEENVIRONMENT_DEFROST_WINDSHIELD;
	static const VehicleEventType CLIMATEENVIRONMENT_DEFROST_REARWINDOW;
	static const VehicleEventType CLIMATEENVIRONMENT_DEFROST_SIDEMIRRORS;
	static const VehicleEventType CLIMATEENVIRONMENT_STEERINGWHEELHEATER;
	static const VehicleEventType CLIMATEENVIRONMENT_SEATHEATER;
	static const VehicleEventType CLIMATEENVIRONMENT_SEATCOOLER;
	static const VehicleEventType CLIMATEENVIRONMENT_WINDOW;
	static const VehicleEventType CLIMATEENVIRONMENT_WINDOW_DRIVER;
	static const VehicleEventType CLIMATEENVIRONMENT_WINDOW_PASSENGER;
	static const VehicleEventType CLIMATEENVIRONMENT_WINDOW_REARLEFT;
	static const VehicleEventType CLIMATEENVIRONMENT_WINDOW_REARRIGHT;
	static const VehicleEventType CLIMATEENVIRONMENT_SUNROOF;
	static const VehicleEventType CLIMATEENVIRONMENT_SUNROOF_OPENNESS;
	static const VehicleEventType CLIMATEENVIRONMENT_SUNROOF_TILT;
	static const VehicleEventType CLIMATEENVIRONMENT_CONVERTIBLEROOF;

private:
	virtual void updateValidity(VehicleEventType type);

	FB::BrowserHostPtr m_host;

	VehicleEventType type;
	short interiorTemp;
	short exteriorTemp;
	unsigned long exteriorBrightness;
	unsigned short rainSensor;
	unsigned short windshieldWiper;
	unsigned short rearWiper;
	unsigned short hvacFanDirection;
	unsigned short hvacFanSpeed;
	short hvacFanTargetTemp;
	bool airConditioning;
	bool airRecirculation;
	bool heater;
	bool defrostWindshield;
	bool defrostRearWindow;
	bool defrostSideMirrors;
	bool steeringWheelHeater;
	bool seatHeater;
	bool seatCooler;
	unsigned short windowDriver;
	unsigned short windowPassenger;
	unsigned short windowRearLeft;
	unsigned short windowRearRight;
	unsigned short sunroofOpenness;
	unsigned short sunroofTilt;
	bool convertibleRoof;
};

#endif // H_ClimateEnvironmentEvent
