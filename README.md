# ##########################################################
#
# Windriver WRSCompoistor based on QtWayland and compatible with GENIVI Wayland Extenstion Protocol
# Supported Platform
# 1. GENIVI/AGL platform
# 2. Windriver Helix Cockpit platform
#
# ##########################################################

### Overview

This README page provides a quick summary of how to build and run the WRSCompositor that will reside in on your Ubuntu Desktop.
In order place wrscompositor on target board based on Yocto, You will need to build/install/run wrscompositor through wrscompositor's recipe. 

### Project License

The license for this project is the MIT-Clause. Text of MIT-Clause license can be found in the COPYING.MIT file in the top level directory. Each source files should include a license notice that designates the licensing terms for the respective file.


### Prerequisite

Before install WRSCompositor, please prepare your development environment and install qt/wayland library.
1. Setup setup environment for local install  
```
$ export WRSCOMPOSITOR=$HOME/install  
$ export LD_LIBRARY_PATH=$WRSCOMPOSITOR/lib  
$ export PKG_CONFIG_PATH=$WRSCOMPOSITOR/lib/pkgconfig/:$WRSCOMPOSITOR/share/pkgconfig/  
$ export PATH=$WRSCOMPOSITOR/bin:$PATH  
$ export ACLOCAL_PATH=$WRSCOMPOSITOR/share/aclocal  
$ export ACLOCAL="aclocal -I $ACLOCAL_PATH"  
$ mkdir -p $ACLOCAL_PATH  
```

2. Clone wayland git and Configure/Build/Install wayland  
```
$ git clone git://anongit.freedesktop.org/wayland/wayland  
$ cd wayland  
$ ./autogen.sh --prefix=$WRSCOMPOSITOR        
$ make && make install  
```

3. Clone qt5.git and make sure qt can be configured with available options. (recommend qt-5.5)  
   if you have such a problem of configuring qt libraray, Yon need to refer "https://wiki.qt.io/Building_Qt_5_from_Git" site.  
   this website explain how to install various packages that are dependent on qt library.  
```
$ git clone git://code.qt.io/qt/qt5.git.  
$ cd qt5 && git checkout 5.5  
$ git submodule update --init **# updating each submodule to match the supermodule.**  
$ cd qt5/qtwebengine && git submodule update --init **# updating qtwebengine module to get 3rdparty's chromium**  
$ cd qt5  
$ ./configure -prefix $WRSCOMPOSITOR -nomake examples -gstreamer -nomake tests -confirm-license -opensource -kms -opengl es2 -egl -linuxfb  
$ make && make install **# building all of qt-modules**  
$ cd qt5/qtwayland  
$ qmake CONFIG+=wayland-compositor **# building qtwayland to build/run wrscompositor**  
$ make && make install 
```

### Configuration/Installation for WRSCompositor

After install Qt Library, You can configure/build/install wrscompositor based on qtwayland.  
1. Configure wrscompositor that is able to work on Linux Desktop.  
   if you want to know more available options in configure, Try to use --help option.  
```
$ ./configure --with-qmake=$WRSCompositor/bin/qmake --hmi-profile=cockpit --overwrite
```

2. Build/Install WRSCompositor. as default prefix path, all of both binaries and library are installed under /usr directory.  
   if you want to change install path, Try to use --prefix option in configure script before make Wrscompositor.  
```
$ make && sudo make install. 
```

### Run WRSCompositor

After install WRSCompositor, You can run WRSCompositor with additioanl environments.  
```
$ export QT_QPA_PLATFORM=eglfs **# WRSCompositor is main-compositor that is able to access gpu driver.**  
$ export XDG_RUNTIME_DIR="/run/user/1000" **# Try to check specific number related to login account (ex: general user account is mapped to 1000)**    
$ wrscompositor &  **# Try to use --help option to know more avaiable arguments.**  
```

When wrscompositor want to be sub-compositor under main-compositor like weston, you need to export QT_QPA_PLATFORM to wayland.


### Run Client

After run WRSCompositor as background process, You can run such a applications based on QT or non-QT under WRSCompositor.
1. Non QT application like weston client.  
   this applcation should include wayland client-protocol to commuicate with WrsCompositor.


2. Qt applcation  
  this application don't need to include wayland client-protocol. that's why qt applcation try to use qtwayland library. 


3. Setup environments & Run Applcations  
  You need to make sure that there is wayland-0 in /run/user/1000.  
  if wayland-0 is created in this directory, wrscompositor can composite client's window.  
```
$ export WAYLAND_DISPLAY=wayland-0  
$ export QT_QPA_PLATFORM=wayland  
$ skobblernavi  
```

### Legal Notices

Disclaimer of Warranty / No Support: Wind River does not provide support and maintenance services for this software, under Wind River’s standard Software Support and Maintenance Agreement or otherwise. Unless required by applicable law, Wind River provides the software (and each contributor provides its contribution) on an “AS IS” BASIS, WITHOUT WARRANTIES OF ANY KIND, either express or implied, including, without limitation, any warranties of TITLE, NONINFRINGEMENT, MERCHANTABILITY, or FITNESS FOR A PARTICULAR PURPOSE. You are solely responsible for determining the appropriateness of using or redistributing the software and assume any risks associated with your exercise of permissions under the license.
