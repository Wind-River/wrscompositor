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
#               http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
############################################################################

cmake_minimum_required (VERSION 2.6)
project(LayerManagerGraphicDFB)
project_type(CORE)

#===========================================================================
# configuration
#===========================================================================
SET(RENDERERBASE_DIR ${CMAKE_SOURCE_DIR}/LayerManagerPlugins/Renderers/Base)
SET(GRAPHIC_LIB_DIR ${CMAKE_SOURCE_DIR}/LayerManagerPlugins/Renderers/Graphic)

#===========================================================================
# component
#===========================================================================
find_package(EGL REQUIRED)
find_package(GLESv2 REQUIRED)
find_package(Threads)
find_package(DirectFB REQUIRED)

if (NOT DirectFB_FOUND)
message(FATAL_ERROR "DFB: Package DirectFB required, but not found!")
endif()

include_directories(
    ${GRAPHIC_LIB_DIR}/include
    ${RENDERERBASE_DIR}/include
    ${CMAKE_SOURCE_DIR}/config
    ${CMAKE_SOURCE_DIR}/LayerManagerPlugins/Renderers/Platform/DFBRenderer/include
    ${CMAKE_SOURCE_DIR}/LayerManagerBase/include
    ${CMAKE_SOURCE_DIR}/LayerManagerUtils/include
    ${EGL_INCLUDE_DIR}
    ${GLESv2_INCLUDE_DIR}
)

set(SOURCES
    ${RENDERERBASE_DIR}/src/BaseRenderer.cpp
    ${GRAPHIC_LIB_DIR}/src/GraphicSystems/DFBGraphicSystem.cpp
    ${GRAPHIC_LIB_DIR}/src/GraphicSystems/GLESGraphicSystem.cpp
    ${GRAPHIC_LIB_DIR}/src/WindowSystems/BaseWindowSystem.cpp
)

set(LIBS
    LayerManagerUtils
    LayerManagerBase
    ${CMAKE_THREAD_LIBS_INIT}
    ${EGL_LIBRARY}
    ${GLESv2_LIBRARIES}
)

if (DirectFB_FOUND)
    list(APPEND SOURCES
        ${GRAPHIC_LIB_DIR}/src/WindowSystems/DFBWindowSystem.cpp
        ${GRAPHIC_LIB_DIR}/src/TextureBinders/DFBEglImage.cpp
        ${GRAPHIC_LIB_DIR}/src/TextureBinders/DFBImage.cpp
    )
    list(APPEND LIBS
        ${DIRECTFB_LDFLAGS} ${DIRECTFB_LIBRARIES} ${DIRECTFB_EGL_LDFLAGS} ${DIRECTFB_EGL_LIBRARIES}
    )
    include_directories(
        ${DIRECTFB_INTERNAL_INCLUDE_DIRS} ${DIRECTFB_EGL_INCLUDE_DIRS}
    )
endif()

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${LIBS})

add_dependencies(${PROJECT_NAME} ${LIBS})

#===========================================================================
# install
#===========================================================================
install(TARGETS     ${PROJECT_NAME}
        DESTINATION lib)

install(FILES       ${GRAPHIC_LIB_DIR}/include/GraphicSystems/BaseGraphicSystem.h
                    ${GRAPHIC_LIB_DIR}/include/GraphicSystems/DFBGraphicSystem.h
                    ${GRAPHIC_LIB_DIR}/include/GraphicSystems/GLESGraphicSystem.h
        DESTINATION include/layermanager/graphic/GraphicSystems)

install(FILES       ${GRAPHIC_LIB_DIR}/include/TextureBinders/ITextureBinder.h
        DESTINATION include/layermanager/graphic/TextureBinders)

if (DirectFB_FOUND)
    install(FILES       ${GRAPHIC_LIB_DIR}/include/TextureBinders/DFBEglImage.h
                        ${GRAPHIC_LIB_DIR}/include/TextureBinders/DFBEglImage.h
            DESTINATION include/layermanager/graphic/TextureBinders)
endif()

if (DirectFB_FOUND)
    install(FILES       ${GRAPHIC_LIB_DIR}/include/PlatformSurfaces/DFBPlatformSurface.h
                        ${GRAPHIC_LIB_DIR}/include/PlatformSurfaces/EglDFBPlatformSurface.h
            DESTINATION include/layermanager/graphic/PlatformSurfaces)
endif()

install(FILES       ${GRAPHIC_LIB_DIR}/include/WindowSystems/BaseWindowSystem.h
        DESTINATION include/layermanager/graphic/WindowSystems)

if (DirectFB_FOUND)
    install(FILES       ${GRAPHIC_LIB_DIR}/include/WindowSystems/DFBWindowSystem.h
            DESTINATION include/layermanager/graphic/WindowSystems)
endif()

install(FILES       ${RENDERERBASE_DIR}/include/BaseRenderer.h
                    ${RENDERERBASE_DIR}/include/RenderUtil.h
        DESTINATION include/layermanager/graphic/Base)
