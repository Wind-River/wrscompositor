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
 * @file ValidityCheck.h 
 * For further information, see http://www.genivi.org/
 */

#ifndef H_ValidityCheck
#define H_ValidityCheck

#include <map>
#include "TypeDef.h"

class ValidityCheck
{
public:
	virtual ~ValidityCheck() {};
	bool isValid(VehicleEventType type);
	void setValidity(VehicleEventType type, bool value);

protected:
	virtual void updateValidity(VehicleEventType type) = 0;

	std::map<VehicleEventType, bool> validity;
};

#endif // H_ValidityCheck
