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
 * @file MaintenanceEvent.h 
 * For further information, see http://www.genivi.org/
 */

#ifndef H_MaintenanceEvent
#define H_MaintenanceEvent

#include <string>
#include <sstream>
#include "VehicleEvent.h"
#include "TypeDef.h"

class MaintenanceEvent : public VehicleEvent
{
public:
	MaintenanceEvent(const FB::BrowserHostPtr& host);
	virtual ~MaintenanceEvent();

	VehicleEventType getType();
	unsigned long getOdometer();
	bool getTransmissionOilLifeLevel();
	short getTransmissionOilTemp();
	bool getBrakeFluidLevel();
	bool getWasherFluidLevel();
	bool getMalfunctionIndicatorLamp();
	unsigned short getBatteryVoltage();
	unsigned short getBatteryCurrent();
	unsigned short getTirePressureFrontLeft();
	unsigned short getTirePressureFrontRight();
	unsigned short getTirePressureRearLeft();
	unsigned short getTirePressureRearRight();
	unsigned short getTirePressureStatusFrontLeft();
	unsigned short getTirePressureStatusFrontRight();
	unsigned short getTirePressureStatusRearLeft();
	unsigned short getTirePressureStatusRearRight();

	void setType(VehicleEventType type);
	void setOdometer(unsigned long odometer);
	void setTransmissionOilLifeLevel(bool transmissionOilLifeLevel);
	void setTransmissionOilTemp(short transmissionOilTemp);
	void setBrakeFluidLevel(bool brakeFluidLevel);
	void setWasherFluidLevel(bool washerFluidLevel);
	void setMalfunctionIndicatorLamp(bool malfunctionIndicatorLamp);
	void setBatteryVoltage(unsigned short batteryVoltage);
	void setBatteryCurrent(unsigned short batteryCurrent);
	void setTirePressureFrontLeft(unsigned short tirePressureFrontLeft);
	void setTirePressureFrontRight(unsigned short tirePressureFrontRight);
	void setTirePressureRearLeft(unsigned short tirePressureRearLeft);
	void setTirePressureRearRight(unsigned short tirePressureRearRight);
	void setTirePressureStatusFrontLeft(unsigned short tirePressureStatusFrontLeft);
	void setTirePressureStatusFrontRight(unsigned short tirePressureStatusFrontRight);
	void setTirePressureStatusRearLeft(unsigned short tirePressureStatusRearLeft);
	void setTirePressureStatusRearRight(unsigned short tirePressureStatusRearRight);

	static const VehicleEventType MAINTENANCE;
	static const VehicleEventType MAINTENANCE_ODOMETER;
	static const VehicleEventType MAINTENANCE_TRANSMISSIONOIL;
	static const VehicleEventType MAINTENANCE_TRANSMISSIONOIL_LIFELEVEL;
	static const VehicleEventType MAINTENANCE_TRANSMISSIONOIL_TEMP;
	static const VehicleEventType MAINTENANCE_BRAKEFLUIDLEVEL;
	static const VehicleEventType MAINTENANCE_WASHERFLUIDLEVEL;
	static const VehicleEventType MAINTENANCE_MALFUNCTIONINDICATORLAMP;
	static const VehicleEventType MAINTENANCE_BATTERY;
	static const VehicleEventType MAINTENANCE_BATTERY_VOLTAGE;
	static const VehicleEventType MAINTENANCE_BATTERY_CURRENT;
	static const VehicleEventType MAINTENANCE_TIREPRESSURE;
	static const VehicleEventType MAINTENANCE_TIREPRESSURE_FRONTLEFT;
	static const VehicleEventType MAINTENANCE_TIREPRESSURE_FRONTRIGHT;
	static const VehicleEventType MAINTENANCE_TIREPRESSURE_REARLEFT;
	static const VehicleEventType MAINTENANCE_TIREPRESSURE_REARRIGHT;
	static const VehicleEventType MAINTENANCE_TIREPRESSURESTATUS;
	static const VehicleEventType MAINTENANCE_TIREPRESSURESTATUS_FRONTLEFT;
	static const VehicleEventType MAINTENANCE_TIREPRESSURESTATUS_FRONTRIGHT;
	static const VehicleEventType MAINTENANCE_TIREPRESSURESTATUS_REARLEFT;
	static const VehicleEventType MAINTENANCE_TIREPRESSURESTATUS_REARRIGHT;

	static const unsigned short TIREPRESSURESTATUS_NORMAL = 0;
	static const unsigned short TIREPRESSURESTATUS_LOW = 1;
	static const unsigned short TIREPRESSURESTATUS_HIGH = 2;

private:
	virtual void updateValidity(VehicleEventType type);

	FB::BrowserHostPtr m_host;

	VehicleEventType type;
	unsigned long odometer;
	bool transmissionOilLifeLevel;
	short transmissionOilTemp;
	bool brakeFluidLevel;
	bool washerFluidLevel;
	bool malfunctionIndicatorLamp;
	unsigned short batteryVoltage;
	unsigned short batteryCurrent;
	unsigned short tirePressureFrontLeft;
	unsigned short tirePressureFrontRight;
	unsigned short tirePressureRearLeft;
	unsigned short tirePressureRearRight;
	unsigned short tirePressureStatusFrontLeft;
	unsigned short tirePressureStatusFrontRight;
	unsigned short tirePressureStatusRearLeft;
	unsigned short tirePressureStatusRearRight;
};

#endif // H_MaintenanceEvent
