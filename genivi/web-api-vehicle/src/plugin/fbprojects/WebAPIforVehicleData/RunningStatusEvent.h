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
 * @file RunningStatusEvent.h 
 * For further information, see http://www.genivi.org/
 */

#ifndef H_RunningStatusEvent
#define H_RunningStatusEvent

#include <string>
#include <sstream>
#include "VehicleEvent.h"
#include "TypeDef.h"

class RunningStatusEvent : public VehicleEvent
{
public:
	RunningStatusEvent(const FB::BrowserHostPtr& host);
	virtual ~RunningStatusEvent();

	VehicleEventType getType();
	unsigned short getVehiclePowerMode();
	unsigned short getSpeedometer();
	unsigned short getEngineSpeed();
	unsigned long getTripMeter1Mileage();
	unsigned long getTripMeter2Mileage();
	unsigned short getTripMeter1AverageSpeed();
	unsigned short getTripMeter2AverageSpeed();
	unsigned long getTripMeter1FuelConsumption();
	unsigned long getTripMeter2FuelConsumption();
	unsigned short getTransmissionGearStatus();
	bool getCruiseControlStatus();
	unsigned short getCruiseControlSpeed();
	unsigned short getWheelBrake();
	bool getLightsStatusHead();
	bool getLightsStatusHighBeam();
	bool getLightsStatusTurnLeft();
	bool getLightsStatusTurnRight();
	bool getLightsStatusBrake();
	bool getLightsStatusFogFront();
	bool getLightsStatusFogRear();
	bool getLightsStatusHazard();
	bool getLightsStatusParking();
	bool getInteriorLightsStatusDriver();
	bool getInteriorLightsStatusPassenger();
	bool getInteriorLightsStatusCenter();
	bool getAutomaticHeadlights();
	bool getDynamicHighBeam();
	bool getHorn();
	bool getChime();
	unsigned short getFuel();
	unsigned long getEstimatedRange();
	unsigned short getEngineOilRemaining();
	bool getEngineOilChange();
	short getEngineOilTemp();
	unsigned short getEngineCoolantLevel();
	short getEngineCoolantTemp();
	short getSteeringWheelAngle();

	void setType(VehicleEventType type);
	void setVehiclePowerMode(unsigned short vehiclePowerMode);
	void setSpeedometer(unsigned short speedometer);
	void setEngineSpeed(unsigned short engineSpeed);
	void setTripMeter1Mileage(unsigned long tripMeter1Mileage);
	void setTripMeter2Mileage(unsigned long tripMeter2Mileage);
	void setTripMeter1AverageSpeed(unsigned short tripMeter1AverageSpeed);
	void setTripMeter2AverageSpeed(unsigned short tripMeter2AverageSpeed);
	void setTripMeter1FuelConsumption(unsigned long tripMeter1FuelConsumption);
	void setTripMeter2FuelConsumption(unsigned long tripMeter2FuelConsumption);
	void setTransmissionGearStatus(unsigned short transmissionGearStatus);
	void setCruiseControlStatus(bool cruiseControlStatus);
	void setCruiseControlSpeed(unsigned short cruiseControlSpeed);
	void setWheelBrake(unsigned short wheelBrake);
	void setLightsStatusHead(bool lightsStatusHead);
	void setLightsStatusHighBeam(bool lightsStatusHighBeam);
	void setLightsStatusTurnLeft(bool lightsStatusTurnLeft);
	void setLightsStatusTurnRight(bool lightsStatusTurnRight);
	void setLightsStatusBrake(bool lightsStatusBrake);
	void setLightsStatusFogFront(bool lightsStatusFogFront);
	void setLightsStatusFogRear(bool lightsStatusFogRear);
	void setLightsStatusHazard(bool lightsStatusHazard);
	void setLightsStatusParking(bool lightsStatusParking);
	void setInteriorLightsStatusDriver(bool interiorLightsStatusDriver);
	void setInteriorLightsStatusPassenger(bool interiorLightsStatusPassenger);
	void setInteriorLightsStatusCenter(bool interiorLightsStatusCenter);
	void setAutomaticHeadlights(bool automaticHeadlights);
	void setDynamicHighBeam(bool dynamicHighBeam);
	void setHorn(bool horn);
	void setChime(bool chime);
	void setFuel(unsigned short fuel);
	void setEstimatedRange(unsigned long estimatedRange);
	void setEngineOilRemaining(unsigned short engineOilRemaining);
	void setEngineOilChange(bool engineOilChange);
	void setEngineOilTemp(short engineOilTemp);
	void setEngineCoolantLevel(unsigned short engineCoolantLevel);
	void setEngineCoolantTemp(short engineCoolantTemp);
	void setSteeringWheelAngle(short steeringWheelAngle);

	static const VehicleEventType RUNNINGSTATUS;
	static const VehicleEventType RUNNINGSTATUS_VEHICLEPOWERMODE;
	static const VehicleEventType RUNNINGSTATUS_SPEEDOMETER;
	static const VehicleEventType RUNNINGSTATUS_ENGINESPEED;
	static const VehicleEventType RUNNINGSTATUS_TRIPMETER;
	static const VehicleEventType RUNNINGSTATUS_TRIPMETER_1;
	static const VehicleEventType RUNNINGSTATUS_TRIPMETER_2;
	static const VehicleEventType RUNNINGSTATUS_TRIPMETER_1_MILEAGE;
	static const VehicleEventType RUNNINGSTATUS_TRIPMETER_2_MILEAGE;
	static const VehicleEventType RUNNINGSTATUS_TRIPMETER_1_AVERAGESPEED;
	static const VehicleEventType RUNNINGSTATUS_TRIPMETER_2_AVERAGESPEED;
	static const VehicleEventType RUNNINGSTATUS_TRIPMETER_1_FUELCONSUMPTION;
	static const VehicleEventType RUNNINGSTATUS_TRIPMETER_2_FUELCONSUMPTION;
	static const VehicleEventType RUNNINGSTATUS_TRANSMISSIONGEARSTATUS;
	static const VehicleEventType RUNNINGSTATUS_CRUISECONTROL;
	static const VehicleEventType RUNNINGSTATUS_CRUISECONTROL_STATUS;
	static const VehicleEventType RUNNINGSTATUS_CRUISECONTROL_SPEED;
	static const VehicleEventType RUNNINGSTATUS_WHEELBRAKE;
	static const VehicleEventType RUNNINGSTATUS_LIGHTSSTATUS;
	static const VehicleEventType RUNNINGSTATUS_LIGHTSSTATUS_HEAD;
	static const VehicleEventType RUNNINGSTATUS_LIGHTSSTATUS_HIGHBEAM;
	static const VehicleEventType RUNNINGSTATUS_LIGHTSSTATUS_TURNLEFT;
	static const VehicleEventType RUNNINGSTATUS_LIGHTSSTATUS_TURNRIGHT;
	static const VehicleEventType RUNNINGSTATUS_LIGHTSSTATUS_BRAKE;
	static const VehicleEventType RUNNINGSTATUS_LIGHTSSTATUS_FOGFRONT;
	static const VehicleEventType RUNNINGSTATUS_LIGHTSSTATUS_FOGREAR;
	static const VehicleEventType RUNNINGSTATUS_LIGHTSSTATUS_HAZARD;
	static const VehicleEventType RUNNINGSTATUS_LIGHTSSTATUS_PARKING;
	static const VehicleEventType RUNNINGSTATUS_INTERIORLIGHTSSTATUS;
	static const VehicleEventType RUNNINGSTATUS_INTERIORLIGHTSSTATUS_DRIVER;
	static const VehicleEventType RUNNINGSTATUS_INTERIORLIGHTSSTATUS_PASSENGER;
	static const VehicleEventType RUNNINGSTATUS_INTERIORLIGHTSSTATUS_CENTER;
	static const VehicleEventType RUNNINGSTATUS_AUTOMATICHEADLIGHTS;
	static const VehicleEventType RUNNINGSTATUS_DYNAMICHIGHBEAM;
	static const VehicleEventType RUNNINGSTATUS_HORN;
	static const VehicleEventType RUNNINGSTATUS_CHIME;
	static const VehicleEventType RUNNINGSTATUS_FUEL;
	static const VehicleEventType RUNNINGSTATUS_ESTIMATEDRANGE;
	static const VehicleEventType RUNNINGSTATUS_ENGINEOIL;
	static const VehicleEventType RUNNINGSTATUS_ENGINEOIL_REMAINING;
	static const VehicleEventType RUNNINGSTATUS_ENGINEOIL_CHANGE;
	static const VehicleEventType RUNNINGSTATUS_ENGINEOIL_TEMP;
	static const VehicleEventType RUNNINGSTATUS_ENGINECOOLANT;
	static const VehicleEventType RUNNINGSTATUS_ENGINECOOLANT_LEVEL;
	static const VehicleEventType RUNNINGSTATUS_ENGINECOOLANT_TEMP;
	static const VehicleEventType RUNNINGSTATUS_STEERINGWHEELANGLE;

	static const unsigned short VEHICLEPOWERMODE_OFF = 1;
	static const unsigned short VEHICLEPOWERMODE_ACC = 2;
	static const unsigned short VEHICLEPOWERMODE_RUN = 3;
	static const unsigned short VEHICLEPOWERMODE_IGNITION = 4;
		
	static const unsigned short TRANSMISSIONGEARSTATUS_NEUTRAL = 0;
	static const unsigned short TRANSMISSIONGEARSTATUS_MANUAL1 = 1;
	static const unsigned short TRANSMISSIONGEARSTATUS_MANUAL2 = 2;
	static const unsigned short TRANSMISSIONGEARSTATUS_MANUAL3 = 3;
	static const unsigned short TRANSMISSIONGEARSTATUS_MANUAL4 = 4;
	static const unsigned short TRANSMISSIONGEARSTATUS_MANUAL5 = 5;
	static const unsigned short TRANSMISSIONGEARSTATUS_MANUAL6 = 6;
	static const unsigned short TRANSMISSIONGEARSTATUS_MANUAL7 = 7;
	static const unsigned short TRANSMISSIONGEARSTATUS_MANUAL8 = 8;
	static const unsigned short TRANSMISSIONGEARSTATUS_MANUAL9 = 9;
	static const unsigned short TRANSMISSIONGEARSTATUS_MANUAL10 = 10;
	static const unsigned short TRANSMISSIONGEARSTATUS_AUTO1 = 1;
	static const unsigned short TRANSMISSIONGEARSTATUS_AUTO2 = 2;
	static const unsigned short TRANSMISSIONGEARSTATUS_AUTO3 = 3;
	static const unsigned short TRANSMISSIONGEARSTATUS_AUTO4 = 4;
	static const unsigned short TRANSMISSIONGEARSTATUS_AUTO5 = 5;
	static const unsigned short TRANSMISSIONGEARSTATUS_AUTO6 = 6;
	static const unsigned short TRANSMISSIONGEARSTATUS_AUTO7 = 7;
	static const unsigned short TRANSMISSIONGEARSTATUS_AUTO8 = 8;
	static const unsigned short TRANSMISSIONGEARSTATUS_AUTO9 = 9;
	static const unsigned short TRANSMISSIONGEARSTATUS_AUTO10 = 10;
	static const unsigned short TRANSMISSIONGEARSTATUS_DRIVE = 32;
	static const unsigned short TRANSMISSIONGEARSTATUS_PARKING = 64;
	static const unsigned short TRANSMISSIONGEARSTATUS_REVERSE = 128;
		
	static const unsigned short WHEELBRAKE_IDLE = 1;
	static const unsigned short WHEELBRAKE_ENGAGED = 2;
	static const unsigned short WHEELBRAKE_MALFUNCTION = 3;
		
	static const unsigned short ENGINECOOLANT_LEVEL_NORMAL = 0;
	static const unsigned short ENGINECOOLANT_LEVEL_LOW = 1;

private:
	virtual void updateValidity(VehicleEventType type);

	FB::BrowserHostPtr m_host;

	VehicleEventType type;
	unsigned short vehiclePowerMode;
	unsigned short speedometer;
	unsigned short engineSpeed;
	unsigned long tripMeter1Mileage;
	unsigned long tripMeter2Mileage;
	unsigned short tripMeter1AverageSpeed;
	unsigned short tripMeter2AverageSpeed;
	unsigned long tripMeter1FuelConsumption;
	unsigned long tripMeter2FuelConsumption;
	unsigned short transmissionGearStatus;
	bool cruiseControlStatus;
	unsigned short cruiseControlSpeed;
	unsigned short wheelBrake;
	bool lightsStatusHead;
	bool lightsStatusHighBeam;
	bool lightsStatusTurnLeft;
	bool lightsStatusTurnRight;
	bool lightsStatusBrake;
	bool lightsStatusFogFront;
	bool lightsStatusFogRear;
	bool lightsStatusHazard;
	bool lightsStatusParking;
	bool interiorLightsStatusDriver;
	bool interiorLightsStatusPassenger;
	bool interiorLightsStatusCenter;
	bool automaticHeadlights;
	bool dynamicHighBeam;
	bool horn;
	bool chime;
	unsigned short fuel;
	unsigned long estimatedRange;
	unsigned short engineOilRemaining;
	bool engineOilChange;
	short engineOilTemp;
	unsigned short engineCoolantLevel;
	short engineCoolantTemp;
	short steeringWheelAngle;
};

#endif // H_RunningStatusEvent
