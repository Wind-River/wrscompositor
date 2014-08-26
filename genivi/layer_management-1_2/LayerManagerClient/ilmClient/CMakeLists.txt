############################################################################
# 
# Copyright 2010-2012 BMW Car IT GmbH
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

project(ilmClient)
project_type(CORE)

find_package(Threads)

include_directories(
    "include"
    "${CMAKE_SOURCE_DIR}/config"
    "${CMAKE_SOURCE_DIR}/LayerManagerClient/ilmCommon/include"
    "${CMAKE_SOURCE_DIR}/LayerManagerBase/include"
    "${CMAKE_SOURCE_DIR}/LayerManagerUtils/include"
)

add_library(${PROJECT_NAME} SHARED
    src/ilm_client.c
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RESOURCE "${CMAKE_BINARY_DIR}/LayerManagerClient/ilmClient/ilm.pc")

add_dependencies(${PROJECT_NAME}
    LayerManagerUtils
    ilmCommon
)

set(LIBS
    ${LIBS}
    LayerManagerUtils
    ilmCommon
    rt
    dl
    ${CMAKE_THREAD_LIBS_INIT}
)

target_link_libraries(${PROJECT_NAME} ${LIBS})

configure_file(ilm.pc.in ilm.pc @ONLY)

if(NOT PKG_CONFIG_INSTALL_LIBDIR)
    include(FindPkgConfig)
    execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=pc_path pkg-config OUTPUT_VARIABLE PC_PATHS)
    string(REPLACE ":" ";" PC_PATHS ${PC_PATHS})
    list(GET PC_PATHS 0 PKG_CONFIG_INSTALL_LIBDIR)
endif(NOT PKG_CONFIG_INSTALL_LIBDIR)

install (
    TARGETS             ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    RESOURCE DESTINATION ${PKG_CONFIG_INSTALL_LIBDIR}
)

install (
    FILES       include/ilm_client.h
    DESTINATION include/ilm
)

