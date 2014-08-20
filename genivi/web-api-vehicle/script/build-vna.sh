#!/bin/bash

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

# SRC_DIR: source directory
SRC_DIR=./src/VehicleNetworkAdapter
# BIN_NAME: binary name
BIN_NAME=VehicleNetworkAdapter

usage() {
	echo "Usage: ./script/build-vna.sh [mode]"
	echo
	echo "Mode:"
	echo "  make 		Build the VehicleNetworkAdapter project"
	echo "  install 	Install the binary file ($BIN_NAME) to the binary directory (./bin)"
	echo "  clean 	Clean the VehicleNetworkAdapter project"
	echo "  distclean 	Clean the VehicleNetworkAdapter project and remove the binary file ($BIN_NAME)"
	echo "  new 		Clean and make the VehicleNetworkAdapter project"
	echo "  -h or --help 	Print help (this message)"
	echo
	exit 1
}
error_make() {
	echo "Run make first"
	echo "Usage: ./script/build-vna.sh make"
	echo
	exit 1
}
check_make() {
	if [ ! -f "$BIN_NAME" ]; then
		error_make
	fi
}

if [ $# -ne 1 ]; then
	usage
else
	cd $SRC_DIR
	if [ $1 = -h ]; then
		usage
	elif [ $1 = --help ]; then
		usage
	elif [ $1 = make ]; then
		make
	elif [ $1 = install ]; then
		check_make
		cp -rvf $BIN_NAME ../../bin
	elif [ $1 = clean ]; then
		make clean
	elif [ $1 = distclean ]; then
		make clean
		rm -rvf ../../bin/$BIN_NAME
	elif [ $1 = new ]; then
		make new
	else
		usage
	fi
fi
