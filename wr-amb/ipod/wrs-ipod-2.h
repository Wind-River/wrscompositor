/*
 * Copyright © 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
#ifndef _WRS_IPOD_2_H_
#define _WRS_IPOD_2_H_
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct _WRSIPodConnection;
typedef struct _WRSIPodConnection WRSIPodConnection;

typedef int ipod_transaction_t;

union _WRSIPodEvent;
typedef union _WRSIPodEvent WRSIPodEvent;
typedef enum _WRSIPodEventType {
    WRSIPOD_EVENT_CONNECTED                = 1<<1,
    WRSIPOD_EVENT_DISCONNECTED             = 1<<2,
    WRSIPOD_EVENT_DETECTION_FAILED         = 1<<3,
    WRSIPOD_EVENT_AUTHENTICATED            = 1<<4,
    WRSIPOD_EVENT_MODE_CHANGED             = 1<<5,
    WRSIPOD_EVENT_PLAYSTATE_CHANGED        = 1<<6,
    WRSIPOD_EVENT_TRACK_CHANGED            = 1<<7,
    WRSIPOD_EVENT_TRACKINFO_RETRIEVED      = 1<<8,
    WRSIPOD_EVENT_SAMPLERATE_RETRIEVED     = 1<<9,
    WRSIPOD_EVENT_TRACK_POSITION_CHANGED   = 1<<10,
    WRSIPOD_EVENT_CHAPTER_CHANGED          = 1<<11,
    WRSIPOD_EVENT_SHUFFLE_CHANGED          = 1<<12,
    WRSIPOD_EVENT_REPEAT_CHANGED           = 1<<13,
    WRSIPOD_EVENT_PLAYLIST_CHANGED         = 1<<14,
    WRSIPOD_EVENT_DEVICE_READY             = 1<<15,
    WRSIPOD_EVENT_UNKNOWN                  = 1<<31
} WRSIPodEventType;

typedef enum _WRSIPodPlayState {
    WRSIPOD_PLAY_STATE_STOPPED = 0,
    WRSIPOD_PLAY_STATE_FFW_STARTED,
    WRSIPOD_PLAY_STATE_REW_STARTED,
    WRSIPOD_PLAY_STATE_FFWREW_STOPPED,
    WRSIPOD_PLAY_STATE_PLAYING,
    WRSIPOD_PLAY_STATE_PAUSED
} WRSIPodPlayState;

typedef enum _WRSIPodShuffleState {
    WRSIPOD_SHUFFLE_OFF = 0,
    WRSIPOD_SHUFFLE_TRACKS,
    WRSIPOD_SHUFFLE_ALBUMS
} WRSIPodShuffleState;

typedef enum _WRSIPodRepeatState {
    WRSIPOD_REPEAT_OFF = 0,
    WRSIPOD_REPEAT_ONE_TRACK,
    WRSIPOD_REPEAT_ALL_TRACKS
} WRSIPodRepeatState;

typedef enum _WRSIPodHierarchy {
    WRSIPOD_HIERARCHY_AUDIO = 1,
    WRSIPOD_HIERARCHY_VIDEO = 2,
} WRSIPodHierarchy;

typedef enum _WRSIPodPreferenceKey {
    WRSIPOD_PREFKEY_VIDEOOUT = 0,
    WRSIPOD_PREFKEY_SCREEN,
    WRSIPOD_PREFKEY_VIDEOFORMAT,
    WRSIPOD_PREFKEY_LINEOUT,
    WRSIPOD_PREFKEY_VIDEOCONNECTION = 8,
    WRSIPOD_PREFKEY_CLOSEDCAPTION,
    WRSIPOD_PREFKEY_VIDEORATIO,
    WRSIPOD_PREFKEY_SUBTITLE = 0x0c,
    WRSIPOD_PREFKEY_ALTERNATEAUDIO,
} WRSIPodPreferenceKey;

typedef enum _WRSIPodPreferenceValue {
    WRSIPOD_PREFVAL_VIDEOOUT_OFF = 0,
    WRSIPOD_PREFVAL_VIDEOOUT_ON,
    WRSIPOD_PREFVAL_SCREEN_FILL_ENTIRE,
    WRSIPOD_PREFVAL_SCREEN_FIT_TO_EDGE,
    WRSIPOD_PREFVAL_VIDEOFORMAT_NTSC,
    WRSIPOD_PREFVAL_VIDEOFORMAT_PAL,
    WRSIPOD_PREFVAL_LINEOUT_NOT_USED,
    WRSIPOD_PREFVAL_LINEOUT_USED,
    WRSIPOD_PREFVAL_VIDEOCONNECTION_NONE,
    WRSIPOD_PREFVAL_VIDEOCONNECTION_COMPOSITE,
    WRSIPOD_PREFVAL_VIDEOCONNECTION_SVIDEO,
    WRSIPOD_PREFVAL_VIDEOCONNECTION_COMPONENT,
    WRSIPOD_PREFVAL_CLOSEDCAPTION_OFF,
    WRSIPOD_PREFVAL_CLOSEDCAPTION_ON,
    WRSIPOD_PREFVAL_VIDEORATIO_4_3,
    WRSIPOD_PREFVAL_VIDEORATIO_16_9,
    WRSIPOD_PREFVAL_SUBTITLE_OFF,
    WRSIPOD_PREFVAL_SUBTITLE_ON,
    WRSIPOD_PREFVAL_ALTERNATEAUDIO_OFF,
    WRSIPOD_PREFVAL_ALTERNATEAUDIO_ON,
    WRSIPOD_PREFVAL_UNKNOWN = 0xFF,
} WRSIPodPreferenceValue;

typedef enum _WRSIPodCategory {
    WRSIPOD_CATEGORY_TOP = 0,
    WRSIPOD_CATEGORY_PLAYLIST,
    WRSIPOD_CATEGORY_ARTIST,
    WRSIPOD_CATEGORY_ALBUM,
    WRSIPOD_CATEGORY_GENRE,
    WRSIPOD_CATEGORY_TRACK,
    WRSIPOD_CATEGORY_COMPOSER,
    WRSIPOD_CATEGORY_AUDIOBOOK,
    WRSIPOD_CATEGORY_PODCAST,
    WRSIPOD_CATEGORY_NESTED_PLAYLIST,
    WRSIPOD_CATEGORY_GENIUS_MIXES,
    WRSIPOD_CATEGORY_ITUNES_U,
} WRSIPodCategory;

typedef void (*wrs_ipod_event_cb)(WRSIPodConnection*, const WRSIPodEvent*, void *);
typedef void (*wrs_ipod_reply_cb)(WRSIPodConnection*, ipod_transaction_t,
        int retval, const char *error, void *);
typedef void (*wrs_ipod_ping_cb)(WRSIPodConnection*, ipod_transaction_t, void *);

WRSIPodConnection* wrs_ipod_connect(const char *serveraddr);

int wrs_ipod_fd(WRSIPodConnection*);
int wrs_ipod_pid(WRSIPodConnection*);
int wrs_ipod_ping(WRSIPodConnection*);
void wrs_ipod_close(WRSIPodConnection*);
int wrs_ipod_is_ready(WRSIPodConnection *);
int wrs_ipod_is_remote(WRSIPodConnection *);
int wrs_ipod_is_session_opened(WRSIPodConnection *);
int wrs_ipod_read(WRSIPodConnection *);

int wrs_ipod_device_count(WRSIPodConnection *);
const char* wrs_ipod_device_name(WRSIPodConnection *, int);
const char* wrs_ipod_device_serial(WRSIPodConnection *, int);
int wrs_ipod_device_type(WRSIPodConnection *, int);
int wrs_ipod_device_authenticated(WRSIPodConnection *, int );
int wrs_ipod_device_used(WRSIPodConnection *, int );
int wrs_ipod_device_is_extended(WRSIPodConnection *, int );
int wrs_ipod_device_ready(WRSIPodConnection *, int );
int wrs_ipod_device_support_itunes_db(WRSIPodConnection *, int);
int wrs_ipod_device_audio_count(WRSIPodConnection *, int);
int wrs_ipod_device_index(WRSIPodConnection *, const char *);

void wrs_ipod_set_event_cb(WRSIPodConnection *, wrs_ipod_event_cb, void*);
void wrs_ipod_set_reply_cb(WRSIPodConnection *, wrs_ipod_reply_cb, void*);
void wrs_ipod_set_ping_cb(WRSIPodConnection *, wrs_ipod_ping_cb, void *);

WRSIPodEventType wrs_ipod_event_type(const WRSIPodEvent* event);
/* event for all session */
const char* wrs_ipod_event_connected_serial(const WRSIPodEvent* event);
const char* wrs_ipod_event_disconnected_serial(const WRSIPodEvent* event);
const char* wrs_ipod_event_device_serial(const WRSIPodEvent* event);

/* event for current session */
int wrs_ipod_event_authenticated(const WRSIPodEvent* event);
int wrs_ipod_event_mode_changed(const WRSIPodEvent* event);

int wrs_ipod_current_session(WRSIPodConnection *);
int wrs_ipod_set_current_session(WRSIPodConnection *, int);
ipod_transaction_t wrs_ipod_open_session(WRSIPodConnection *, int index);
int wrs_ipod_open_session_sync(WRSIPodConnection *, int index);

ipod_transaction_t wrs_ipod_close_session(WRSIPodConnection *);

ipod_transaction_t wrs_ipod_play(WRSIPodConnection *);
int wrs_ipod_play_sync(WRSIPodConnection *);

ipod_transaction_t wrs_ipod_stop(WRSIPodConnection *);
int wrs_ipod_stop_sync(WRSIPodConnection *);

ipod_transaction_t wrs_ipod_pause(WRSIPodConnection *);
int wrs_ipod_pause_sync(WRSIPodConnection *);

ipod_transaction_t wrs_ipod_next_track(WRSIPodConnection *);
int wrs_ipod_next_track_sync(WRSIPodConnection *);

ipod_transaction_t wrs_ipod_prev_track(WRSIPodConnection *);
int wrs_ipod_prev_track_sync(WRSIPodConnection *);

ipod_transaction_t wrs_ipod_start_ffw(WRSIPodConnection *);
int wrs_ipod_start_ffw_sync(WRSIPodConnection *);

ipod_transaction_t wrs_ipod_start_rew(WRSIPodConnection *);
int wrs_ipod_start_rew_sync(WRSIPodConnection *);

ipod_transaction_t wrs_ipod_set_position(WRSIPodConnection *, int);
int wrs_ipod_set_position_sync(WRSIPodConnection *, int);

ipod_transaction_t wrs_ipod_stop_ffwrew(WRSIPodConnection *);
int wrs_ipod_stop_ffwrew_sync(WRSIPodConnection *);

ipod_transaction_t wrs_ipod_get_play_state(WRSIPodConnection *);
WRSIPodPlayState wrs_ipod_get_play_state_sync(WRSIPodConnection *);

ipod_transaction_t wrs_ipod_get_current_track_index(WRSIPodConnection *);
int wrs_ipod_get_current_track_index_sync(WRSIPodConnection *);

ipod_transaction_t wrs_ipod_set_shuffle(WRSIPodConnection *, WRSIPodShuffleState);
int wrs_ipod_set_shuffle_sync(WRSIPodConnection *, WRSIPodShuffleState);

ipod_transaction_t wrs_ipod_set_repeat(WRSIPodConnection *, WRSIPodRepeatState);
int wrs_ipod_set_repeat_sync(WRSIPodConnection *, WRSIPodRepeatState);

ipod_transaction_t wrs_ipod_get_shuffle(WRSIPodConnection *);
WRSIPodShuffleState wrs_ipod_get_shuffle_sync(WRSIPodConnection *);

ipod_transaction_t wrs_ipod_get_repeat(WRSIPodConnection *);
WRSIPodRepeatState wrs_ipod_get_repeat_sync(WRSIPodConnection *);

ipod_transaction_t wrs_ipod_write_tag(WRSIPodConnection *);
int wrs_ipod_write_tag_sync(WRSIPodConnection *);

ipod_transaction_t wrs_ipod_set_preference(WRSIPodConnection *, WRSIPodPreferenceKey, WRSIPodPreferenceValue);
int wrs_ipod_set_preference_sync(WRSIPodConnection *, WRSIPodPreferenceKey, WRSIPodPreferenceValue);

ipod_transaction_t wrs_ipod_get_preference(WRSIPodConnection *, WRSIPodPreferenceKey);
WRSIPodPreferenceValue wrs_ipod_get_preference_sync(WRSIPodConnection *, WRSIPodPreferenceKey);

ipod_transaction_t wrs_ipod_current_track_info(WRSIPodConnection *);
int wrs_ipod_current_track_info_sync(WRSIPodConnection *);
ipod_transaction_t wrs_ipod_get_sample_rate(WRSIPodConnection *);
const char* wrs_ipod_current_track_title(WRSIPodConnection *);
const char* wrs_ipod_current_track_uid(WRSIPodConnection *);
const char* wrs_ipod_current_track_artist(WRSIPodConnection *);
const char* wrs_ipod_current_track_album(WRSIPodConnection *);
const char* wrs_ipod_current_track_chapter(WRSIPodConnection *);
int wrs_ipod_current_number_of_tracks(WRSIPodConnection *);
int wrs_ipod_current_track_index(WRSIPodConnection *);
int wrs_ipod_current_chapter_index(WRSIPodConnection *);
int wrs_ipod_current_chapter_count(WRSIPodConnection *);
int wrs_ipod_current_track_length(WRSIPodConnection *);
int wrs_ipod_current_track_position(WRSIPodConnection *);
int wrs_ipod_current_track_timestamp(WRSIPodConnection *);
int wrs_ipod_current_track_has_artwork(WRSIPodConnection *);
int wrs_ipod_current_track_has_chapters(WRSIPodConnection *);
WRSIPodPlayState wrs_ipod_current_track_state(WRSIPodConnection *);
int wrs_ipod_current_sample_rate(WRSIPodConnection *);
int wrs_ipod_current_track_is_audiobook(WRSIPodConnection *);
int wrs_ipod_current_track_is_podcast(WRSIPodConnection *);
int wrs_ipod_current_shuffle_state(WRSIPodConnection *);
int wrs_ipod_current_repeat_state(WRSIPodConnection *);

ipod_transaction_t wrs_ipod_current_track_artwork(WRSIPodConnection *, int);
int wrs_ipod_current_track_artwork_sync(WRSIPodConnection *, int);
uint32_t wrs_ipod_current_track_artwork_length(WRSIPodConnection *);
uint8_t* wrs_ipod_current_track_artwork_data(WRSIPodConnection *);
uint16_t wrs_ipod_current_track_artwork_width(WRSIPodConnection *);
uint16_t wrs_ipod_current_track_artwork_height(WRSIPodConnection *);
uint16_t wrs_ipod_current_track_artwork_rowstride(WRSIPodConnection *);

ipod_transaction_t wrs_ipod_set_hierarchy(WRSIPodConnection *, WRSIPodHierarchy);
int wrs_ipod_set_hierarchy_sync(WRSIPodConnection *, WRSIPodHierarchy);
ipod_transaction_t wrs_ipod_set_category(WRSIPodConnection *, WRSIPodCategory);
int wrs_ipod_set_category_sync(WRSIPodConnection *, WRSIPodCategory);
WRSIPodCategory wrs_ipod_get_category(WRSIPodConnection *);
ipod_transaction_t wrs_ipod_select_db(WRSIPodConnection *, int, WRSIPodCategory view);
int wrs_ipod_select_db_sync(WRSIPodConnection *, int, WRSIPodCategory view);
ipod_transaction_t wrs_ipod_get_records(WRSIPodConnection *,
                                                int offset, int count);
int wrs_ipod_get_records_sync(WRSIPodConnection *,
                                                int offset, int count);
int wrs_ipod_record_count(WRSIPodConnection *);
const char* wrs_ipod_record(WRSIPodConnection *, int offset);
void* wrs_ipod_records(WRSIPodConnection *);

ipod_transaction_t wrs_ipod_reauthentication(WRSIPodConnection *, int use_digital_audio);
int wrs_ipod_reauthentication_sync(WRSIPodConnection *, int use_digital_audio);

ipod_transaction_t wrs_ipod_enter_videooutmode(WRSIPodConnection *);
int wrs_ipod_enter_videooutmode_sync(WRSIPodConnection *);

ipod_transaction_t wrs_ipod_exit_videooutmode(WRSIPodConnection *);

ipod_transaction_t wrs_ipod_get_videooutoptions(WRSIPodConnection *,unsigned int);
int wrs_ipod_get_videooutoptions_sync(WRSIPodConnection *,unsigned int);

ipod_transaction_t wrs_ipod_set_videooutoptions(WRSIPodConnection *, int);
int wrs_ipod_set_videooutoptions_sync(WRSIPodConnection *, int);

ipod_transaction_t wrs_ipod_set_statechange(WRSIPodConnection *, int);
int wrs_ipod_set_statechange_sync(WRSIPodConnection *, int);

ipod_transaction_t wrs_ipod_get_mode(WRSIPodConnection *);

ipod_transaction_t wrs_ipod_navigate_videoout(WRSIPodConnection *, int);
int wrs_ipod_navigate_videoout_sync(WRSIPodConnection *, int);


int wrs_ipod_current_track_timestamp_usec(WRSIPodConnection *c);


///////////// database engine ///////////////

// request functions
ipod_transaction_t wrs_ipod_get_db_itunes_info(WRSIPodConnection *, int);
ipod_transaction_t wrs_ipod_get_db_track_info(WRSIPodConnection *c, int, int, int);
ipod_transaction_t wrs_ipod_get_db_track_info_sync(WRSIPodConnection *c, int, int, int);
ipod_transaction_t wrs_ipod_get_pb_track_info(WRSIPodConnection *c, int, int, int);
ipod_transaction_t wrs_ipod_get_pb_track_info_sync(WRSIPodConnection *c, int, int, int);

ipod_transaction_t wrs_ipod_request_lingo_version(WRSIPodConnection *, int);
char *wrs_ipod_get_result_lingo_version(WRSIPodConnection *c);
int wrs_ipod_request_lingo_version_sync(WRSIPodConnection *c, int lingo);

// get result functions
char *wrs_ipod_get_result_db_device_uid(WRSIPodConnection *c);
char *wrs_ipod_get_result_db_last_sync_time(WRSIPodConnection *c);
int wrs_ipod_get_result_db_itunes_info_count(WRSIPodConnection *c);
void *wrs_ipod_get_result_db_track_info(WRSIPodConnection *c);
void *wrs_ipod_get_result_pb_track_info(WRSIPodConnection *c);

ipod_transaction_t wrs_ipod_prepare_uid_list(WRSIPodConnection *, int, int, int, const char*);
int wrs_ipod_prepare_uid_list_sync(WRSIPodConnection *, const char*);
ipod_transaction_t wrs_ipod_play_uid_list(WRSIPodConnection *, const char*);
int wrs_ipod_play_uid_list_sync(WRSIPodConnection *, const char*);
ipod_transaction_t wrs_ipod_set_current_playing_track_chapter(WRSIPodConnection *, int);
ipod_transaction_t wrs_ipod_set_current_playing_track(WRSIPodConnection *, int);

ipod_transaction_t wrs_ipod_indexed_track_info(WRSIPodConnection *, int);
int wrs_ipod_indexed_track_info_sync(WRSIPodConnection *, int);
void *wrs_ipod_get_result_indexed_track_info(WRSIPodConnection *c);

ipod_transaction_t wrs_ipod_indexed_track_chapter_info(WRSIPodConnection *, int, int);
int wrs_ipod_indexed_track_chapter_info_sync(WRSIPodConnection *, int, int);
void *wrs_ipod_get_result_indexed_track_chapter_info(WRSIPodConnection *c);

const char* wrs_ipod_indexed_track_title(WRSIPodConnection *c);
const char* wrs_ipod_indexed_track_uid(WRSIPodConnection *c);
const char* wrs_ipod_indexed_track_artist(WRSIPodConnection *c);
const char* wrs_ipod_indexed_track_album(WRSIPodConnection *c);
const char* wrs_ipod_indexed_track_composer(WRSIPodConnection *c);
const char* wrs_ipod_indexed_track_genre(WRSIPodConnection *c);
int wrs_ipod_indexed_track_is_audiobook(WRSIPodConnection *c);
int wrs_ipod_indexed_track_is_podcast(WRSIPodConnection *c);
int wrs_ipod_indexed_track_length(WRSIPodConnection *);
int wrs_ipod_indexed_track_chapter_count(WRSIPodConnection *c);

int wrs_ipod_device_port(WRSIPodConnection *, int);
int wrs_ipod_device_sound_card_id(WRSIPodConnection *, int);
int wrs_ipod_device_product_id(WRSIPodConnection *, int);

ipod_transaction_t wrs_ipod_set_available_current(WRSIPodConnection *, int);
int wrs_ipod_set_available_current_sync(WRSIPodConnection *, int);

ipod_transaction_t wrs_ipod_set_bluetooth_state(WRSIPodConnection *, int, const char*);

ipod_transaction_t wrs_ipod_get_artwork_data(WRSIPodConnection *, const char *);
int wrs_ipod_get_artwork_data_sync(WRSIPodConnection *c, const char *uid);

#ifdef __cplusplus
}
#endif
#endif

// vim600: sw=4 ts=8 sts=4 et bs=2 fdm=marker fileencoding=utf8

