############################################################################
#
# Copyright (C) 2012 Bayerische Motorenwerke Aktiengesellschaft
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
############################################################################


# This is an example toolchain file, please adapt for your needs
# To use that file please call
# cmake <LayerManager_Source_Dir> -DCMAKE_TOOLCHAIN_FILE=<LayerManager_Source_Dir>/toolchain.cmake

set (CMAKE_FIND_ROOT_PATH /home/user/weston)
set (CMAKE_INSTALL_PREFIX /home/user/weston)
set (FFI_INCLUDE_DIR /usr/lib/libffi-3.0.9/include)
set (XKB_INCLUDE_DIR /home/user/weston/include/xkbcommon)

################################################################################
#                                                                              #
#                         Layer Manager Configuration                          #
#                                                                              #
################################################################################

# this preset enables the build of LayerManagement with Wayland Renderer Plugin
# based in X11 backend.
# Additionally the Wayland Example applications and LayerManagerControl tool
# is built.

# Build examples for client library usage
set (WITH_CLIENTEXAMPLES             OFF  CACHE BOOL "" FORCE)

# Build LayerManagement client library
set (WITH_CLIENT_LIB                 ON  CACHE BOOL "" FORCE)

# Build renderer plugin for OpenGL/X11 based platforms
set (WITH_DESKTOP                    OFF CACHE BOOL "" FORCE)

# Build LayerManagerControl binary
set (WITH_CONTROL_BIN                ON  CACHE BOOL "" FORCE)

# Build LayerManagement control library
set (WITH_CONTROL_LIB                OFF CACHE BOOL "" FORCE)

# Build with DLT logging support
set (WITH_DLT                        OFF CACHE BOOL "" FORCE)

# Generate documentation during build (requires doxygen)
set (WITH_DOCUMENTATION              OFF CACHE BOOL "" FORCE)

# Build examples for GLES/X11 based platforms
set (WITH_EGL_EXAMPLE                OFF  CACHE BOOL "" FORCE)

# Build scene provider plugin to run example applications
set (WITH_EXAMPLE_SCENE_PROVIDER     ON CACHE BOOL "" FORCE)

# Force Software Copy of Pixmaps (compatibility for VMs)
set (WITH_FORCE_COPY                 OFF CACHE BOOL "" FORCE)

# Build Generic Communicator Plugin based on IpcModules
set (WITH_GENERIC_COMMUNICATOR       ON  CACHE BOOL "" FORCE)

# Build development library for GLES/X11 based renderers
set (WITH_GLESv2_LIB                 OFF CACHE BOOL "" FORCE)

# Build examples for OpenGL/X11 based platforms
set (WITH_GLX_EXAMPLE                OFF CACHE BOOL "" FORCE)

# Build development library for OpenGL/X11 based renderers
set (WITH_GLX_LIB                    OFF CACHE BOOL "" FORCE)

# Build with TCP Ipc Module
set (WITH_IPC_MODULE_TCP             OFF CACHE BOOL "" FORCE)

# Build with D-Bus Ipc Module
set (WITH_IPC_MODULE_DBUS            ON  CACHE BOOL "" FORCE)

# Build LayerManagerService binary
set (WITH_SERVICE_BIN                ON  CACHE BOOL "" FORCE)

# Link all plugins and libraries statically
set (WITH_STATIC_LIBRARIES           OFF CACHE BOOL "" FORCE)

# Report styleguide problems during build (requires python)
set (WITH_STYLE_CHECKING             OFF CACHE BOOL "" FORCE)

# Build plugin for systemd based health monitoring
set (WITH_SYSTEMD_HEALTH_MONITOR     OFF CACHE BOOL "" FORCE)

# Build unit test binaries for all enabled components
set (WITH_TESTS                      OFF CACHE BOOL "" FORCE)

# Build renderer plugin with pure logging (no rendering)
set (WITH_TEXT_RENDERER              OFF CACHE BOOL "" FORCE)

# Build renderer plugin for GLES/Wayland with X11 backend
set (WITH_WAYLAND_X11                ON CACHE BOOL "" FORCE)

# Build development library for GLES/Wayland X11 based renderers
set (WITH_WAYLAND_X11_LIB            OFF CACHE BOOL "" FORCE)

# Build renderer plugin for GLES/Wayland with DRM backend
set (WITH_WAYLAND_DRM                OFF CACHE BOOL "" FORCE)

# Build development library for GLES/Wayland DRM based renderers
set (WITH_WAYLAND_DRM_LIB            OFF CACHE BOOL "" FORCE)

# Build renderer plugin for GLES/Wayland with FBDEV backend
set (WITH_WAYLAND_FBDEV              OFF CACHE BOOL "" FORCE)

# Build development library for GLES/Wayland FBDEV based renderers
set (WITH_WAYLAND_FBDEV_LIB          OFF CACHE BOOL "" FORCE)

# Build examples for GLES/Wayland based platforms
set (WITH_WL_EXAMPLE                 ON CACHE BOOL "" FORCE)

# Build renderer plugin for GLES/X11 based platforms
set (WITH_X11_GLES                   OFF  CACHE BOOL "" FORCE)

