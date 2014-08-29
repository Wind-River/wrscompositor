#include "wrs_ipod_2_pyutil.h"
#include "wrs_ipod_2_wrap.c"



#ifdef THREAD_SUPPORT
static PyThreadState *tstate;
#define enter_python() \
     if (tstate) PyEval_RestoreThread(tstate)

#define leave_python() \
     tstate = PyEval_SaveThread()
#else
#define enter_python()
#define leave_python()
#endif


void handler_event_cb(WRSIPodConnection *c, const WRSIPodEvent *ev, void *user_data) {
    PyObject *tuple, *ret;
    PyObject *cb_arg;
    tuple = (PyObject *) user_data;
    PyObject *_c = SWIG_NewPointerObj(SWIG_as_voidptr(c), SWIGTYPE_p__WRSIPodConnection, 0 |  0 );
    PyObject *_ev = SWIG_NewPointerObj(SWIG_as_voidptr(ev), SWIGTYPE_p__WRSIPodEvent, 0 |  0 );

    cb_arg = Py_BuildValue("(OOO)", _c, _ev, PyTuple_GetItem(tuple, 1));

    enter_python();
    ret = PyObject_CallObject(PyTuple_GetItem(tuple, 0), cb_arg);
    if (!ret) {
    if (PyErr_ExceptionMatches(PyExc_KeyboardInterrupt)) {
        exit(0);
    }
    PyErr_Print();
    }
    leave_python();
    Py_DECREF(cb_arg);
    Py_DECREF(_c);
    Py_DECREF(_ev);
}
PyObject *event_cb_userdata = NULL;

void wrs_ipod_set_event_pycb(PyObject* _c, PyObject *callback, PyObject *user_data) {
    PyObject *data;
    WRSIPodConnection *c = (WRSIPodConnection *) 0 ;
    void *argp1 = 0 ;
    int res1 = 0 ;

    res1 = SWIG_ConvertPtr(_c, &argp1, SWIGTYPE_p__WRSIPodConnection, 0 |  0 );
    if (!SWIG_IsOK(res1)) {
        SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "cwin_set_event_callback" "', argument " "1"" of type '" "CWinDisplay *""'"); 
    }
    c = (WRSIPodConnection *)(argp1);
    if(callback==Py_None) {
        if(event_cb_userdata) {
            Py_DECREF((PyObject*)event_cb_userdata);
        }
        wrs_ipod_set_event_cb(c, NULL, NULL);
    } else {
        if (!PyCallable_Check(callback)) {
            fprintf(stderr, "2nd argument must be callable object\n");
            return;
        }
        event_cb_userdata = user_data;
        data = Py_BuildValue("(OO)", callback, user_data);
        wrs_ipod_set_event_cb(c, handler_event_cb, (void*)data);
    }

fail:
    return ;
}


void handler_reply_cb(WRSIPodConnection *c, const ipod_transaction_t tid, int retval, const char *error, void *user_data) {
    PyObject *tuple, *ret;
    PyObject *cb_arg;
    tuple = (PyObject *) user_data;
    PyObject *_c = SWIG_NewPointerObj(SWIG_as_voidptr(c), SWIGTYPE_p__WRSIPodConnection, 0 |  0 );

    cb_arg = Py_BuildValue("(OiisO)", _c, tid, retval, error, PyTuple_GetItem(tuple, 1));

    enter_python();
    ret = PyObject_CallObject(PyTuple_GetItem(tuple, 0), cb_arg);
    if (!ret) {
    if (PyErr_ExceptionMatches(PyExc_KeyboardInterrupt)) {
        exit(0);
    }
    PyErr_Print();
    }
    leave_python();
    Py_DECREF(cb_arg);
    Py_DECREF(_c);
}
PyObject *reply_cb_userdata = NULL;

void wrs_ipod_set_reply_pycb(PyObject* _c, PyObject *callback, PyObject *user_data) {
    PyObject *data;
    WRSIPodConnection *c = (WRSIPodConnection *) 0 ;
    void *argp1 = 0 ;
    int res1 = 0 ;

    res1 = SWIG_ConvertPtr(_c, &argp1, SWIGTYPE_p__WRSIPodConnection, 0 |  0 );
    if (!SWIG_IsOK(res1)) {
        SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "cwin_set_reply_callback" "', argument " "1"" of type '" "CWinDisplay *""'"); 
    }
    c = (WRSIPodConnection *)(argp1);
    if(callback==Py_None) {
        if(reply_cb_userdata) {
            Py_DECREF((PyObject*)reply_cb_userdata);
        }
        wrs_ipod_set_reply_cb(c, NULL, NULL);
    } else {
        if (!PyCallable_Check(callback)) {
            fprintf(stderr, "2nd argument must be callable object\n");
            return;
        }
        reply_cb_userdata = user_data;
        data = Py_BuildValue("(OO)", callback, user_data);
        wrs_ipod_set_reply_cb(c, handler_reply_cb, (void*)data);
    }

fail:
    return ;
}

PyObject* wrs_ipod_current_track_artwork_pydata(PyObject* _c) {
    WRSIPodConnection *c = (WRSIPodConnection *) 0 ;
    void *argp1 = 0 ;
    int res1 = 0 ;

    res1 = SWIG_ConvertPtr(_c, &argp1, SWIGTYPE_p__WRSIPodConnection, 0 |  0 );
    if (!SWIG_IsOK(res1)) {
        SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "cwin_set_reply_callback" "', argument " "1"" of type '" "CWinDisplay *""'"); 
    }
    c = (WRSIPodConnection *)(argp1);
    if(wrs_ipod_current_track_artwork_length(c)<=0) {
        goto fail;
    }
    PyObject *ret;
    ret = PyBuffer_FromReadWriteMemory( wrs_ipod_current_track_artwork_data(c),
            wrs_ipod_current_track_artwork_length(c)) ;
    return ret;
fail:
    Py_INCREF(Py_None);
    return Py_None;
}
