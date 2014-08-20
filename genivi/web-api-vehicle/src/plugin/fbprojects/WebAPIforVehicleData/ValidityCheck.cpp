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
 * @file ValidityCheck.cpp 
 * For further information, see http://www.genivi.org/
 */

#include "ValidityCheck.h"

bool ValidityCheck::isValid(VehicleEventType type)
{
	return validity[type];
}

void ValidityCheck::setValidity(VehicleEventType type, bool value)
{
	if (validity.count(type))
	{
		validity[type] = value;
	}

	updateValidity(type);
}
