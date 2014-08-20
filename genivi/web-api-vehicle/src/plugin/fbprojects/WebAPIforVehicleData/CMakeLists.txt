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

cmake_minimum_required (VERSION 2.6)
set (CMAKE_BACKWARDS_COMPATIBILITY 2.6)

Project(${PLUGIN_NAME})

file (GLOB GENERAL RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    [^.]*.cpp
    [^.]*.h
    [^.]*.cmake
    )

include_directories(${PLUGIN_INCLUDE_DIRS})
pkg_check_modules(dbus REQUIRED dbus-1)
include_directories(${dbus_INCLUDE_DIRS})

# Generated files are stored in ${GENERATED} by the project configuration
SET_SOURCE_FILES_PROPERTIES(
    ${GENERATED}
    PROPERTIES
        GENERATED 1
    )

SOURCE_GROUP(Generated FILES
    ${GENERATED}
    )

SET( SOURCES
    ${GENERAL}
    ${GENERATED}
    )

# This will include Win/projectDef.cmake, X11/projectDef.cmake, Mac/projectDef 
# depending on the platform
include_platform()
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${dbus_LIBRARIES})
message(${dbus_LIBRARIES})
