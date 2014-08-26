############################################################################
# 
# Copyright (c) 2013 DirectFB integrated media GmbH
# Copyright (c) 2013 Renesas Solutions Corp.
# 
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

#===========================================================================
# plugin configuration
#===========================================================================
project(DFBRenderer)
project_type(PLUGIN)

find_package(DirectFB REQUIRED)
find_package(EGL REQUIRED)
find_package(GLESv2 REQUIRED)
#find_package(Threads REQUIRED)

include_directories(
    include
    ../../Base/include
    ../../Graphic/include
    ${CMAKE_SOURCE_DIR}/config
    ${CMAKE_SOURCE_DIR}/LayerManagerBase/include
    ${CMAKE_SOURCE_DIR}/LayerManagerUtils/include
    ${DIRECTFB_INTERNAL_INCLUDE_DIRS} ${DIRECTFB_EGL_INCLUDE_DIRS}
    ${EGL_INCLUDE_DIR}
    ${GLESv2_INCLUDE_DIR}
)

set(LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${EGL_LIBRARY}
    ${GLESv2_LIBRARIES}
    ${DIRECTFB_LDFLAGS} ${DIRECTFB_LIBRARIES} ${DIRECTFB_EGL_LDFLAGS} ${DIRECTFB_EGL_LIBRARIES}
    LayerManagerGraphicDFB
    LayerManagerUtils
)

set(SRC_FILES
    src/DFBRenderer.cpp
    src/ShaderProgramGLES.cpp
)

#===========================================================================
# create plugin
#===========================================================================
add_library(${PROJECT_NAME} ${LIBRARY_BUILDMODE} ${SRC_FILES})

install(TARGETS             ${PROJECT_NAME}
        LIBRARY DESTINATION lib/layermanager/renderer
        ARCHIVE DESTINATION lib/layermanager/static)

#===========================================================================
# external libraries
#===========================================================================
target_link_libraries(${PROJECT_NAME} ${LIBS})

add_dependencies(${PROJECT_NAME} ${LIBS})
