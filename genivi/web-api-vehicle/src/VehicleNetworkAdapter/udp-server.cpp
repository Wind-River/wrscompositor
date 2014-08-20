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
 * @file udp-server.cpp 
 * For further information, see http://www.genivi.org/
 */

#include "udp-server.h"

UdpServer::UdpServer()
{
	m_loop = false;
	serv_sock = -1;
}

UdpServer::~UdpServer()
{
	m_loop = false;
	pthread_join(m_thread, 0);

	if (serv_sock != -1)
	{
		::close(serv_sock);
	}
}

bool UdpServer::init(int port_num)
{
	serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (serv_sock == -1)
	{
		std::cout << "ERROR: socket()" << std::endl;

		return false;
	}

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(port_num);

	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
	{
		std::cout << "ERROR: bind()" << std::endl;

		return false;
	}

	return true;
}

void UdpServer::listen(MessageHandlingFunction func)
{
	listen(func, 10000);
}

void UdpServer::listen(MessageHandlingFunction func, int poll_interval)
{
	this->poll_interval = poll_interval;	
	this->handler = func;
	m_loop = true;
	pthread_create(&m_thread, 0, &UdpServer::start_thread, this);
}

void UdpServer::stop_listening()
{
	m_loop = false;
}

void* UdpServer::start_thread(void* ptr)
{
	reinterpret_cast<UdpServer*>(ptr)->thread_listen();
}

void UdpServer::thread_listen()
{
	char message[BUF_SIZE];
	int str_len;
	socklen_t clnt_adr_sz;
	struct sockaddr_in clnt_adr;
	struct timespec ts;

	ts.tv_sec = 0;
	ts.tv_nsec = poll_interval*1000;

	while (m_loop)
	{
		clnt_adr_sz = sizeof(clnt_adr);
		str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		handler(message, str_len);
		
		nanosleep(&ts, NULL);
	}
}
