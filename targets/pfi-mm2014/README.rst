Installation
=============

Enable systemd services
------------------------

* systemctl enable X.service
* systemctl enable pulseaudio.service
* systemctl enable ipod-daemon-2.service
* systemctl enable wr-amb.service
* systemctl enable duduregi.service

Directory structure
-------------------

::

	/opt/windriver/bin/cp-reset
	/opt/windriver/bin/ipod-daemon-2
	/opt/windriver/lib/libwrs-iopd-2.so
	/opt/windriver/duduregi
	/opt/windriver/wr-amb
	/opt/windriver/share/html5apps -> ../wr-amb/www/html5apps
	/opt/windriver/share/tizen-web-ui-fw -> ../wr-amb/www/tizen-web-ui-fw
