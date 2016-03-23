Wind River Wayland Compositor (a.k.a duduregi)
==============================================

* for GENIVI/AGL platform
* part of Helix Cockpit
* GENIVI Compliant

Qt Installation for duduregi (for dev env)
------------------------------------------

::

	$ cd qt-every....5.5.0     # Qt
	$ ./configure -prefix /opt/windriver ...
	$ # ./configure -prefix /opt/windriver -nomake examples -nomake tests -confirm-license -opensource -kms
	$ make -j8 && make install
	$ cd qtwayland             # QtWayland
	$ /opt/windriver/bin/qmake CONFIG+=wayland-compositor
	$ make -j8 && make install


Build
------

::

	$ ./configure --help
	$ ./configure --with-qtdir=/opt/windriver --overwrite
	$ make


WR-AMB (WindRiver Automotive Message Broker)
--------------------------------------------

* for demo purpose
* WebSocket Gateway for Google Earth Plugin based Driving Simulator
* Joystic Driving Wheel handler and uinput event generator
* local webserver which serves HTML5 app contents

::

	$ cd wr-amb
	$ ./wr-amb

Setup HTML5 Applications Demo
-----------------------------

::

	$ cd wr-amb/www
	$ ln -sf $(pwd)/html5apps /opt/windriver/share/html5apps
	$ ln -sf $(pwd)/tizen-web-ui-fw /opt/windriver/share/tizen-web-ui-fw

