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
 * @file dbus-receiver.h 
 * For further information, see http://www.genivi.org/
 */

#ifndef H_DBusReceiver
#define H_DBusReceiver

// for std::cout and std::endl
#include <iostream> 
// for sprintf()
#include <cstdio> 
#include <dbus/dbus.h>

// A function called when a signal needs to be handled
// Set with add_signal_filter
typedef void(*SignalHandlingFunction)(DBusMessage* message, 
									  const char* signal_name,
									  int index1,
									  int index2);
// Length of a filter string
const int MAX_FILTER_STR_LEN = 1024; 

class DBusReceiver 
{
	class SignalFilter 
	{
	public:
		const char* path;
		const char* interface;
		const char** signal_list;
		SignalHandlingFunction func;
	};
	
public:
	DBusReceiver();
	~DBusReceiver();

	// Initialize D-Bus with or without a given bus name
	bool init_dbus(); 
	bool init_dbus(const char* bus_name);
	// Initialize a pointer array to contain signal filters 
	// filter_n: the total number of filters to be added
	void init_signal_filter(int filter_n);
	// Add a new signal filter to signal_filters 
	// index: the filter number starting from 0 to (signal_filter_n - 1)
	void add_signal_filter(int index,
						   const char* path,
						   const char* interface,
						   const char* signal_list[],
						   SignalHandlingFunction func);
	// Create and run a signal listener 
	// poll_interval: the time interval between polling [us]
	//                (default: 10 ms)
	void listen_signal();
	void listen_signal(int poll_interval);
	// Stop the signal listener
	void stop_listening();

private:
	static void* start_thread(void* ptr);
	void thread_listen_signal();

	pthread_t m_thread;
	DBusConnection* conn;
	// A pointer array of SignalFilter class
	SignalFilter* signal_filters; 
	// The number of SignalFilters which contains in this class
	int signal_filter_n; 
	// Keep running a signal listener until this value is true
	bool m_loop; 
	// The time interval between polling [us]
	int poll_interval; 
};

#endif // H_DBusReceiver
