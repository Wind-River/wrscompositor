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
 * @file Factory.cpp
 * For further information, see http://www.genivi.org/
 */

#include "FactoryBase.h"
#include "WebAPIforVehicleData.h"
#include <boost/make_shared.hpp>

class PluginFactory : public FB::FactoryBase
{
public:
    ///////////////////////////////////////////////////////////////////////////////
    /// @fn FB::PluginCorePtr createPlugin(const std::string& mimetype)
    ///
    /// @brief  Creates a plugin object matching the provided mimetype
    ///         If mimetype is empty, returns the default plugin
    ///////////////////////////////////////////////////////////////////////////////
    FB::PluginCorePtr createPlugin(const std::string& mimetype)
    {
        return boost::make_shared<WebAPIforVehicleData>();
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    /// @see FB::FactoryBase::globalPluginInitialize
    ///////////////////////////////////////////////////////////////////////////////
    void globalPluginInitialize()
    {
        WebAPIforVehicleData::StaticInitialize();
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    /// @see FB::FactoryBase::globalPluginDeinitialize
    ///////////////////////////////////////////////////////////////////////////////
    void globalPluginDeinitialize()
    {
        WebAPIforVehicleData::StaticDeinitialize();
    }
};

///////////////////////////////////////////////////////////////////////////////
/// @fn getFactoryInstance()
///
/// @brief  Returns the factory instance for this plugin module
///////////////////////////////////////////////////////////////////////////////
FB::FactoryBasePtr getFactoryInstance()
{
    static boost::shared_ptr<PluginFactory> factory = boost::make_shared<PluginFactory>();
    return factory;
}

