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
 * @file dbus-sender.cpp 
 * For further information, see http://www.genivi.org/
 */

#include "dbus-sender.h"

DBusSender::DBusSender() 
{
	conn = 0;
}

DBusSender::~DBusSender() 
{
	if(conn) 
	{
		dbus_connection_unref(conn);
	}
}

bool DBusSender::init_dbus() 
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

bool DBusSender::send_signal(const char* path, 
							 const char* interface, 
							 const char* signal_name,
							 int first_arg_type,
							 ...) 
{
	va_list var_args;
	
	// A pointer for a D-Bus message
	DBusMessage* message; 

	// Set the message as a signal
	message = dbus_message_new_signal(path, interface, signal_name); 
	if (!message) 
	{
		std::cout << "FAIL: dbus_message_new_signal(), cannot allocate memory for the message." << std::endl;
		return false;
	}

	// Append input arguments to message
	va_start(var_args, first_arg_type);
	dbus_message_append_args_valist(message, first_arg_type, var_args);
	va_end(var_args);

	// Send message
	if (!dbus_connection_send(conn, message, NULL)) 
	{
		std::cout << "FAIL: dbus_connection_send(), lack of memory" << std::endl;
		return false;
	}
	dbus_message_unref(message);

	return true;
}
