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
 * @file VehicleError.h 
 * For further information, see http://www.genivi.org/
 */

#ifndef H_Error
#define H_Error

class Error
{
public:
	static const short NOT_AVAILABLE_ERR = 101;
};

#endif // H_Error

#ifndef H_VehicleError
#define H_VehicleError

class VehicleError : public Error
{
public:
	static const short ACCESS_DENIED = 1;
	static const short NOT_AVAILABLE = 2;
	static const short UNKNOWN = 0;
};

#endif // H_VehicleError
