WR's Genivi based IVI system demo
=================================

* based on wr-tizenivi demo
* run on wayland/x.org
* add genivi features
* add HTML5 app f/w

Qt Installation for duduregi & demo stuff
------------------------------------------

::

	$ cd qt-every....5.5.0     # Qt
	$ ./configure -prefix /opt/windriver ...
	$ make -j8 && make install
	$ cd qtwayland             # QtWayland
	$ /opt/windriver/bin/qmake CONFIG+=wayland-compositor
	$ make -j8 && make install


duduregi X11
------------

Build

::

	$ cd duduregi
	$ ./configure --with-wlcompositor --with-webengine
	$ make


Run on Desktop environment

::

	$ Xephyr :1 -ac -screen 800x480 &
	$ DISPLAY=:1 ./duduregi


duduregi Wayland (Experimental)
-------------------------------

::

	$ cd duduregi
	$ ./configure --with-wlcompositor --with-webengine
	$ make


WR-AMB (WindRiver Automotive Message Broker)
--------------------------------------------

::

	$ cd wr-amb
	$ ./wr-amb

HTML5 Applications
------------------

::

	$ cd wr-amb/www
	$ ln -sf $(pwd)/html5apps /opt/windriver/share/html5apps
	$ ln -sf $(pwd)/tizen-web-ui-fw /opt/windriver/share/tizen-web-ui-fw


Native Qt App examples
----------------------

::

	$ cd qt5-examples
	$ cd eyes
	$ /opt/windriver/bin/qmake && make
	$ cp eyes /opt/windriver/bin
	$ cd ../qt5-examples
	$ cd mocknavi
	$ /opt/windriver/bin/qmake && make
	$ cp mocknavi /opt/windriver/bin

Genivi VehicleDataSimulator (VDS)
----------------------------------

Build

::

	$ cd genivi/web-api-vehicle/src/VehicleDataSimulator/
	$ qmake-qt4
	$ make

Run on PC

::

	$ cp ../../bin/network.cfg
	Edit VNA_IP as Target Device's IP in the network.cfg
	$ ./VehicleDataSimulator

Supported Vehicle Info

* Vehicle Information -> World Manufacturer ID
* Running Status 1 -> Speedometer

Genivi VehicleNetworkAdapter (VNA)
----------------------------------

Build

::

	$ make

Run on Target

::

	$ cp ../../bin/network.cfg
	$ ./VehicleDataSimulator
