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

set(PLUGIN_NAME "WebAPIforVehicleData")
set(PLUGIN_PREFIX "WAVD")
set(COMPANY_NAME "LGE")

# ActiveX constants:
set(FBTYPELIB_NAME WebAPIforVehicleDataLib)
set(FBTYPELIB_DESC "WebAPIforVehicleData 1.0 Type Library")
set(IFBControl_DESC "WebAPIforVehicleData Control Interface")
set(FBControl_DESC "WebAPIforVehicleData Control Class")
set(IFBComJavascriptObject_DESC "WebAPIforVehicleData IComJavascriptObject Interface")
set(FBComJavascriptObject_DESC "WebAPIforVehicleData ComJavascriptObject Class")
set(IFBComEventSource_DESC "WebAPIforVehicleData IFBComEventSource Interface")
set(AXVERSION_NUM "1")

# NOTE: THESE GUIDS *MUST* BE UNIQUE TO YOUR PLUGIN/ACTIVEX CONTROL!  YES, ALL OF THEM!
set(FBTYPELIB_GUID 3f3e5047-5f84-520c-9190-849d29aa98cd)
set(IFBControl_GUID 1ab7db6d-54df-58da-9c28-e721fc1b8a07)
set(FBControl_GUID 833249b1-cea0-556f-83e2-df5893b0485e)
set(IFBComJavascriptObject_GUID 70fd5917-848a-5d25-b8ab-d62dc8fcf991)
set(FBComJavascriptObject_GUID 31c2882b-0dd5-5393-a0ed-f19c2bbf9cf1)
set(IFBComEventSource_GUID b696b62d-17ca-57e6-b829-f700338990be)

# these are the pieces that are relevant to using it from Javascript
set(ACTIVEX_PROGID "LGE.WebAPIforVehicleData")
set(MOZILLA_PLUGINID "lge.com/WebAPIforVehicleData")

# strings
set(FBSTRING_CompanyName "LGE")
set(FBSTRING_FileDescription "The interfaces for web applications in IVI system to access vehicle data delivered via vehicle bus")
set(FBSTRING_PLUGIN_VERSION "1.0.0.0")
set(FBSTRING_LegalCopyright "Copyright 2013 LGE")
set(FBSTRING_PluginFileName "np${PLUGIN_NAME}.dll")
set(FBSTRING_ProductName "Web API for Vehicle Data")
set(FBSTRING_FileExtents "")
set(FBSTRING_PluginName "Web API for Vehicle Data")
set(FBSTRING_MIMEType "application/x-webapiforvehicledata")

# Uncomment this next line if you're not planning on your plugin doing
# any drawing:

set (FB_GUI_DISABLED 1)

# Mac plugin settings. If your plugin does not draw, set these all to 0
set(FBMAC_USE_QUICKDRAW 0)
set(FBMAC_USE_CARBON 0)
set(FBMAC_USE_COCOA 0)
set(FBMAC_USE_COREGRAPHICS 0)
set(FBMAC_USE_COREANIMATION 0)
set(FBMAC_USE_INVALIDATINGCOREANIMATION 0)

# If you want to register per-machine on Windows, uncomment this line
#set (FB_ATLREG_MACHINEWIDE 1)
