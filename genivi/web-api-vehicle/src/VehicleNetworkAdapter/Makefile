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
# @author Joonhyung Kim <joonhg.kim@lge.com>
#
# For further information, see http://www.genivi.org/

TARGET = VehicleNetworkAdapter 

CPP = g++
INC1 = `pkg-config --cflags dbus-1`
LIBS = -ldbus-1

all: $(TARGET)

$(TARGET): main.o dbus-sender.o dbus-receiver.o udp-client.o udp-server.o
	$(CPP) -o $@ main.o dbus-sender.o dbus-receiver.o udp-client.o udp-server.o $(LIBS)

main.o: main.cpp dbus-sender.h dbus-receiver.h
	$(CPP) -c main.cpp $(INC1) $(INC2)

dbus-sender.o: dbus-sender.cpp dbus-sender.h
	$(CPP) -c dbus-sender.cpp $(INC1)

dbus-receiver.o: dbus-receiver.cpp dbus-receiver.h
	$(CPP) -c dbus-receiver.cpp $(INC1)

udp-client.o: udp-client.cpp udp-client.h
	$(CPP) -c udp-client.cpp $(INC1)

udp-server.o: udp-server.cpp udp-server.h
	$(CPP) -c udp-server.cpp $(INC1)

clean:
	rm -rf $(TARGET) main.o dbus-sender.o dbus-receiver.o udp-client.o udp-server.o

new:
	$(MAKE) clean
	$(MAKE)
