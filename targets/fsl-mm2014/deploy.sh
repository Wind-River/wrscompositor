#!/bin/bash
cat deploy.sh
FILENAME=windriver-`date +%Y%m%d`.tar.gz
tar -zcvf $FILENAME etc/init.d/ipod-daemon-2 etc/init.d/wr-amb etc/rc5.d/S30ipod-daemon-2 etc/rc5.d/S99wr-amb opt/windriver/

md5sum $FILENAME
