%module wrs_ipod_2
%{
#include "wrs-ipod-2.h"
#include "wrs_ipod_2_pyutil.h"

// FIXME to avoid undefined reference
int wrs_ipod_enter_videooutmode_sync(WRSIPodConnection *c) {
    return -1;
}
const char* wrs_ipod_indexed_track_composer(WRSIPodConnection *c) {
    return NULL;
}
const char* wrs_ipod_indexed_track_genre(WRSIPodConnection *c) {
    return NULL;
}
%}

typedef int ipod_transaction_t;



%include "wrs-ipod-2.h"
%include "wrs_ipod_2_pyutil.h"
