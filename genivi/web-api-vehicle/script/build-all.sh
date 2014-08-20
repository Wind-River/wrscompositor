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

usage() {
	echo "Usage: ./script/build-all.sh 		Build and install all projects (plugin, VehicleNetworkAdapter,"
	echo "  					VehicleDataSimulator)"
	echo "   or: ./script/build-all.sh [mode]"
	echo
	echo "mode:"
	echo "  make 		Build all projects"
	echo "  install 	Install all projects"
	echo "  clean 	Clean all projects"
	echo "  distclean 	Clean all projects, remove all files created during building process,"
	echo " 		and remove all binaries"
	echo "  new 		Clean and make all projects"
	echo "  -h or --help 	Print help (this message)"
	echo
}

if [ $# -eq 1 ]; then
	if [ $1 = -h ]; then
		usage
	elif [ $1 = --help ]; then
		usage
	elif [ $1 = make ]; then
		./script/build-plugin.sh prepmake
		./script/build-plugin.sh make
		./script/build-vna.sh make
		./script/build-vds.sh make
	elif [ $1 = install ]; then
		./script/build-plugin.sh install
		./script/build-vna.sh install
		./script/build-vds.sh install
	elif [ $1 = clean ]; then
		./script/build-plugin.sh clean
		./script/build-vna.sh clean
		./script/build-vds.sh clean
	elif [ $1 = distclean ]; then
		./script/build-plugin.sh distclean
		./script/build-vna.sh distclean
		./script/build-vds.sh distclean
	elif [ $1 = new ]; then
		./script/build-plugin.sh new
		./script/build-vna.sh new
		./script/build-vds.sh new 
	else
		usage
	fi
elif [ $# -eq 0 ]; then
	./script/build-plugin.sh prepmake 
	./script/build-plugin.sh make
	./script/build-plugin.sh install

	./script/build-vna.sh make
	./script/build-vna.sh install

	./script/build-vds.sh make
	./script/build-vds.sh install
else
	usage
fi
