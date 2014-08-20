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
 * @file dbus-receiver.cpp 
 * For further information, see http://www.genivi.org/
 */

#include "dbus-receiver.h"

DBusReceiver::DBusReceiver() 
{
	m_loop = false;
	conn = 0;
	signal_filters = 0;
}

DBusReceiver::~DBusReceiver() 
{
	m_loop = false;
	pthread_join(m_thread, 0);

	if (conn) 
	{
		dbus_connection_unref(conn);
	}
	if (signal_filters)
	{
		delete[] signal_filters;
	}
}

bool DBusReceiver::init_dbus() 
{
	DBusError error;

	// Initialize a variable for error
	dbus_error_init(&error); 
	// Get a session from D-Bus
	conn = dbus_bus_get(DBUS_BUS_SESSION, &error); 
	if (!conn) 
	{
		std::cout << "FAIL: dbus_bus_get(), " << error.message << std::endl;
		dbus_error_free(&error);
		return false;
	}

	return true;
}

bool DBusReceiver::init_dbus(const char* bus_name) 
{
	DBusError error;

	dbus_error_init(&error); 
	conn = dbus_bus_get(DBUS_BUS_SESSION, &error);
	if (!conn) 
	{
		std::cout << "FAIL: dbus_bus_get(), " << error.message << std::endl;
		dbus_error_free(&error);
		return false;
	}

	// Register bus_name to D-Bus
	if (dbus_bus_request_name(conn, bus_name, DBUS_NAME_FLAG_REPLACE_EXISTING, &error) 
		!= DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) 
	{
		std::cout << "FAIL: dbus_bus_request_name(), " << error.message << std::endl;
		dbus_error_free(&error);
		return false;
	}

	return true;
}

void DBusReceiver::init_signal_filter(int filter_n) 
{
	// Create a new pointer array to contain filters
	signal_filters = new SignalFilter[filter_n];
	signal_filter_n = filter_n;
}

void DBusReceiver::add_signal_filter(int index, 
									 const char* path,
									 const char* interface,
									 const char* signal_list[],
									 SignalHandlingFunction func) 
{
	signal_filters[index].path = path;
	signal_filters[index].interface = interface;
	signal_filters[index].signal_list = signal_list;
	signal_filters[index].func = func;
}

void DBusReceiver::listen_signal()
{
	listen_signal(10000);
}

void DBusReceiver::listen_signal(int poll_interval)
{
	this->poll_interval = poll_interval;
	m_loop = true;
	pthread_create(&m_thread, 0, &DBusReceiver::start_thread, this);
}

void DBusReceiver::stop_listening()
{
	m_loop = false;
}

void* DBusReceiver::start_thread(void* ptr)
{
	reinterpret_cast<DBusReceiver*>(ptr)->thread_listen_signal();
}

void DBusReceiver::thread_listen_signal() 
{
	DBusMessage* message;
	DBusError error;
	const char* signal_name;
	char signal_filter[MAX_FILTER_STR_LEN];
	struct timespec ts;

	ts.tv_sec = 0;
	ts.tv_nsec = poll_interval*1000;

	for (int iter = 0; iter < signal_filter_n; iter++) 
	{
		// Set a filter string
		sprintf(signal_filter, "type='signal',path='%s',interface='%s'", 
				signal_filters[iter].path, signal_filters[iter].interface);
		// and add the filter to dbus
		dbus_error_init(&error);
		dbus_bus_add_match(conn, signal_filter, &error);
		dbus_connection_flush(conn);
		dbus_error_free(&error);
	}

	while (m_loop)
	{
		dbus_connection_read_write(conn, 0);
		message = dbus_connection_pop_message(conn);
		if (message == NULL)
		{
			nanosleep(&ts, NULL);
			continue;
		}

		for (int iter1 = 0; iter1 < signal_filter_n; iter1++) 
		{
			// Match the input signal to signal names in the signal list
			int iter2 = 0;
			while ((signal_name = signal_filters[iter1].signal_list[iter2]) != NULL) 
			{
				// If matched
				if (dbus_message_is_signal(message, signal_filters[iter1].interface, signal_name)) 
				{
					// Send the signal to the callback function
					signal_filters[iter1].func(message, signal_name, iter1, iter2);
				}
				iter2++;
			}
		}

		dbus_message_unref(message);
	}
}
