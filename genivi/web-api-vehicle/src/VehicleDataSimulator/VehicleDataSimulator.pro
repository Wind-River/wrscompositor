# Copyright (C) 2013, LG Electronics
#
# This file is part of GENIVI Project web-api-vehicle.
#
# Contributions are licensed to the GENIVI Alliance under one or more
# Contribution License Agreements.
#
# @copyright
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# @author Chunsung Jung <chunsung.jung@lge.com>
#
# For further information, see http://www.genivi.org/

#-------------------------------------------------
#
# Project created by QtCreator 2012-07-06T13:12:23
#
#-------------------------------------------------

QT       += core gui network

TARGET = VehicleDataSimulator
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp\
        udpclient.cpp\
        udpserver.cpp

HEADERS  += mainwindow.h\
            udpclient.h\
            udpserver.h\
            def-socket.h

FORMS    += mainwindow.ui

OTHER_FILES += README.txt\
               network.cfg
