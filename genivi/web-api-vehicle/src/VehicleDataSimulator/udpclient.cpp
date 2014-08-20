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
 * @author Chunsung Jung <chunsung.jung@lge.com>
 *
 * @file udpclient.cpp 
 * For further information, see http://www.genivi.org/
 */

#include "udpclient.h"

UdpClient::UdpClient(QString ipAddress, int portNumber)
{
    udpSocket = new QUdpSocket();
    m_ipAddress = QHostAddress(ipAddress);
    m_portNumber = portNumber;
}

int UdpClient::send(QByteArray datagram)
{
    udpSocket->writeDatagram(datagram.data(), datagram.size(), m_ipAddress, m_portNumber);
    return 0;
}
