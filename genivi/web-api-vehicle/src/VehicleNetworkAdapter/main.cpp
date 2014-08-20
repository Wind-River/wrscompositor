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
 * @file main.cpp
 * For further information, see http://www.genivi.org/
 */

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "dbus-sender.h"
#include "dbus-receiver.h"
#include "udp-client.h"
#include "udp-server.h"
#include "def-dbus.h"
#include "def-socket.h"

char DBUS_PATH[BUF_SIZE];
char DBUS_INTERFACE[BUF_SIZE];
int VNA_PORT;
char VDS_IP[BUF_SIZE];
int VDS_PORT;

const char* config_file = "network.cfg";

const char* signal_list_set_personalization[23] = {
	genivi::wavd::dbus::set::PERSONALIZATION_LANGUAGE,
	genivi::wavd::dbus::set::PERSONALIZATION_MEASUREMENTSYSTEM,
	genivi::wavd::dbus::set::PERSONALIZATION_MIRROR_DRIVER,
	genivi::wavd::dbus::set::PERSONALIZATION_MIRROR_PASSENGER,
	genivi::wavd::dbus::set::PERSONALIZATION_MIRROR_INSIDE,
	genivi::wavd::dbus::set::PERSONALIZATION_STEERINGWHEELPOSITION_SLIDE,
	genivi::wavd::dbus::set::PERSONALIZATION_STEERINGWHEELPOSITION_TILT,
	genivi::wavd::dbus::set::PERSONALIZATION_DRIVINGMODE,
	genivi::wavd::dbus::set::PERSONALIZATION_DRIVERSEATPOSITION_RECLINESEATBACK,
	genivi::wavd::dbus::set::PERSONALIZATION_DRIVERSEATPOSITION_SLIDE,
	genivi::wavd::dbus::set::PERSONALIZATION_DRIVERSEATPOSITION_CUSHIONHEIGHT,
	genivi::wavd::dbus::set::PERSONALIZATION_DRIVERSEATPOSITION_HEADREST,
	genivi::wavd::dbus::set::PERSONALIZATION_DRIVERSEATPOSITION_BACKCUSHION,
	genivi::wavd::dbus::set::PERSONALIZATION_DRIVERSEATPOSITION_SIDECUSHION,
	genivi::wavd::dbus::set::PERSONALIZATION_PASSENGERSEATPOSITION_RECLINESEATBACK,
	genivi::wavd::dbus::set::PERSONALIZATION_PASSENGERSEATPOSITION_SLIDE,
	genivi::wavd::dbus::set::PERSONALIZATION_PASSENGERSEATPOSITION_CUSHIONHEIGHT,
	genivi::wavd::dbus::set::PERSONALIZATION_PASSENGERSEATPOSITION_HEADREST,
	genivi::wavd::dbus::set::PERSONALIZATION_PASSENGERSEATPOSITION_BACKCUSHION,
	genivi::wavd::dbus::set::PERSONALIZATION_PASSENGERSEATPOSITION_SIDECUSHION,
	genivi::wavd::dbus::set::PERSONALIZATION_DASHBOARDILLUMINATION,
	genivi::wavd::dbus::set::PERSONALIZATION_GENERATEDVEHICLESOUNDMODE,
	NULL
};

const char* signal_list_set_driving_safety[10] = {
	genivi::wavd::dbus::set::DRIVINGSAFETY_VEHICLETOPSPEEDLIMIT,
	genivi::wavd::dbus::set::DRIVINGSAFETY_DOORLOCKSTATUS_DRIVER,
	genivi::wavd::dbus::set::DRIVINGSAFETY_DOORLOCKSTATUS_PASSENGER,
	genivi::wavd::dbus::set::DRIVINGSAFETY_DOORLOCKSTATUS_REARLEFT,
	genivi::wavd::dbus::set::DRIVINGSAFETY_DOORLOCKSTATUS_REARRIGHT,
	genivi::wavd::dbus::set::DRIVINGSAFETY_WINDOWLOCK_DRIVER,
	genivi::wavd::dbus::set::DRIVINGSAFETY_WINDOWLOCK_PASSENGER,
	genivi::wavd::dbus::set::DRIVINGSAFETY_WINDOWLOCK_REARLEFT,
	genivi::wavd::dbus::set::DRIVINGSAFETY_WINDOWLOCK_REARRIGHT,
	NULL
};

const char* signal_list_set_parking[2] = {
	genivi::wavd::dbus::set::PARKING_PARKINGLIGHTS,
	NULL
};

const char* signal_list_set_climate_environment[18] = {
	genivi::wavd::dbus::set::CLIMATEENVIRONMENT_WINDSHIELDWIPER,
	genivi::wavd::dbus::set::CLIMATEENVIRONMENT_REARWIPER,
	genivi::wavd::dbus::set::CLIMATEENVIRONMENT_HVACFAN_DIRECTION,
	genivi::wavd::dbus::set::CLIMATEENVIRONMENT_HVACFAN_SPEED,
	genivi::wavd::dbus::set::CLIMATEENVIRONMENT_HVACFAN_TARGETTEMP,
	genivi::wavd::dbus::set::CLIMATEENVIRONMENT_AIRCONDITIONING,
	genivi::wavd::dbus::set::CLIMATEENVIRONMENT_AIRRECIRCULATION,
	genivi::wavd::dbus::set::CLIMATEENVIRONMENT_HEATER,
	genivi::wavd::dbus::set::CLIMATEENVIRONMENT_DEFROST_WINDSHIELD,
	genivi::wavd::dbus::set::CLIMATEENVIRONMENT_DEFROST_REARWINDOW,
	genivi::wavd::dbus::set::CLIMATEENVIRONMENT_DEFROST_SIDEMIRRORS,
	genivi::wavd::dbus::set::CLIMATEENVIRONMENT_STEERINGWHEELHEATER,
	genivi::wavd::dbus::set::CLIMATEENVIRONMENT_SEATHEATER,
	genivi::wavd::dbus::set::CLIMATEENVIRONMENT_SEATCOOLER,
	genivi::wavd::dbus::set::CLIMATEENVIRONMENT_SUNROOF_OPENNESS,
	genivi::wavd::dbus::set::CLIMATEENVIRONMENT_SUNROOF_TILT,
	genivi::wavd::dbus::set::CLIMATEENVIRONMENT_CONVERTIBLEROOF,
	NULL
};

bool get_config(const char* filename);
void run_dbus_receiver();
void run_udp_server();
void process_signal(DBusMessage* message, const char* signal_name, int index1, int index2);
void process_message(const char* message, int length);

DBusSender dbus_sender;
DBusReceiver dbus_receiver;
UdpClient udp_client;
UdpServer udp_server;

int main(int argc, char *argv[])
{
	std::string config_path = argv[0];
	config_path = config_path.substr(0, config_path.find_last_of("/") + 1).append(config_file);

	if (!get_config(config_path.c_str()))
	{
		return 1;
	}
	else
	{
		std::cout << " [OK] Read network configurations" << std::endl;
	}

	if (!dbus_sender.init_dbus())
	{
		return 1;
	}
	else
	{
		std::cout << " [OK] Initialize a D-Bus client" << std::endl;
	}

	if (!udp_client.init(VDS_IP, VDS_PORT))
	{
		return 1;
	}
	else
	{
		std::cout << " [OK] Initialize a UDP socket client" << std::endl;
	}

	run_dbus_receiver();
	run_udp_server();

	std::cout << "========== Vehicle Network Adapter is running ==========" << std::endl;

	while(true) {
		sleep(1);
	}

	return 0;
}

bool get_config(const char* filename)
{
	std::string str;
	std::ifstream fin(filename);

	if (fin.is_open())
	{
		while (fin.good())
		{
			getline(fin, str);
			if (str.compare(0, 10, "DBUS_PATH=") == 0)
			{
				int length = (str.substr(str.find("=") + 1)).copy(DBUS_PATH, BUF_SIZE);
				DBUS_PATH[length] = '\0';
			} 
			else if (str.compare(0, 15, "DBUS_INTERFACE=") == 0)
			{
				int length = (str.substr(str.find("=") + 1)).copy(DBUS_INTERFACE, BUF_SIZE);
				DBUS_INTERFACE[length] = '\0';
			}
			else if (str.compare(0, 9, "VNA_PORT=") == 0)
			{
				std::istringstream(str.substr(str.find("=") + 1)) >> VNA_PORT;
			}
			else if (str.compare(0, 7, "VDS_IP=") == 0)
			{
				int length = (str.substr(str.find("=") + 1)).copy(VDS_IP, BUF_SIZE);
				VDS_IP[length] = '\0';
			}
			else if (str.compare(0, 9, "VDS_PORT=") == 0)
			{
				std::istringstream(str.substr(str.find("=") + 1)) >> VDS_PORT;
			}
		}
		fin.close();
	}
	else
	{
		std::cout << " [ERROR] Cannot open a network configuration file (network.cfg)" << std::endl;
		return false;
	}

	return true;
}

void run_dbus_receiver()
{
	dbus_receiver.init_dbus();
	dbus_receiver.init_signal_filter(4);
	dbus_receiver.add_signal_filter(0, DBUS_PATH, DBUS_INTERFACE, signal_list_set_personalization, process_signal);
	dbus_receiver.add_signal_filter(1, DBUS_PATH, DBUS_INTERFACE, signal_list_set_driving_safety, process_signal);
	dbus_receiver.add_signal_filter(2, DBUS_PATH, DBUS_INTERFACE, signal_list_set_parking, process_signal);
	dbus_receiver.add_signal_filter(3, DBUS_PATH, DBUS_INTERFACE, signal_list_set_climate_environment, process_signal);
	dbus_receiver.listen_signal();
}

void run_udp_server()
{
	udp_server.init(VNA_PORT);
	udp_server.listen(process_message);
}

void process_signal(DBusMessage* message, const char* signal_name, int index1, int index2)
{
	DBusError error;
	dbus_error_init(&error);
	char* data = new char[BUF_SIZE];

	switch (index1)
	{
	case 0:
		{
			switch (index2)
			{
			case 0:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::PERSONALIZATION_LANGUAGE, value);
					break;
				}
			case 1:
				{
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%d", genivi::wavd::socket::PERSONALIZATION_MEASUREMENTSYSTEM, value);
					break;
				}
			case 2:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::PERSONALIZATION_MIRROR_DRIVER, value);
					break;
				}
			case 3:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::PERSONALIZATION_MIRROR_PASSENGER, value);
					break;
				}
			case 4:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::PERSONALIZATION_MIRROR_INSIDE, value);
					break;
				}
			case 5:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::PERSONALIZATION_STEERINGWHEELPOSITION_SLIDE, value);
					break;
				}
			case 6:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::PERSONALIZATION_STEERINGWHEELPOSITION_TILT, value);
					break;
				}
			case 7:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::PERSONALIZATION_DRIVINGMODE, value);
					break;
				}
			case 8:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::PERSONALIZATION_DRIVERSEATPOSITION_RECLINESEATBACK, value);
					break;
				}
			case 9:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::PERSONALIZATION_DRIVERSEATPOSITION_SLIDE, value);
					break;
				}
			case 10:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::PERSONALIZATION_DRIVERSEATPOSITION_CUSHIONHEIGHT, value);
					break;
				}
			case 11:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::PERSONALIZATION_DRIVERSEATPOSITION_HEADREST, value);
					break;
				}
			case 12:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::PERSONALIZATION_DRIVERSEATPOSITION_BACKCUSHION, value);
					break;
				}
			case 13:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::PERSONALIZATION_DRIVERSEATPOSITION_SIDECUSHION, value);
					break;
				}
			case 14:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::PERSONALIZATION_PASSENGERSEATPOSITION_RECLINESEATBACK, value);
					break;
				}
			case 15:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::PERSONALIZATION_PASSENGERSEATPOSITION_SLIDE, value);
					break;
				}
			case 16:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::PERSONALIZATION_PASSENGERSEATPOSITION_CUSHIONHEIGHT, value);
					break;
				}
			case 17:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::PERSONALIZATION_PASSENGERSEATPOSITION_HEADREST, value);
					break;
				}
			case 18:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::PERSONALIZATION_PASSENGERSEATPOSITION_BACKCUSHION, value);
					break;
				}
			case 19:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::PERSONALIZATION_PASSENGERSEATPOSITION_SIDECUSHION, value);
					break;
				}
			case 20:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::PERSONALIZATION_DASHBOARDILLUMINATION, value);
					break;
				}
			case 21:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::PERSONALIZATION_GENERATEDVEHICLESOUNDMODE, value);
					break;
				}
			}
		break;	
		}
	case 1:
		{
			switch (index2)
			{
			case 0:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::DRIVINGSAFETY_VEHICLETOPSPEEDLIMIT, value);
					break;
				}
			case 1:
				{
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_DOORLOCKSTATUS_DRIVER, value);
					break;
				}
			case 2:
				{
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_DOORLOCKSTATUS_PASSENGER, value);
					break;
				}
			case 3:
				{
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_DOORLOCKSTATUS_REARLEFT, value);
					break;
				}
			case 4:
				{
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_DOORLOCKSTATUS_REARRIGHT, value);
					break;
				}
			case 5:
				{
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_WINDOWLOCK_DRIVER, value);
					break;
				}
			case 6:
				{
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_WINDOWLOCK_PASSENGER, value);
					break;
				}
			case 7:
				{
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_WINDOWLOCK_REARLEFT, value);
					break;
				}
			case 8:
				{
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_WINDOWLOCK_REARRIGHT, value);
					break;
				}
			}
		break;	
		}
	case 2:
		{
			switch (index2)
			{
			case 0:
				{
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%d", genivi::wavd::socket::PARKING_PARKINGLIGHTS, value);
					break;
				}
			}
		break;	
		}
	case 3:
		{
			switch (index2)
			{
			case 0:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::CLIMATEENVIRONMENT_WINDSHIELDWIPER, value);
					break;
				}
			case 1:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::CLIMATEENVIRONMENT_REARWIPER, value);
					break;
				}
			case 2:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::CLIMATEENVIRONMENT_HVACFAN_DIRECTION, value);
					break;
				}
			case 3:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::CLIMATEENVIRONMENT_HVACFAN_SPEED, value);
					break;
				}
			case 4:
				{
					short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_INT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_HVACFAN_TARGETTEMP, value);
					break;
				}
			case 5:
				{
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_AIRCONDITIONING, value);
					break;
				}
			case 6:
				{
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_AIRRECIRCULATION, value);
					break;
				}
			case 7:
				{
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_HEATER, value);
					break;
				}
			case 8:
				{
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_DEFROST_WINDSHIELD, value);
					break;
				}
			case 9:
				{
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_DEFROST_REARWINDOW, value);
					break;
				}
			case 10:
				{
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_DEFROST_SIDEMIRRORS, value);
					break;
				}
			case 11:
				{
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_STEERINGWHEELHEATER, value);
					break;
				}
			case 12:
				{
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_SEATHEATER, value);
					break;
				}
			case 13:
				{
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_SEATCOOLER, value);
					break;
				}
			case 14:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::CLIMATEENVIRONMENT_SUNROOF_OPENNESS, value);
					break;
				}
			case 15:
				{
					unsigned short value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%u", genivi::wavd::socket::CLIMATEENVIRONMENT_SUNROOF_TILT, value);
					break;
				}
			case 16:
				{
					bool value;
					dbus_message_get_args(message, &error, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
					sprintf(data, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_CONVERTIBLEROOF, value);
					break;
				}
			}
		break;	
		}
	}
	udp_client.send(data);

	if (data)
	{
		delete [] data;
	}
}

void process_message(const char* message, int length)
{
	int id;
	std::string raw_value = message;
	std::istringstream(raw_value.substr(0, 3)) >> id;

	if (id == genivi::wavd::socket::VEHICLEINFO_WMI)
	{
		std::string valueStr = raw_value.substr(3, length - 3);
		const char* value = valueStr.c_str();
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::VEHICLEINFO_WMI, DBUS_TYPE_STRING, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::VEHICLEINFO_VIN)
	{
		std::string valueStr = raw_value.substr(3, length - 3);
		const char* value = valueStr.c_str();
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::VEHICLEINFO_VIN, DBUS_TYPE_STRING, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::VEHICLEINFO_VEHICLETYPE)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::VEHICLEINFO_VEHICLETYPE, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::VEHICLEINFO_DOORTYPE_1STROW)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::VEHICLEINFO_DOORTYPE_1STROW, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::VEHICLEINFO_DOORTYPE_2NDROW)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::VEHICLEINFO_DOORTYPE_2NDROW, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::VEHICLEINFO_DOORTYPE_3RDROW)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::VEHICLEINFO_DOORTYPE_3RDROW, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::VEHICLEINFO_FUELTYPE)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::VEHICLEINFO_FUELTYPE, DBUS_TYPE_BYTE, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::VEHICLEINFO_TRANSMISSIONGEARTYPE)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::VEHICLEINFO_TRANSMISSIONGEARTYPE, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::VEHICLEINFO_WHEELINFO_RADIUS)
	{
		double value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::VEHICLEINFO_WHEELINFO_RADIUS, DBUS_TYPE_DOUBLE, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::VEHICLEINFO_WHEELINFO_TRACK)
	{
		double value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::VEHICLEINFO_WHEELINFO_TRACK, DBUS_TYPE_DOUBLE, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_VEHICLEPOWERMODE)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_VEHICLEPOWERMODE, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_SPEEDOMETER)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_SPEEDOMETER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_ENGINESPEED)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_ENGINESPEED, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_TRIPMETER_1_MILEAGE)
	{
		unsigned long value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_TRIPMETER_1_MILEAGE, DBUS_TYPE_UINT64, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_TRIPMETER_2_MILEAGE)
	{
		unsigned long value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_TRIPMETER_2_MILEAGE, DBUS_TYPE_UINT64, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_TRIPMETER_1_AVERAGESPEED)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_TRIPMETER_1_AVERAGESPEED, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_TRIPMETER_2_AVERAGESPEED)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_TRIPMETER_2_AVERAGESPEED, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_TRIPMETER_1_FUELCONSUMPTION)
	{
		unsigned long value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_TRIPMETER_1_FUELCONSUMPTION, DBUS_TYPE_UINT64, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_TRIPMETER_2_FUELCONSUMPTION)
	{
		unsigned long value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_TRIPMETER_2_FUELCONSUMPTION, DBUS_TYPE_UINT64, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_TRANSMISSIONGEARSTATUS)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_TRANSMISSIONGEARSTATUS, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_CRUISECONTROL_STATUS)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_CRUISECONTROL_STATUS, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_CRUISECONTROL_SPEED)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_CRUISECONTROL_SPEED, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_WHEELBRAKE)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_WHEELBRAKE, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_HEAD)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_LIGHTSSTATUS_HEAD, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_HIGHBEAM)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_LIGHTSSTATUS_HIGHBEAM, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_TURNLEFT)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_LIGHTSSTATUS_TURNLEFT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_TURNRIGHT)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_LIGHTSSTATUS_TURNRIGHT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_BRAKE)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_LIGHTSSTATUS_BRAKE, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_FOGFRONT)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_LIGHTSSTATUS_FOGFRONT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_FOGREAR)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_LIGHTSSTATUS_FOGREAR, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_HAZARD)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_LIGHTSSTATUS_HAZARD, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_PARKING)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_LIGHTSSTATUS_PARKING, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_INTERIORLIGHTSSTATUS_DRIVER)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_INTERIORLIGHTSSTATUS_DRIVER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_INTERIORLIGHTSSTATUS_PASSENGER)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_INTERIORLIGHTSSTATUS_PASSENGER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_INTERIORLIGHTSSTATUS_CENTER)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_INTERIORLIGHTSSTATUS_CENTER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_AUTOMATICHEADLIGHTS)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_AUTOMATICHEADLIGHTS, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_DYNAMICHIGHBEAM)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_DYNAMICHIGHBEAM, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_HORN)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_HORN, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_CHIME)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_CHIME, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_FUEL)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_FUEL, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_ESTIMATEDRANGE)
	{
		unsigned long value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_ESTIMATEDRANGE, DBUS_TYPE_UINT64, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_ENGINEOIL_REMAINING)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_ENGINEOIL_REMAINING, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_ENGINEOIL_CHANGE)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_ENGINEOIL_CHANGE, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_ENGINEOIL_TEMP)
	{
		short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_ENGINEOIL_TEMP, DBUS_TYPE_INT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_ENGINECOOLANT_LEVEL)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_ENGINECOOLANT_LEVEL, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_ENGINECOOLANT_TEMP)
	{
		short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_ENGINECOOLANT_TEMP, DBUS_TYPE_INT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::RUNNINGSTATUS_STEERINGWHEELANGLE)
	{
		short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::RUNNINGSTATUS_STEERINGWHEELANGLE, DBUS_TYPE_INT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::MAINTENANCE_ODOMETER)
	{
		unsigned long value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::MAINTENANCE_ODOMETER, DBUS_TYPE_UINT64, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::MAINTENANCE_TRANSMISSIONOIL_LIFELEVEL)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::MAINTENANCE_TRANSMISSIONOIL_LIFELEVEL, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::MAINTENANCE_TRANSMISSIONOIL_TEMP)
	{
		short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::MAINTENANCE_TRANSMISSIONOIL_TEMP, DBUS_TYPE_INT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::MAINTENANCE_BRAKEFLUIDLEVEL)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::MAINTENANCE_BRAKEFLUIDLEVEL, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::MAINTENANCE_WASHERFLUIDLEVEL)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::MAINTENANCE_WASHERFLUIDLEVEL, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::MAINTENANCE_MALFUNCTIONINDICATORLAMP)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::MAINTENANCE_MALFUNCTIONINDICATORLAMP, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::MAINTENANCE_BATTERY_VOLTAGE)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::MAINTENANCE_BATTERY_VOLTAGE, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::MAINTENANCE_BATTERY_CURRENT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::MAINTENANCE_BATTERY_CURRENT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::MAINTENANCE_TIREPRESSURE_FRONTLEFT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::MAINTENANCE_TIREPRESSURE_FRONTLEFT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::MAINTENANCE_TIREPRESSURE_FRONTRIGHT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::MAINTENANCE_TIREPRESSURE_FRONTRIGHT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::MAINTENANCE_TIREPRESSURE_REARLEFT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::MAINTENANCE_TIREPRESSURE_REARLEFT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::MAINTENANCE_TIREPRESSURE_REARRIGHT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::MAINTENANCE_TIREPRESSURE_REARRIGHT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::MAINTENANCE_TIREPRESSURESTATUS_FRONTLEFT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::MAINTENANCE_TIREPRESSURESTATUS_FRONTLEFT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::MAINTENANCE_TIREPRESSURESTATUS_FRONTRIGHT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::MAINTENANCE_TIREPRESSURESTATUS_FRONTRIGHT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::MAINTENANCE_TIREPRESSURESTATUS_REARLEFT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::MAINTENANCE_TIREPRESSURESTATUS_REARLEFT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::MAINTENANCE_TIREPRESSURESTATUS_REARRIGHT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::MAINTENANCE_TIREPRESSURESTATUS_REARRIGHT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_KEYID)
	{
		std::string valueStr = raw_value.substr(3, length - 3);
		const char* value = valueStr.c_str();
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_KEYID, DBUS_TYPE_STRING, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_LANGUAGE)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_LANGUAGE, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_MEASUREMENTSYSTEM)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_MEASUREMENTSYSTEM, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_FUEL)
	{
		std::string valueStr = raw_value.substr(3, length - 3);
		const char* value = valueStr.c_str();
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_FUEL, DBUS_TYPE_STRING, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_DISTANCE)
	{
		std::string valueStr = raw_value.substr(3, length - 3);
		const char* value = valueStr.c_str();
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_DISTANCE, DBUS_TYPE_STRING, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_SPEED)
	{
		std::string valueStr = raw_value.substr(3, length - 3);
		const char* value = valueStr.c_str();
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_SPEED, DBUS_TYPE_STRING, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_CONSUMPTION)
	{
		std::string valueStr = raw_value.substr(3, length - 3);
		const char* value = valueStr.c_str();
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_CONSUMPTION, DBUS_TYPE_STRING, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_MIRROR_DRIVER)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_MIRROR_DRIVER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_MIRROR_PASSENGER)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_MIRROR_PASSENGER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_MIRROR_INSIDE)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_MIRROR_INSIDE, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_STEERINGWHEELPOSITION_SLIDE)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_STEERINGWHEELPOSITION_SLIDE, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_STEERINGWHEELPOSITION_TILT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_STEERINGWHEELPOSITION_TILT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_DRIVINGMODE)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_DRIVINGMODE, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_DRIVERSEATPOSITION_RECLINESEATBACK)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_DRIVERSEATPOSITION_RECLINESEATBACK, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_DRIVERSEATPOSITION_SLIDE)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_DRIVERSEATPOSITION_SLIDE, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_DRIVERSEATPOSITION_CUSHIONHEIGHT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_DRIVERSEATPOSITION_CUSHIONHEIGHT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_DRIVERSEATPOSITION_HEADREST)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_DRIVERSEATPOSITION_HEADREST, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_DRIVERSEATPOSITION_BACKCUSHION)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_DRIVERSEATPOSITION_BACKCUSHION, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_DRIVERSEATPOSITION_SIDECUSHION)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_DRIVERSEATPOSITION_SIDECUSHION, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_PASSENGERSEATPOSITION_RECLINESEATBACK)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_PASSENGERSEATPOSITION_RECLINESEATBACK, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_PASSENGERSEATPOSITION_SLIDE)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_PASSENGERSEATPOSITION_SLIDE, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_PASSENGERSEATPOSITION_CUSHIONHEIGHT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_PASSENGERSEATPOSITION_CUSHIONHEIGHT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_PASSENGERSEATPOSITION_HEADREST)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_PASSENGERSEATPOSITION_HEADREST, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_PASSENGERSEATPOSITION_BACKCUSHION)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_PASSENGERSEATPOSITION_BACKCUSHION, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_PASSENGERSEATPOSITION_SIDECUSHION)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_PASSENGERSEATPOSITION_SIDECUSHION, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_DASHBOARDILLUMINATION)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_DASHBOARDILLUMINATION, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PERSONALIZATION_GENERATEDVEHICLESOUNDMODE)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PERSONALIZATION_GENERATEDVEHICLESOUNDMODE, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_ANTILOCKBRAKINGSYSTEM)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_ANTILOCKBRAKINGSYSTEM, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_TRACTIONCONTROLSYSTEM)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_TRACTIONCONTROLSYSTEM, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_ELECTRONICSTABILITYCONTROL)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_ELECTRONICSTABILITYCONTROL, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_VEHICLETOPSPEEDLIMIT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_VEHICLETOPSPEEDLIMIT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_AIRBAGSTATUS_DRIVER)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_AIRBAGSTATUS_DRIVER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_AIRBAGSTATUS_PASSENGER)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_AIRBAGSTATUS_PASSENGER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_AIRBAGSTATUS_SIDE)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_AIRBAGSTATUS_SIDE, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_DOOROPENSTATUS_DRIVER)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_DOOROPENSTATUS_DRIVER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_DOOROPENSTATUS_PASSENGER)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_DOOROPENSTATUS_PASSENGER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_DOOROPENSTATUS_REARLEFT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_DOOROPENSTATUS_REARLEFT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_DOOROPENSTATUS_REARRIGHT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_DOOROPENSTATUS_REARRIGHT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_DOOROPENSTATUS_TRUNK)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_DOOROPENSTATUS_TRUNK, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_DOOROPENSTATUS_FUELFILTERCAP)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_DOOROPENSTATUS_FUELFILTERCAP, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_DOOROPENSTATUS_HOOD)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_DOOROPENSTATUS_HOOD, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_DOORLOCKSTATUS_DRIVER)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_DOORLOCKSTATUS_DRIVER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_DOORLOCKSTATUS_PASSENGER)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_DOORLOCKSTATUS_PASSENGER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_DOORLOCKSTATUS_REARLEFT)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_DOORLOCKSTATUS_REARLEFT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_DOORLOCKSTATUS_REARRIGHT)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_DOORLOCKSTATUS_REARRIGHT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_CHILDSAFETYLOCK)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_CHILDSAFETYLOCK, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_OCCUPANTSSTATUS_DRIVER)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_OCCUPANTSSTATUS_DRIVER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_OCCUPANTSSTATUS_PASSENGER)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_OCCUPANTSSTATUS_PASSENGER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_OCCUPANTSSTATUS_REARLEFT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_OCCUPANTSSTATUS_REARLEFT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_OCCUPANTSSTATUS_REARRIGHT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_OCCUPANTSSTATUS_REARRIGHT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_SEATBELT_DRIVER)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_SEATBELT_DRIVER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_SEATBELT_PASSENGER)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_SEATBELT_PASSENGER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_SEATBELT_REARLEFT)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_SEATBELT_REARLEFT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_SEATBELT_REARRIGHT)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_SEATBELT_REARRIGHT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_WINDOWLOCK_DRIVER)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_WINDOWLOCK_DRIVER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_WINDOWLOCK_PASSENGER)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_WINDOWLOCK_PASSENGER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_WINDOWLOCK_REARLEFT)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_WINDOWLOCK_REARLEFT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_WINDOWLOCK_REARRIGHT)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_WINDOWLOCK_REARRIGHT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_SENSORSTATUS)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_OBSTACLEDISTANCE_SENSORSTATUS, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTCENTER)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTCENTER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_REARCENTER)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_OBSTACLEDISTANCE_REARCENTER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTLEFT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTLEFT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTRIGHT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTRIGHT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_MIDDLELEFT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_OBSTACLEDISTANCE_MIDDLELEFT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_MIDDLERIGHT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_OBSTACLEDISTANCE_MIDDLERIGHT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_REARLEFT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_OBSTACLEDISTANCE_REARLEFT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_REARRIGHT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_OBSTACLEDISTANCE_REARRIGHT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_FRONTCOLLISIONDETECTION_STATUS)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_FRONTCOLLISIONDETECTION_STATUS, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_FRONTCOLLISIONDETECTION_DISTANCE)
	{
		unsigned long value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_FRONTCOLLISIONDETECTION_DISTANCE, DBUS_TYPE_UINT64, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::DRIVINGSAFETY_FRONTCOLLISIONDETECTION_TIME)
	{
		unsigned long value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::DRIVINGSAFETY_FRONTCOLLISIONDETECTION_TIME, DBUS_TYPE_UINT64, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::VISIONSYSTEM_LANEDEPARTUREDETECTIONSTATUS)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::VISIONSYSTEM_LANEDEPARTUREDETECTIONSTATUS, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::VISIONSYSTEM_LANEDEPARTED)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::VISIONSYSTEM_LANEDEPARTED, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PARKING_SECURITYALERT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PARKING_SECURITYALERT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PARKING_PARKINGBRAKE)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PARKING_PARKINGBRAKE, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::PARKING_PARKINGLIGHTS)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::PARKING_PARKINGLIGHTS, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::CLIMATEENVIRONMENT_INTERIORTEMP)
	{
		short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::CLIMATEENVIRONMENT_INTERIORTEMP, DBUS_TYPE_INT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::CLIMATEENVIRONMENT_EXTERIORTEMP)
	{
		short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::CLIMATEENVIRONMENT_EXTERIORTEMP, DBUS_TYPE_INT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::CLIMATEENVIRONMENT_EXTERIORBRIGHTNESS)
	{
		unsigned long value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::CLIMATEENVIRONMENT_EXTERIORBRIGHTNESS, DBUS_TYPE_UINT64, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::CLIMATEENVIRONMENT_RAINSENSOR)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::CLIMATEENVIRONMENT_RAINSENSOR, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::CLIMATEENVIRONMENT_WINDSHIELDWIPER)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::CLIMATEENVIRONMENT_WINDSHIELDWIPER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::CLIMATEENVIRONMENT_REARWIPER)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::CLIMATEENVIRONMENT_REARWIPER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::CLIMATEENVIRONMENT_HVACFAN_DIRECTION)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::CLIMATEENVIRONMENT_HVACFAN_DIRECTION, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::CLIMATEENVIRONMENT_HVACFAN_SPEED)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::CLIMATEENVIRONMENT_HVACFAN_SPEED, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::CLIMATEENVIRONMENT_HVACFAN_TARGETTEMP)
	{
		short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::CLIMATEENVIRONMENT_HVACFAN_TARGETTEMP, DBUS_TYPE_INT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::CLIMATEENVIRONMENT_AIRCONDITIONING)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::CLIMATEENVIRONMENT_AIRCONDITIONING, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::CLIMATEENVIRONMENT_AIRRECIRCULATION)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::CLIMATEENVIRONMENT_AIRRECIRCULATION, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::CLIMATEENVIRONMENT_HEATER)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::CLIMATEENVIRONMENT_HEATER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::CLIMATEENVIRONMENT_DEFROST_WINDSHIELD)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::CLIMATEENVIRONMENT_DEFROST_WINDSHIELD, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::CLIMATEENVIRONMENT_DEFROST_REARWINDOW)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::CLIMATEENVIRONMENT_DEFROST_REARWINDOW, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::CLIMATEENVIRONMENT_DEFROST_SIDEMIRRORS)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::CLIMATEENVIRONMENT_DEFROST_SIDEMIRRORS, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::CLIMATEENVIRONMENT_STEERINGWHEELHEATER)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::CLIMATEENVIRONMENT_STEERINGWHEELHEATER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::CLIMATEENVIRONMENT_SEATHEATER)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::CLIMATEENVIRONMENT_SEATHEATER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::CLIMATEENVIRONMENT_SEATCOOLER)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::CLIMATEENVIRONMENT_SEATCOOLER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::CLIMATEENVIRONMENT_WINDOW_DRIVER)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::CLIMATEENVIRONMENT_WINDOW_DRIVER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::CLIMATEENVIRONMENT_WINDOW_PASSENGER)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::CLIMATEENVIRONMENT_WINDOW_PASSENGER, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::CLIMATEENVIRONMENT_WINDOW_REARLEFT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::CLIMATEENVIRONMENT_WINDOW_REARLEFT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::CLIMATEENVIRONMENT_WINDOW_REARRIGHT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::CLIMATEENVIRONMENT_WINDOW_REARRIGHT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::CLIMATEENVIRONMENT_SUNROOF_OPENNESS)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::CLIMATEENVIRONMENT_SUNROOF_OPENNESS, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::CLIMATEENVIRONMENT_SUNROOF_TILT)
	{
		unsigned short value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::CLIMATEENVIRONMENT_SUNROOF_TILT, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
	else if (id == genivi::wavd::socket::CLIMATEENVIRONMENT_CONVERTIBLEROOF)
	{
		bool value;
		std::istringstream(raw_value.substr(3, length - 3)) >> value;
		dbus_sender.send_signal(DBUS_PATH, DBUS_INTERFACE, genivi::wavd::dbus::get::CLIMATEENVIRONMENT_CONVERTIBLEROOF, DBUS_TYPE_UINT16, &value, DBUS_TYPE_INVALID);
	}
}
