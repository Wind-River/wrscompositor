############################################################################
# 
# Copyright 2010-2012 BMW Car IT GmbH 
# Copyright (c) 2013 DirectFB integrated media GmbH
# Copyright (c) 2013 Renesas Solutions Corp.
# 
# Licensed under the Apache License, Version 2.0 (the "License"); 
# you may not use this file except in compliance with the License. 
# You may obtain a copy of the License at 
#
#		http://www.apache.org/licenses/LICENSE-2.0 
#
# Unless required by applicable law or agreed to in writing, software 
# distributed under the License is distributed on an "AS IS" BASIS, 
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
# See the License for the specific language governing permissions and 
# limitations under the License.
#
############################################################################

cmake_minimum_required (VERSION 2.6)

project_type(EXAMPLE)

include_directories ("include")
include_directories ("../../LayerManagerClient/ilmClient/include")
include_directories ("../LayerSceneDescriptionExample/include")
include_directories ("../../LayerManagerUtils/include")
include_directories ("../../LayerManagerClient/ilmControl/include")

add_executable(EGLDFBApplicationExample src/egl_helper.cpp src/eglDFB_application.cpp src/gles2application.cpp)

add_dependencies(EGLDFBApplicationExample ilmClient)
add_dependencies(EGLDFBApplicationExample LayerManagerUtils)

find_package (GLESv2 REQUIRED)
include_directories(${GLESv2_INCLUDE_DIR})

find_package (EGL REQUIRED)
include_directories(${EGL_INCLUDE_DIR})

find_package (DirectFB REQUIRED)
include_directories(${DIRECTFB_INTERNAL_INCLUDE_DIRS})
include_directories(${DIRECTFB_EGL_INCLUDE_DIRS})

set(LIBS ${LIBS} ${GLESv2_LIBRARIES} ${DIRECTFB_LDFLAGS} ${DIRECTFB_LIBRARIES} ${DIRECTFB_EGL_LDFLAGS} ${DIRECTFB_EGL_LIBRARIES} ${EGL_LIBRARY} LayerManagerUtils)
target_link_libraries(EGLDFBApplicationExample ${LIBS} ilmClient ilmControl)

install (TARGETS EGLDFBApplicationExample DESTINATION bin)
