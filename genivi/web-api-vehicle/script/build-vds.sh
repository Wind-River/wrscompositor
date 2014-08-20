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
SRC_DIR=./src/VehicleDataSimulator
# BIN_NAME: binary name
BIN_NAME=VehicleDataSimulator

usage() {
	echo "Usage: ./script/build-vds.sh [mode]"
	echo
	echo "Mode:"
	echo "  make 		Build the VehicleDataSimulator project"
	echo "  install 	Install the binary file ($BIN_NAME) to the binary directory (./bin)"
	echo "  clean 	Clean the VehicleDataSimulator project"
	echo "  distclean 	Clean the VehicleDataSimulator project and remove the binary file ($BIN_NAME)"
	echo "  		and Makefile"
	echo "  new 		Clean and make the VehicleDataSimulator project"
	echo "  -h or --help 	Print help (this message)"
	echo
	exit 1
}
error_makefile() {
	echo "Makefile does not exist"
	echo
	exit 1
}
error_make() {
	echo "Run make first"
	echo "Usage: ./script/build-vds.sh make"
	echo
	exit 1
}
check_makefile() {
	if [ ! -f "Makefile" ]; then
		error_makefile
	fi
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
		qmake
		check_makefile
		make
	elif [ $1 = install ]; then
		check_make
		cp -rvf $BIN_NAME ../../bin
	elif [ $1 = clean ]; then
		check_makefile
		make clean
	elif [ $1 = distclean ]; then
		make clean
		rm -rvf $BIN_NAME
		rm -rvf ../../bin/$BIN_NAME
		rm -rvf Makefile
	elif [ $1 = new ]; then
		check_makefile
		make clean
		make
	else
		usage
	fi
fi
