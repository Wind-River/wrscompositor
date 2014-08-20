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
 * @file DrivingSafetyEvent.h 
 * For further information, see http://www.genivi.org/
 */

#ifndef H_DrivingSafetyEvent
#define H_DrivingSafetyEvent

#include <string>
#include <sstream>
#include "VehicleEvent.h"
#include "TypeDef.h"

class DrivingSafetyEvent : public VehicleEvent
{
public:
	DrivingSafetyEvent(const FB::BrowserHostPtr& host);
	virtual ~DrivingSafetyEvent();

	VehicleEventType getType();
	unsigned short getAntilockBrakingSystem();
	unsigned short getTractionControlSystem();
	unsigned short getElectronicStabilityControl();
	unsigned short getVehicleTopSpeedLimit();
	unsigned short getAirbagStatusDriver();
	unsigned short getAirbagStatusPassenger();
	unsigned short getAirbagStatusSide();
	unsigned short getDoorOpenStatusDriver();
	unsigned short getDoorOpenStatusPassenger();
	unsigned short getDoorOpenStatusRearLeft();
	unsigned short getDoorOpenStatusRearRight();
	unsigned short getDoorOpenStatusTrunk();
	unsigned short getDoorOpenStatusFuelFilterCap();
	unsigned short getDoorOpenStatusHood();
	bool getDoorLockStatusDriver();
	bool getDoorLockStatusPassenger();
	bool getDoorLockStatusRearLeft();
	bool getDoorLockStatusRearRight();
	bool getChildSafetyLock();
	unsigned short getOccupantsStatusDriver();
	unsigned short getOccupantsStatusPassenger();
	unsigned short getOccupantsStatusRearLeft();
	unsigned short getOccupantsStatusRearRight();
	bool getSeatBeltDriver();
	bool getSeatBeltPassenger();
	bool getSeatBeltRearLeft();
	bool getSeatBeltRearRight();
	bool getWindowLockDriver();
	bool getWindowLockPassenger();
	bool getWindowLockRearLeft();
	bool getWindowLockRearRight();
	bool getObstacleDistanceSensorStatus();
	unsigned short getObstacleDistanceFrontCenter();
	unsigned short getObstacleDistanceRearCenter();
	unsigned short getObstacleDistanceFrontLeft();
	unsigned short getObstacleDistanceFrontRight();
	unsigned short getObstacleDistanceMiddleLeft();
	unsigned short getObstacleDistanceMiddleRight();
	unsigned short getObstacleDistanceRearLeft();
	unsigned short getObstacleDistanceRearRight();
	bool getFrontCollisionDetectionStatus();
	unsigned long getFrontCollisionDetectionDistance();
	unsigned long getFrontCollisionDetectionTime();

	void setType(VehicleEventType type);
	void setAntilockBrakingSystem(unsigned short antilockBrakingSystem);
	void setTractionControlSystem(unsigned short tractionControlSystem);
	void setElectronicStabilityControl(unsigned short electronicStabilityControl);
	void setVehicleTopSpeedLimit(unsigned short vehicleTopSpeedLimit);
	void setAirbagStatusDriver(unsigned short airbagStatusDriver);
	void setAirbagStatusPassenger(unsigned short airbagStatusPassenger);
	void setAirbagStatusSide(unsigned short airbagStatusSide);
	void setDoorOpenStatusDriver(unsigned short doorOpenStatusDriver);
	void setDoorOpenStatusPassenger(unsigned short doorOpenStatusPassenger);
	void setDoorOpenStatusRearLeft(unsigned short doorOpenStatusRearLeft);
	void setDoorOpenStatusRearRight(unsigned short doorOpenStatusRearRight);
	void setDoorOpenStatusTrunk(unsigned short doorOpenStatusTrunk);
	void setDoorOpenStatusFuelFilterCap(unsigned short doorOpenStatusFuelFilterCap);
	void setDoorOpenStatusHood(unsigned short doorOpenStatusHood);
	void setDoorLockStatusDriver(bool doorLockStatusDriver);
	void setDoorLockStatusPassenger(bool doorLockStatusPassenger);
	void setDoorLockStatusRearLeft(bool doorLockStatusRearLeft);
	void setDoorLockStatusRearRight(bool doorLockStatusRearRight);
	void setChildSafetyLock(bool childSafetyLock);
	void setOccupantsStatusDriver(unsigned short occupantsStatusDriver);
	void setOccupantsStatusPassenger(unsigned short occupantsStatusPassenger);
	void setOccupantsStatusRearLeft(unsigned short occupantsStatusRearLeft);
	void setOccupantsStatusRearRight(unsigned short occupantsStatusRearRight);
	void setSeatBeltDriver(bool seatBeltDriver);
	void setSeatBeltPassenger(bool seatBeltPassenger);
	void setSeatBeltRearLeft(bool seatBeltRearLeft);
	void setSeatBeltRearRight(bool seatBeltRearRight);
	void setWindowLockDriver(bool windowLockDriver);
	void setWindowLockPassenger(bool windowLockPassenger);
	void setWindowLockRearLeft(bool windowLockRearLeft);
	void setWindowLockRearRight(bool windowLockRearRight);
	void setObstacleDistanceSensorStatus(bool obstacleDistanceSensorStatus);
	void setObstacleDistanceFrontCenter(unsigned short obstacleDistanceFrontCenter);
	void setObstacleDistanceRearCenter(unsigned short obstacleDistanceRearCenter);
	void setObstacleDistanceFrontLeft(unsigned short obstacleDistanceFrontLeft);
	void setObstacleDistanceFrontRight(unsigned short obstacleDistanceFrontRight);
	void setObstacleDistanceMiddleLeft(unsigned short obstacleDistanceMiddleLeft);
	void setObstacleDistanceMiddleRight(unsigned short obstacleDistanceMiddleRight);
	void setObstacleDistanceRearLeft(unsigned short obstacleDistanceRearLeft);
	void setObstacleDistanceRearRight(unsigned short obstacleDistanceRearRight);
	void setFrontCollisionDetectionStatus(bool frontCollisionDetectionStatus);
	void setFrontCollisionDetectionDistance(unsigned long frontCollisionDetectionDistance);
	void setFrontCollisionDetectionTime(unsigned long frontCollisionDetectionTime);

	static const VehicleEventType DRIVINGSAFETY;
	static const VehicleEventType DRIVINGSAFETY_ANTILOCKBRAKINGSYSTEM;
	static const VehicleEventType DRIVINGSAFETY_TRACTIONCONTROLSYSTEM;
	static const VehicleEventType DRIVINGSAFETY_ELECTRONICSTABILITYCONTROL;
	static const VehicleEventType DRIVINGSAFETY_VEHICLETOPSPEEDLIMIT;
	static const VehicleEventType DRIVINGSAFETY_AIRBAGSTATUS;
	static const VehicleEventType DRIVINGSAFETY_AIRBAGSTATUS_DRIVER;
	static const VehicleEventType DRIVINGSAFETY_AIRBAGSTATUS_PASSENGER;
	static const VehicleEventType DRIVINGSAFETY_AIRBAGSTATUS_SIDE;
	static const VehicleEventType DRIVINGSAFETY_DOOROPENSTATUS;
	static const VehicleEventType DRIVINGSAFETY_DOOROPENSTATUS_DRIVER;
	static const VehicleEventType DRIVINGSAFETY_DOOROPENSTATUS_PASSENGER;
	static const VehicleEventType DRIVINGSAFETY_DOOROPENSTATUS_REARLEFT;
	static const VehicleEventType DRIVINGSAFETY_DOOROPENSTATUS_REARRIGHT;
	static const VehicleEventType DRIVINGSAFETY_DOOROPENSTATUS_TRUNK; 
	static const VehicleEventType DRIVINGSAFETY_DOOROPENSTATUS_FUELFILTERCAP;
	static const VehicleEventType DRIVINGSAFETY_DOOROPENSTATUS_HOOD;
	static const VehicleEventType DRIVINGSAFETY_DOORLOCKSTATUS;
	static const VehicleEventType DRIVINGSAFETY_DOORLOCKSTATUS_DRIVER;
	static const VehicleEventType DRIVINGSAFETY_DOORLOCKSTATUS_PASSENGER;
	static const VehicleEventType DRIVINGSAFETY_DOORLOCKSTATUS_REARLEFT;
	static const VehicleEventType DRIVINGSAFETY_DOORLOCKSTATUS_REARRIGHT;
	static const VehicleEventType DRIVINGSAFETY_CHILDSAFETYLOCK;
	static const VehicleEventType DRIVINGSAFETY_OCCUPANTSSTATUS;
	static const VehicleEventType DRIVINGSAFETY_OCCUPANTSSTATUS_DRIVER;
	static const VehicleEventType DRIVINGSAFETY_OCCUPANTSSTATUS_PASSENGER;
	static const VehicleEventType DRIVINGSAFETY_OCCUPANTSSTATUS_REARLEFT;
	static const VehicleEventType DRIVINGSAFETY_OCCUPANTSSTATUS_REARRIGHT;
	static const VehicleEventType DRIVINGSAFETY_SEATBELT;
	static const VehicleEventType DRIVINGSAFETY_SEATBELT_DRIVER;
	static const VehicleEventType DRIVINGSAFETY_SEATBELT_PASSENGER;
	static const VehicleEventType DRIVINGSAFETY_SEATBELT_REARLEFT;
	static const VehicleEventType DRIVINGSAFETY_SEATBELT_REARRIGHT;
	static const VehicleEventType DRIVINGSAFETY_WINDOWLOCK;
	static const VehicleEventType DRIVINGSAFETY_WINDOWLOCK_DRIVER;
	static const VehicleEventType DRIVINGSAFETY_WINDOWLOCK_PASSENGER;
	static const VehicleEventType DRIVINGSAFETY_WINDOWLOCK_REARLEFT;
	static const VehicleEventType DRIVINGSAFETY_WINDOWLOCK_REARRIGHT;
	static const VehicleEventType DRIVINGSAFETY_OBSTACLEDISTANCE;
	static const VehicleEventType DRIVINGSAFETY_OBSTACLEDISTANCE_SENSORSTATUS;
	static const VehicleEventType DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTCENTER;
	static const VehicleEventType DRIVINGSAFETY_OBSTACLEDISTANCE_REARCENTER;
	static const VehicleEventType DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTLEFT;
	static const VehicleEventType DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTRIGHT;
	static const VehicleEventType DRIVINGSAFETY_OBSTACLEDISTANCE_MIDDLELEFT;
	static const VehicleEventType DRIVINGSAFETY_OBSTACLEDISTANCE_MIDDLERIGHT;
	static const VehicleEventType DRIVINGSAFETY_OBSTACLEDISTANCE_REARLEFT;
	static const VehicleEventType DRIVINGSAFETY_OBSTACLEDISTANCE_REARRIGHT;
	static const VehicleEventType DRIVINGSAFETY_FRONTCOLLISIONDETECTION;
	static const VehicleEventType DRIVINGSAFETY_FRONTCOLLISIONDETECTION_STATUS;
	static const VehicleEventType DRIVINGSAFETY_FRONTCOLLISIONDETECTION_DISTANCE;
	static const VehicleEventType DRIVINGSAFETY_FRONTCOLLISIONDETECTION_TIME;

	static const unsigned short ANTILOCKBRAKINGSYSTEM_AVAILABLE = 1;
	static const unsigned short ANTILOCKBRAKINGSYSTEM_IDLE = 2;
	static const unsigned short ANTILOCKBRAKINGSYSTEM_ENGAGED = 3;
		
	static const unsigned short TRACTIONCONTROLSYSTEM_AVAILABLE = 1;
	static const unsigned short TRACTIONCONTROLSYSTEM_IDLE = 2;
	static const unsigned short TRACTIONCONTROLSYSTEM_ENGAGED = 3;
		
	static const unsigned short ELECTRONICSTABILITYCONTROL_AVAILABLE = 1;
	static const unsigned short ELECTRONICSTABILITYCONTROL_IDLE = 2;
	static const unsigned short ELECTRONICSTABILITYCONTROL_ENGAGED = 3;
		
	static const unsigned short AIRBAGSTATUS_ACTIVATE = 1;
	static const unsigned short AIRBAGSTATUS_DEACTIVATE = 2;
	static const unsigned short AIRBAGSTATUS_DEPLOYMENT = 3;
	
	static const unsigned short DOOROPENSTATUS_OPEN = 1;
	static const unsigned short DOOROPENSTATUS_AJAR = 2;
	static const unsigned short DOOROPENSTATUS_CLOSE = 3;
		
	static const unsigned short OCCUPANTSSTATUS_ADULT = 1;
	static const unsigned short OCCUPANTSSTATUS_CHILD = 2;
	static const unsigned short OCCUPANTSSTATUS_VACANT = 3;

private:
	virtual void updateValidity(VehicleEventType type);

	FB::BrowserHostPtr m_host;

	VehicleEventType type;
	unsigned short antilockBrakingSystem;
	unsigned short tractionControlSystem;
	unsigned short electronicStabilityControl;
	unsigned short vehicleTopSpeedLimit;
	unsigned short airbagStatusDriver;
	unsigned short airbagStatusPassenger;
	unsigned short airbagStatusSide;
	unsigned short doorOpenStatusDriver;
	unsigned short doorOpenStatusPassenger;
	unsigned short doorOpenStatusRearLeft;
	unsigned short doorOpenStatusRearRight;
	unsigned short doorOpenStatusTrunk;
	unsigned short doorOpenStatusFuelFilterCap;
	unsigned short doorOpenStatusHood;
	bool doorLockStatusDriver;
	bool doorLockStatusPassenger;
	bool doorLockStatusRearLeft;
	bool doorLockStatusRearRight;
	bool childSafetyLock;
	unsigned short occupantsStatusDriver;
	unsigned short occupantsStatusPassenger;
	unsigned short occupantsStatusRearLeft;
	unsigned short occupantsStatusRearRight;
	bool seatBeltDriver;
	bool seatBeltPassenger;
	bool seatBeltRearLeft;
	bool seatBeltRearRight;
	bool windowLockDriver;
	bool windowLockPassenger;
	bool windowLockRearLeft;
	bool windowLockRearRight;
	bool obstacleDistanceSensorStatus;
	unsigned short obstacleDistanceFrontCenter;
	unsigned short obstacleDistanceRearCenter;
	unsigned short obstacleDistanceFrontLeft;
	unsigned short obstacleDistanceFrontRight;
	unsigned short obstacleDistanceMiddleLeft;
	unsigned short obstacleDistanceMiddleRight;
	unsigned short obstacleDistanceRearLeft;
	unsigned short obstacleDistanceRearRight;
	bool frontCollisionDetectionStatus;
	unsigned long frontCollisionDetectionDistance;
	unsigned long frontCollisionDetectionTime;
};

#endif // H_DrivingSafetyEvent
