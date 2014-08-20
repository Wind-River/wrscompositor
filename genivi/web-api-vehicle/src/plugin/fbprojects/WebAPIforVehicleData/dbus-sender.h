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
 * @file dbus-sender.h 
 * For further information, see http://www.genivi.org/
 */

#ifndef H_DBusSender
#define H_DBusSender

// for std::cout and std::endl
#include <iostream> 
// for va_start(), va_arg(), and va_end()
#include <cstdarg> 
// for dbus_error_init(), dbus_error_free(), dbus_message_new_signal(), 
// dbus_message_append_args(), dbus_connection_send(), dbus_message_unref(),
// and all defined values like DBUS_TYPE_INT32
#include <dbus/dbus.h> 	

class DBusSender {
public:
	DBusSender();
	~DBusSender();

	// Initialize D-Bus
	bool init_dbus();
	// Send a signal to the given path and interface
	// with the signal name and arguments 
	// Use the same method with dbus_message_append_args() to append arguments
	bool send_signal(const char* path,
					 const char* interface,
					 const char* signal_name,
					 int first_arg_type,
					 ...);

private:
	DBusConnection* conn;
};

#endif // H_DBusSender
