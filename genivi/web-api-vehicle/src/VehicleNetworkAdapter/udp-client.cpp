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
 * @file udp-client.cpp 
 * For further information, see http://www.genivi.org/
 */

#include "udp-client.h"

UdpClient::UdpClient()
{
	sock = -1;
}

UdpClient::~UdpClient()
{
	if (sock != -1)
	{
		::close(sock);
	}
}

bool UdpClient::init(const char* ip_addr, int port_num)
{
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
	{
		std::cout << "ERROR: socket()" << std::endl;

		return false;
	}

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(ip_addr);
	serv_adr.sin_port = htons(port_num);

	if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
	{
		std::cout << "ERROR: connect()" << std::endl;

		return false;
	}

	return true;
}

bool UdpClient::send(const char* message)
{
	if (sock == -1)
	{
		std::cout << "ERROR: SOCKET NOT ESTABLISHED" << std::endl;

		return false;
	}

	if (write(sock, message, strlen(message)) == -1)
	{
		std::cout << "ERROR: write()" << std::endl;

		return false;
	}

	return true;
}
