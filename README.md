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

The license for this project is the MIT license. Text of MIT license can be found in the LICENSE-NOTICES.txt file in the top level directory. Each source files should include a license notice that designates the licensing terms for the respective file.

### Software Dependencies

- qtbase  5.5.99+5.6.0 (https://github.com/qt/qtbase/tree/5.6)
- qtwayland  5.5.99+5.6.0 (https://github.com/qt/qtwayland/tree/5.6)
- qtdeclarative  5.5.99+5.6.0 (https://github.com/qt/qtdeclarative/tree/5.6)
- qtwebkit  5.5.99+5.6.0 (https://github.com/qt/qtwebkit/tree/5.6)
- qtmultimedia  5.5.99+5.6.0 (https://github.com/qt/qtmultimedia/tree/5.6)
- qtquick1  5.5.99+5.6.0 (https://github.com/qt/qtquick1/tree/5.6)
- qtsvg  5.5.99+5.6.0 (https://github.com/qt/qtsvg/tree/5.6)
- qtgraphicaleffects  5.5.99+5.6.0 (https://github.com/qt/qtgraphicaleffects/tree/5.6)
- libpam  1.2.1 (http://linux-pam.org/library/Linux-PAM-1.2.1.tar.bz2)
- gstreamer1.0-plugins-bad  1.6.1 (https://github.com/GStreamer/gst-plugins-bad)
- gstreamer1.0-plugins-ugly  1.6.1 (https://github.com/GStreamer/gst-plugins-ugly)
- wayland  1.7.0 (https://wayland.freedesktop.org/releases/1.7.0.tar.xz)
- mesa  11.1.2 (https://cgit.freedesktop.org/mesa/mesa/log/?h=11.1)
- libdrm  2.4.66 (https://dri.freedesktop.org/libdrm/libdrm/ibdrm-2.4.66.tar.bz2)

### Prerequisites

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
$ cd qt5 && git checkout 5.6
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

### Credits

- tango weather clear (wrscompositor-master/compositor/hmi/cockpit/tango/01d.svg), © 2010 warszawianka (https://openclipart.org/user-detail/warszawianka) used under a Creative Commons Zero 1.0 Public Domain License: https://creativecommons.org/publicdomain/zero/1.0/ 
- tango weather clear night (wrscompositor-master/compositor/hmi/cockpit/tango/01n.svg), © 2010 warszawianka (https://openclipart.org/user-detail/warszawianka) used under a Creative Commons Zero 1.0 Public Domain License: https://creativecommons.org/publicdomain/zero/1.0/  
- tango weather few clouds (wrscompositor-master/compositor/hmi/cockpit/tango/02d.svg), © 2010 warszawianka (https://openclipart.org/user-detail/warszawianka) used under a Creative Commons Zero 1.0 Public Domain License: https://creativecommons.org/publicdomain/zero/1.0/  
- tango weather overcast (wrscompositor-master/compositor/hmi/cockpit/tango/03d.svg), © 2010 warszawianka (https://openclipart.org/user-detail/warszawianka) used under a Creative Commons Zero 1.0 Public Domain License: https://creativecommons.org/publicdomain/zero/1.0/  
- tango weather showers (wrscompositor-master/compositor/hmi/cockpit/tango/09d.svg), © 2010 warszawianka (https://openclipart.org/user-detail/warszawianka) used under a Creative Commons Zero 1.0 Public Domain License: https://creativecommons.org/publicdomain/zero/1.0/  
- tango weather showers scattered (wrscompositor-master/compositor/hmi/cockpit/tango/10d.svg), © 2010 warszawianka (https://openclipart.org/user-detail/warszawianka) used under a Creative Commons Zero 1.0 Public Domain License: https://creativecommons.org/publicdomain/zero/1.0/  
- tango weather storm (wrscompositor-master/compositor/hmi/cockpit/tango/11d.svg), © 2010 warszawianka (https://openclipart.org/user-detail/warszawianka) used under a Creative Commons Zero 1.0 Public Domain License: https://creativecommons.org/publicdomain/zero/1.0/  
- tango weather snow (wrscompositor-master/compositor/hmi/cockpit/tango/13d.svg), © 2010 warszawianka (https://openclipart.org/user-detail/warszawianka) used under a Creative Commons Zero 1.0 Public Domain License: https://creativecommons.org/publicdomain/zero/1.0/  
- tango waether severe alert (wrscompositor-master/compositor/hmi/cockpit/tango/error.svg), © 2010 warszawianka (https://openclipart.org/user-detail/warszawianka) used under a Creative Commons Zero 1.0 Public Domain License: https://creativecommons.org/publicdomain/zero/1.0/  
- B&W Abstract Wallpaper (wrscompositor-master/compositor/hmi/cockpit/resources/wallpaper.svg), © 2010 mystica (https://openclipart.org/user-detail/mystica) used under a Creative Commons Zero 1.0 Public Domain License: https://creativecommons.org/publicdomain/zero/1.0/  
- song playlist free icon (wrscompositor-master/compositor/hmi/cockpit/resources/album-cover.png), © 2013 Appzgear (http://appzgear.com) from www.flaticon.com used under a Creative Commons Attribution 3.0 Unported License: https://creativecommons.org/licenses/by/3.0/ 
- Backspace arrow free icon (wrscompositor-master/compositor/hmi/cockpit/resources/backspace.svg), © 2014 Google (http://www.google.com) from www.flaticon.com used under a Creative Commons Attribution 3.0 Unported License: https://creativecommons.org/licenses/by/3.0/  
- Enter arrow free icon (wrscompositor-master/compositor/hmi/cockpit/resources/enter.svg), © 2014 Google ( http://www.google.com) from www.flaticon.com used under a Creative Commons Attribution 3.0 Unported License: https://creativecommons.org/licenses/by/3.0/ 
- Hide keyboard button free icon (wrscompositor-master/compositor/hmi/cockpit/resources/hidekeyboard.svg), © 2014 Google (http://www.google.com) from www.flaticon.com used under a Creative Commons Attribution 3.0 Unported License: https://creativecommons.org/licenses/by/3.0/  
- Baseline shift free icon (wrscompositor-master/compositor/hmi/cockpit/resources/shift-gray.svg), © 2016 Dario Ferrando (http://dario.io) from www.flaticon.com used under a Creative Commons Attribution 3.0 Unported License: https://creativecommons.org/licenses/by/3.0/  
- Baseline shift free icon (wrscompositor-master/compositor/hmi/cockpit/resources/shift-green.svg), © 2016 Dario Ferrando (http://dario.io) from www.flaticon.com used under a Creative Commons Attribution 3.0 Unported License: https://creativecommons.org/licenses/by/3.0/  
- Cancel free icon (wrscompositor-master/compositor/hmi/cockpit/icons/close.png), © 2016 Roundicons (https://roundicons.com) from www.flaticon.com used under a Flaticon Basic License: http://file000.flaticon.com/downloads/license/license.pdf 
- Translation free icon (wrscompositor-master/compositor/hmi/cockpit/resources/global.svg), © 2016 Freepik (http://www.freepik.com) from www.flaticon.com used under a Flaticon Basic License: http://file000.flaticon.com/downloads/license/license.pdf  
- Map free icon (wrscompositor-master/compositor/hmi/cockpit/icons/native-map-location.png), © 2016 Madebyoliver (http://www.flaticon.com/authors/madebyoliver) from www.flaticon.com used under a Flaticon Basic License: http://file000.flaticon.com/downloads/license/license.pdf  
- Play button free icon (wrscompositor-master/compositor/hmi/cockpit/icons/native-video.png), © 2016 Madebyoliver (http://www.flaticon.com/authors/madebyoliver) from www.flaticon.com used under a Flaticon Basic License: http://file000.flaticon.com/downloads/license/license.pdf  
- Worldwide free icon (wrscompositor-master/compositor/hmi/cockpit/icons/native-web.png), © 2016 Gregor Cresnar (http://www.flaticon.com/authors/gregor-cresnar) from www.flaticon.com used under a Flaticon Basic License: http://file000.flaticon.com/downloads/license/license.pdf  
- Play free icon (wrscompositor-master/compositor/hmi/cockpit/resources/play.png), © 2016 Freepik (http://www.freepik.com)  from www.flaticon.com used under a Flaticon Basic License: http://file000.flaticon.com/downloads/license/license.pdf
- Stop free icon (wrscompositor-master/compositor/hmi/cockpit/resources/stop.png), © 2016 Freepik (http://www.freepik.com)  from www.flaticon.com used under a Flaticon Basic License: http://file000.flaticon.com/downloads/license/license.pdf
- Backward free icon (wrscompositor-master/compositor/hmi/cockpit/resources/backward.svg), © 2016 Freepik (http://www.freepik.com)  from www.flaticon.com used under a Flaticon Basic License: http://file000.flaticon.com/downloads/license/license.pdf
- Pause free icon (wrscompositor-master/compositor/hmi/cockpit/resources/pause.svg), © 2016 Freepik (http://www.freepik.com)  from www.flaticon.com used under a Flaticon Basic License: http://file000.flaticon.com/downloads/license/license.pdf
- Forward free icon (wrscompositor-master/compositor/hmi/cockpit/resources/forward.svg), © 2016 Freepik (http://www.freepik.com)  from www.flaticon.com used under a Flaticon Basic License: http://file000.flaticon.com/downloads/license/license.pdf
- Speaker free icon (wrscompositor-master/compositor/hmi/cockpit/resources/volume-up.svg), © 2016 Madebyoliver (http://www.flaticon.com/authors/madebyoliver)  from www.flaticon.com used under a Flaticon Basic License: http://file000.flaticon.com/downloads/license/license.pdf


### Legal Notices

Disclaimer of Warranty / No Support: Wind River does not provide support and maintenance services for this software, under Wind River’s standard Software Support and Maintenance Agreement or otherwise. Unless required by applicable law, Wind River provides the software (and each contributor provides its contribution) on an “AS IS” BASIS, WITHOUT WARRANTIES OF ANY KIND, either express or implied, including, without limitation, any warranties of TITLE, NONINFRINGEMENT, MERCHANTABILITY, or FITNESS FOR A PARTICULAR PURPOSE. You are solely responsible for determining the appropriateness of using or redistributing the software and assume any risks associated with your exercise of permissions under the license.
