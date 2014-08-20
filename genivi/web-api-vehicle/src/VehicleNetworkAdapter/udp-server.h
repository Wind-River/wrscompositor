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
 * @file udp-server.h 
 * For further information, see http://www.genivi.org/
 */

#ifndef H_UdpServer
#define H_UdpServer

// for std::cout and std::endl
#include <iostream> 
// for atoi()
#include <cstdlib> 
// for memset()
#include <cstring> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

typedef void(*MessageHandlingFunction)(const char* message, int str_len);
const int BUF_SIZE = 1024;

class UdpServer
{
public:
	UdpServer();
	~UdpServer();

	bool init(int port_num);
	void listen(MessageHandlingFunction func);
	void listen(MessageHandlingFunction, int poll_interval);
	void stop_listening();

private:
	void thread_listen();
	static void* start_thread(void* ptr);

	pthread_t m_thread;
	int serv_sock;
	struct sockaddr_in serv_adr;
	MessageHandlingFunction handler;
	bool m_loop;
	int poll_interval;
};

#endif // H_UdpServer
