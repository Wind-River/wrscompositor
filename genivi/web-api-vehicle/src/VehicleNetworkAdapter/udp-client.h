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
 * @file udp-client.h 
 * For further information, see http://www.genivi.org/
 */

#ifndef H_UdpClient
#define H_UdpClient

// for std::cout and std::endl
#include <iostream> 
// for memset()
#include <cstring> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

class UdpClient
{
public:
	UdpClient();
	~UdpClient();

	bool init(const char* ip_addr, int port_num);
	bool send(const char* message);

private:
	int sock;
	int str_len;
	struct sockaddr_in serv_adr; 
};

#endif // H_UdpClient
