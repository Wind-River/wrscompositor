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
 * @file udpclient.h
 * For further information, see http://www.genivi.org/
 */

#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QtNetwork>
#include <QByteArray>

class UdpClient
{
public:
    UdpClient(QString ipAddress, int portNumber);
    int send(QByteArray datagram);

private:
    QUdpSocket *udpSocket;
    QHostAddress m_ipAddress;
    int m_portNumber;
};

#endif // UDPCLIENT_H
