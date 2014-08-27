TARGET=_wrs_ipod_2.so
SWIG=swig
CC=gcc
PYTHON_CFLAGS=`python-config --cflags`
PYTHON_LIBS=`python-config --libs`
#WRSIPOD2_CFLAGS=`pkg-config --cflags libwrs-ipod-2`
#WRSIPOD2_LIBS=`pkg-config --libs libwrs-ipod-2`
WRSIPOD2_CFLAGS=-I.
WRSIPOD2_LIBS=-L. -lwrs-ipod-2
CFLAGS+=-O2 -Wall

all: $(TARGET)

_wrs_ipod_2.so: wrs_ipod_2_pyutil.o
	$(CC) -shared -o $@ $< $(WRSIPOD2_LIBS)

wrs_ipod_2_wrap.c: wrs_ipod_2.i wrs_ipod_2_pyutil.h
	$(SWIG) $(WRSIPOD2_CFLAGS) -python wrs_ipod_2.i

wrs_ipod_2_pyutil.o: wrs_ipod_2_wrap.c wrs_ipod_2_pyutil.c
	$(CC) $(CFLAGS) $(PYTHON_CFLAGS) $(WRSIPOD2_CFLAGS) -c -fPIC -o $@ wrs_ipod_2_pyutil.c

clean:
	rm -f *.o *_wrap.c wrs_ipod_2.py* $(TARGET)
