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
 * @file ClimateEnvironmentEvent.cpp
 * For further information, see http://www.genivi.org/
 */

#include "JSObject.h"
#include <boost/assign.hpp>
#include "DOM/Document.h"

#include "ClimateEnvironmentEvent.h"

const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT = "climateenvironment";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_INTERIORTEMP = "climateenvironment_interiortemp";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_EXTERIORTEMP = "climateenvironment_exteriortemp";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_EXTERIORBRIGHTNESS = "climateenvironment_exteriorbrightness";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_RAINSENSOR = "climateenvironment_rainsensor";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_WINDSHIELDWIPER = "climateenvironment_windshieldwiper";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_REARWIPER = "climateenvironment_rearwiper";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_HVACFAN = "climateenvironment_hvacfan";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_HVACFAN_DIRECTION = "climateenvironment_hvacfan_direction";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_HVACFAN_SPEED = "climateenvironment_hvacfan_speed";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_HVACFAN_TARGETTEMP = "climateenvironment_hvacfan_targettemp";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_AIRCONDITIONING = "climateenvironment_airconditioning";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_AIRRECIRCULATION = "climateenvironment_airrecirculation";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_HEATER = "climateenvironment_heater";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_DEFROST = "climateenvironment_defrost";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_DEFROST_WINDSHIELD = "climateenvironment_defrost_windshield";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_DEFROST_REARWINDOW = "climateenvironment_defrost_rearwindow";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_DEFROST_SIDEMIRRORS = "climateenvironment_defrost_sidemirrors";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_STEERINGWHEELHEATER = "climateenvironment_steeringwheelheater";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_SEATHEATER = "climateenvironment_seatheater";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_SEATCOOLER = "climateenvironment_seatcooler";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_WINDOW = "climateenvironment_window";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_WINDOW_DRIVER = "climateenvironment_window_driver";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_WINDOW_PASSENGER = "climateenvironment_window_passenger";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_WINDOW_REARLEFT = "climateenvironment_window_rearleft";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_WINDOW_REARRIGHT = "climateenvironment_window_rearright";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_SUNROOF = "climateenvironment_sunroof";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_SUNROOF_OPENNESS = "climateenvironment_sunroof_openness";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_SUNROOF_TILT = "climateenvironment_sunroof_tilt";
const VehicleEventType ClimateEnvironmentEvent::CLIMATEENVIRONMENT_CONVERTIBLEROOF = "climateenvironment_convertibleroof";

ClimateEnvironmentEvent::ClimateEnvironmentEvent(const FB::BrowserHostPtr& host)
{
	registerProperty("type",
					 make_property(this,
								   &ClimateEnvironmentEvent::getType));
	registerProperty("interiorTemp",
					 make_property(this,
								   &ClimateEnvironmentEvent::getInteriorTemp));
	registerProperty("exteriorTemp",
					 make_property(this,
								   &ClimateEnvironmentEvent::getExteriorTemp));
	registerProperty("exteriorBrightness",
					 make_property(this,
								   &ClimateEnvironmentEvent::getExteriorBrightness));
	registerProperty("rainSensor",
					 make_property(this,
								   &ClimateEnvironmentEvent::getRainSensor));
	registerProperty("windshieldWiper",
					 make_property(this,
								   &ClimateEnvironmentEvent::getWindshieldWiper));
	registerProperty("rearWiper",
					 make_property(this,
								   &ClimateEnvironmentEvent::getRearWiper));
	registerProperty("hvacFanDirection",
					 make_property(this,
								   &ClimateEnvironmentEvent::getHvacFanDirection));
	registerProperty("hvacFanSpeed",
					 make_property(this,
								   &ClimateEnvironmentEvent::getHvacFanSpeed));
	registerProperty("hvacFanTargetTemp",
					 make_property(this,
								   &ClimateEnvironmentEvent::getHvacFanTargetTemp));
	registerProperty("airConditioning",
					 make_property(this,
								   &ClimateEnvironmentEvent::getAirConditioning));
	registerProperty("airRecirculation",
					 make_property(this,
								   &ClimateEnvironmentEvent::getAirRecirculation));
	registerProperty("heater",
					 make_property(this,
								   &ClimateEnvironmentEvent::getHeater));
	registerProperty("defrostWindshield",
					 make_property(this,
								   &ClimateEnvironmentEvent::getDefrostWindshield));
	registerProperty("defrostRearWindow",
					 make_property(this,
								   &ClimateEnvironmentEvent::getDefrostRearWindow));
	registerProperty("defrostSideMirrors",
					 make_property(this,
								   &ClimateEnvironmentEvent::getDefrostSideMirrors));
	registerProperty("steeringWheelHeater",
					 make_property(this,
								   &ClimateEnvironmentEvent::getSteeringWheelHeater));
	registerProperty("seatHeater",
					 make_property(this,
								   &ClimateEnvironmentEvent::getSeatHeater));
	registerProperty("seatCooler",
					 make_property(this,
								   &ClimateEnvironmentEvent::getSeatCooler));
	registerProperty("windowDriver",
					 make_property(this,
								   &ClimateEnvironmentEvent::getWindowDriver));
	registerProperty("windowPassenger",
					 make_property(this,
								   &ClimateEnvironmentEvent::getWindowPassenger));
	registerProperty("windowRearLeft",
					 make_property(this,
								   &ClimateEnvironmentEvent::getWindowRearLeft));
	registerProperty("windowRearRight",
					 make_property(this,
								   &ClimateEnvironmentEvent::getWindowRearRight));
	registerProperty("sunroofOpenness",
					 make_property(this,
								   &ClimateEnvironmentEvent::getSunroofOpenness));
	registerProperty("sunroofTilt",
					 make_property(this,
								   &ClimateEnvironmentEvent::getSunroofTilt));
	registerProperty("convertibleRoof",
					 make_property(this,
								   &ClimateEnvironmentEvent::getConvertibleRoof));

	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_INTERIORTEMP);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_EXTERIORTEMP);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_EXTERIORBRIGHTNESS);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_RAINSENSOR);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_WINDSHIELDWIPER);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_REARWIPER);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_HVACFAN);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_HVACFAN_DIRECTION);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_HVACFAN_SPEED);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_HVACFAN_TARGETTEMP);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_AIRCONDITIONING);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_AIRRECIRCULATION);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_HEATER);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_DEFROST);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_DEFROST_WINDSHIELD);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_DEFROST_REARWINDOW);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_DEFROST_SIDEMIRRORS);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_STEERINGWHEELHEATER);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_SEATHEATER);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_SEATCOOLER);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_WINDOW);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_WINDOW_DRIVER);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_WINDOW_PASSENGER);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_WINDOW_REARLEFT);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_WINDOW_REARRIGHT);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_SUNROOF);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_SUNROOF_OPENNESS);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_SUNROOF_TILT);
	allSupportedEventTypesR.push_back(CLIMATEENVIRONMENT_CONVERTIBLEROOF);

	allSupportedEventTypesW.push_back(CLIMATEENVIRONMENT_WINDSHIELDWIPER);
	allSupportedEventTypesW.push_back(CLIMATEENVIRONMENT_REARWIPER);
	allSupportedEventTypesW.push_back(CLIMATEENVIRONMENT_HVACFAN);
	allSupportedEventTypesW.push_back(CLIMATEENVIRONMENT_HVACFAN_DIRECTION);
	allSupportedEventTypesW.push_back(CLIMATEENVIRONMENT_HVACFAN_SPEED);
	allSupportedEventTypesW.push_back(CLIMATEENVIRONMENT_HVACFAN_TARGETTEMP);
	allSupportedEventTypesW.push_back(CLIMATEENVIRONMENT_AIRCONDITIONING);
	allSupportedEventTypesW.push_back(CLIMATEENVIRONMENT_AIRRECIRCULATION);
	allSupportedEventTypesW.push_back(CLIMATEENVIRONMENT_HEATER);
	allSupportedEventTypesW.push_back(CLIMATEENVIRONMENT_DEFROST);
	allSupportedEventTypesW.push_back(CLIMATEENVIRONMENT_DEFROST_WINDSHIELD);
	allSupportedEventTypesW.push_back(CLIMATEENVIRONMENT_DEFROST_REARWINDOW);
	allSupportedEventTypesW.push_back(CLIMATEENVIRONMENT_DEFROST_SIDEMIRRORS);
	allSupportedEventTypesW.push_back(CLIMATEENVIRONMENT_STEERINGWHEELHEATER);
	allSupportedEventTypesW.push_back(CLIMATEENVIRONMENT_SEATHEATER);
	allSupportedEventTypesW.push_back(CLIMATEENVIRONMENT_SEATCOOLER);
	allSupportedEventTypesW.push_back(CLIMATEENVIRONMENT_SUNROOF);
	allSupportedEventTypesW.push_back(CLIMATEENVIRONMENT_SUNROOF_OPENNESS);
	allSupportedEventTypesW.push_back(CLIMATEENVIRONMENT_SUNROOF_TILT);
	allSupportedEventTypesW.push_back(CLIMATEENVIRONMENT_CONVERTIBLEROOF);

	validity[CLIMATEENVIRONMENT] = false;
	validity[CLIMATEENVIRONMENT_INTERIORTEMP] = false;
	validity[CLIMATEENVIRONMENT_EXTERIORTEMP] = false;
	validity[CLIMATEENVIRONMENT_EXTERIORBRIGHTNESS] = false;
	validity[CLIMATEENVIRONMENT_RAINSENSOR] = false;
	validity[CLIMATEENVIRONMENT_WINDSHIELDWIPER] = false;
	validity[CLIMATEENVIRONMENT_REARWIPER] = false;
	validity[CLIMATEENVIRONMENT_HVACFAN] = false;
	validity[CLIMATEENVIRONMENT_HVACFAN_DIRECTION] = false;
	validity[CLIMATEENVIRONMENT_HVACFAN_SPEED] = false;
	validity[CLIMATEENVIRONMENT_HVACFAN_TARGETTEMP] = false;
	validity[CLIMATEENVIRONMENT_AIRCONDITIONING] = false;
	validity[CLIMATEENVIRONMENT_AIRRECIRCULATION] = false;
	validity[CLIMATEENVIRONMENT_HEATER] = false;
	validity[CLIMATEENVIRONMENT_DEFROST] = false;
	validity[CLIMATEENVIRONMENT_DEFROST_WINDSHIELD] = false;
	validity[CLIMATEENVIRONMENT_DEFROST_REARWINDOW] = false;
	validity[CLIMATEENVIRONMENT_DEFROST_SIDEMIRRORS] = false;
	validity[CLIMATEENVIRONMENT_STEERINGWHEELHEATER] = false;
	validity[CLIMATEENVIRONMENT_SEATHEATER] = false;
	validity[CLIMATEENVIRONMENT_SEATCOOLER] = false;
	validity[CLIMATEENVIRONMENT_WINDOW] = false;
	validity[CLIMATEENVIRONMENT_WINDOW_DRIVER] = false;
	validity[CLIMATEENVIRONMENT_WINDOW_PASSENGER] = false;
	validity[CLIMATEENVIRONMENT_WINDOW_REARLEFT] = false;
	validity[CLIMATEENVIRONMENT_WINDOW_REARRIGHT] = false;
	validity[CLIMATEENVIRONMENT_SUNROOF] = false;
	validity[CLIMATEENVIRONMENT_SUNROOF_OPENNESS] = false;
	validity[CLIMATEENVIRONMENT_SUNROOF_TILT] = false;
	validity[CLIMATEENVIRONMENT_CONVERTIBLEROOF] = false;
}

ClimateEnvironmentEvent::~ClimateEnvironmentEvent()
{

}

void ClimateEnvironmentEvent::updateValidity(VehicleEventType type)
{
	std::string str = type;
	size_t pos;

	while ((pos = str.find_last_of('_')) != std::string::npos)
	{
		str = str.substr(0, pos);

		if (str.compare(CLIMATEENVIRONMENT) == 0)
		{
			setValidity(CLIMATEENVIRONMENT, isValid(CLIMATEENVIRONMENT_INTERIORTEMP)
						& isValid(CLIMATEENVIRONMENT_EXTERIORTEMP)
						& isValid(CLIMATEENVIRONMENT_EXTERIORBRIGHTNESS)
						& isValid(CLIMATEENVIRONMENT_RAINSENSOR)
						& isValid(CLIMATEENVIRONMENT_WINDSHIELDWIPER)
						& isValid(CLIMATEENVIRONMENT_REARWIPER)
						& isValid(CLIMATEENVIRONMENT_HVACFAN)
						& isValid(CLIMATEENVIRONMENT_AIRCONDITIONING)
						& isValid(CLIMATEENVIRONMENT_AIRRECIRCULATION)
						& isValid(CLIMATEENVIRONMENT_HEATER)
						& isValid(CLIMATEENVIRONMENT_DEFROST)
						& isValid(CLIMATEENVIRONMENT_STEERINGWHEELHEATER)
						& isValid(CLIMATEENVIRONMENT_SEATHEATER)
						& isValid(CLIMATEENVIRONMENT_SEATCOOLER)
						& isValid(CLIMATEENVIRONMENT_WINDOW)
						& isValid(CLIMATEENVIRONMENT_SUNROOF)
						& isValid(CLIMATEENVIRONMENT_CONVERTIBLEROOF));
		}
		else if (str.compare(CLIMATEENVIRONMENT_HVACFAN) == 0)
		{
			setValidity(CLIMATEENVIRONMENT_HVACFAN, isValid(CLIMATEENVIRONMENT_HVACFAN_DIRECTION)
						& isValid(CLIMATEENVIRONMENT_HVACFAN_SPEED)
						& isValid(CLIMATEENVIRONMENT_HVACFAN_TARGETTEMP));

		}
		else if (str.compare(CLIMATEENVIRONMENT_DEFROST) == 0)
		{
			setValidity(CLIMATEENVIRONMENT_DEFROST, isValid(CLIMATEENVIRONMENT_DEFROST_WINDSHIELD)
						& isValid(CLIMATEENVIRONMENT_DEFROST_REARWINDOW)
						& isValid(CLIMATEENVIRONMENT_DEFROST_SIDEMIRRORS));

		}
		else if (str.compare(CLIMATEENVIRONMENT_WINDOW) == 0)
		{
			setValidity(CLIMATEENVIRONMENT_WINDOW, isValid(CLIMATEENVIRONMENT_WINDOW_DRIVER)
						& isValid(CLIMATEENVIRONMENT_WINDOW_PASSENGER)
						& isValid(CLIMATEENVIRONMENT_WINDOW_REARLEFT)
						& isValid(CLIMATEENVIRONMENT_WINDOW_REARRIGHT));

		}
		else if (str.compare(CLIMATEENVIRONMENT_SUNROOF) == 0)
		{
			setValidity(CLIMATEENVIRONMENT_SUNROOF, isValid(CLIMATEENVIRONMENT_SUNROOF_OPENNESS)
						& isValid(CLIMATEENVIRONMENT_SUNROOF_TILT));

		}
	}
}

VehicleEventType ClimateEnvironmentEvent::getType()
{
	return type;
}

short ClimateEnvironmentEvent::getInteriorTemp()
{
	return interiorTemp;
}

short ClimateEnvironmentEvent::getExteriorTemp()
{
	return exteriorTemp;
}

unsigned long ClimateEnvironmentEvent::getExteriorBrightness()
{
	return exteriorBrightness;
}

unsigned short ClimateEnvironmentEvent::getRainSensor()
{
	return rainSensor;
}

unsigned short ClimateEnvironmentEvent::getWindshieldWiper()
{
	return windshieldWiper;
}

unsigned short ClimateEnvironmentEvent::getRearWiper()
{
	return rearWiper;
}

unsigned short ClimateEnvironmentEvent::getHvacFanDirection()
{
	return hvacFanDirection;
}

unsigned short ClimateEnvironmentEvent::getHvacFanSpeed()
{
	return hvacFanSpeed;
}

short ClimateEnvironmentEvent::getHvacFanTargetTemp()
{
	return hvacFanTargetTemp;
}

bool ClimateEnvironmentEvent::getAirConditioning()
{
	return airConditioning;
}

bool ClimateEnvironmentEvent::getAirRecirculation()
{
	return airRecirculation;
}

bool ClimateEnvironmentEvent::getHeater()
{
	return heater;
}

bool ClimateEnvironmentEvent::getDefrostWindshield()
{
	return defrostWindshield;
}

bool ClimateEnvironmentEvent::getDefrostRearWindow()
{
	return defrostRearWindow;
}

bool ClimateEnvironmentEvent::getDefrostSideMirrors()
{
	return defrostSideMirrors;
}

bool ClimateEnvironmentEvent::getSteeringWheelHeater()
{
	return steeringWheelHeater;
}

bool ClimateEnvironmentEvent::getSeatHeater()
{
	return seatHeater;
}

bool ClimateEnvironmentEvent::getSeatCooler()
{
	return seatCooler;
}

unsigned short ClimateEnvironmentEvent::getWindowDriver()
{
	return windowDriver;
}

unsigned short ClimateEnvironmentEvent::getWindowPassenger()
{
	return windowPassenger;
}

unsigned short ClimateEnvironmentEvent::getWindowRearLeft()
{
	return windowRearLeft;
}

unsigned short ClimateEnvironmentEvent::getWindowRearRight()
{
	return windowRearRight;
}

unsigned short ClimateEnvironmentEvent::getSunroofOpenness()
{
	return sunroofOpenness;
}

unsigned short ClimateEnvironmentEvent::getSunroofTilt()
{
	return sunroofTilt;
}

bool ClimateEnvironmentEvent::getConvertibleRoof()
{
	return convertibleRoof;
}

void ClimateEnvironmentEvent::setType(VehicleEventType type)
{
	this->type = type;
}

void ClimateEnvironmentEvent::setInteriorTemp(short interiorTemp)
{
	this->interiorTemp = interiorTemp;
}

void ClimateEnvironmentEvent::setExteriorTemp(short exteriorTemp)
{
	this->exteriorTemp = exteriorTemp;
}

void ClimateEnvironmentEvent::setExteriorBrightness(unsigned long exteriorBrightness)
{
	this->exteriorBrightness = exteriorBrightness;
}

void ClimateEnvironmentEvent::setRainSensor(unsigned short rainSensor)
{
	this->rainSensor = rainSensor;
}

void ClimateEnvironmentEvent::setWindshieldWiper(unsigned short windshieldWiper)
{
	this->windshieldWiper = windshieldWiper;
}

void ClimateEnvironmentEvent::setRearWiper(unsigned short rearWiper)
{
	this->rearWiper = rearWiper;
}

void ClimateEnvironmentEvent::setHvacFanDirection(unsigned short hvacFanDirection)
{
	this->hvacFanDirection = hvacFanDirection;
}

void ClimateEnvironmentEvent::setHvacFanSpeed(unsigned short hvacFanSpeed)
{
	this->hvacFanSpeed = hvacFanSpeed;
}

void ClimateEnvironmentEvent::setHvacFanTargetTemp(short hvacFanTargetTemp)
{
	this->hvacFanTargetTemp = hvacFanTargetTemp;
}

void ClimateEnvironmentEvent::setAirConditioning(bool airConditioning)
{
	this->airConditioning = airConditioning;
}

void ClimateEnvironmentEvent::setAirRecirculation(bool airRecirculation)
{
	this->airRecirculation = airRecirculation;
}

void ClimateEnvironmentEvent::setHeater(bool heater)
{
	this->heater = heater;
}

void ClimateEnvironmentEvent::setDefrostWindshield(bool defrostWindshield)
{
	this->defrostWindshield = defrostWindshield;
}

void ClimateEnvironmentEvent::setDefrostRearWindow(bool defrostRearWindow)
{
	this->defrostRearWindow = defrostRearWindow;
}

void ClimateEnvironmentEvent::setDefrostSideMirrors(bool defrostSideMirrors)
{
	this->defrostSideMirrors = defrostSideMirrors;
}

void ClimateEnvironmentEvent::setSteeringWheelHeater(bool steeringWheelHeater)
{
	this->steeringWheelHeater = steeringWheelHeater;
}

void ClimateEnvironmentEvent::setSeatHeater(bool seatHeater)
{
	this->seatHeater = seatHeater;
}

void ClimateEnvironmentEvent::setSeatCooler(bool seatCooler)
{
	this->seatCooler = seatCooler;
}

void ClimateEnvironmentEvent::setWindowDriver(unsigned short windowDriver)
{
	this->windowDriver = windowDriver;
}

void ClimateEnvironmentEvent::setWindowPassenger(unsigned short windowPassenger)
{
	this->windowPassenger = windowPassenger;
}

void ClimateEnvironmentEvent::setWindowRearLeft(unsigned short windowRearLeft)
{
	this->windowRearLeft = windowRearLeft;
}

void ClimateEnvironmentEvent::setWindowRearRight(unsigned short windowRearRight)
{
	this->windowRearRight = windowRearRight;
}

void ClimateEnvironmentEvent::setSunroofOpenness(unsigned short sunroofOpenness)
{
	this->sunroofOpenness = sunroofOpenness;
}

void ClimateEnvironmentEvent::setSunroofTilt(unsigned short sunroofTilt)
{
	this->sunroofTilt = sunroofTilt;
}

void ClimateEnvironmentEvent::setConvertibleRoof(bool convertibleRoof)
{
	this->convertibleRoof = convertibleRoof;
}

