WR's Genivi based IVI system demo
=================================

* based on wr-tizenivi demo
* run on wayland/x.org
* add genivi features
* add HTML5 app f/w


duduregi X11
------------

Build

::

	$ cd duduregi
	$ ./configure
	$ make


Run on Desktop environment

::

	$ Xephyr :1 -ac -screen 800x480 &
	$ DISPLAY=:1 ./duduregi


duduregi Wayland (Optional)
---------------------------

::

	$ cd duduregi
	$ ./configure --with-wlcompositor
	$ make


WR-AMB (WindRiver Automotive Message Broker)
--------------------------------------------

::

	$ cd wr-amb
	$ ./wr-amb

Genivi Vehicle Data Simulator
-----------------------------

Build

::

	$ cd genivi/web-api-vehicle/src/VehicleDataSimulator/
	$ qmake-qt4
	$ make

Run with WR-AMB

::

	$ cp ../../bin/network.cfg
	Edit VNA_IP as WR-AMB's IP in the network.cfg
	$ ./VehicleDataSimulator
