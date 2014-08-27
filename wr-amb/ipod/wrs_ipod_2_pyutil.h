#include <Python.h>
#include "wrs-ipod-2.h"

void wrs_ipod_set_event_pycb(PyObject* c, PyObject* event_cb, PyObject* userdata);
void wrs_ipod_set_reply_pycb(PyObject* c, PyObject* event_cb, PyObject* userdata);
//void wrs_ipod_set_ping_pycb(PyObject* c, PyObject* event_cb, PyObject* userdata);
