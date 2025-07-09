#pragma once

extern "C" {
    #include <ubox/uloop.h>
    #include <ubox/ustream.h>
    #include <ubox/utils.h>
    #include <ubus/libubus.h>
    #include <json-c/json.h>
    #include <ubox/blobmsg_json.h>
    }

enum {
    HELLO_ID, //int
    HELLO_MSG, //string
    __HELLO_MAX
};

extern const struct blobmsg_policy hello_method_policy[__HELLO_MAX];

enum {
    DEBUG_LEVEL,
    __DEBUG_LEVEL_MAX
};

extern const struct blobmsg_policy set_debug_level_method_policy[__DEBUG_LEVEL_MAX];

/*
    ubus events
*/

#define	UBUS_EVENT_IGNITION	"ignition"

enum {
    IGNITION_EVENT_STATE,
    IGNITION_EVENT_SVC, //string
    __IGNITION_EVENT_MAX
};

extern const struct blobmsg_policy ignition_event_policy[__IGNITION_EVENT_MAX];

/*
    start event
*/

#define	UBUS_EVENT_START	"start"

enum {
    START_EVENT_SVC, //string
    __START_EVENT_MAX
};

extern const struct blobmsg_policy start_event_policy[__START_EVENT_MAX];

// enum class gnss_fix {
//   STATUS, //int16
//   FLAGS,  //int16
//   LATITUDE,
//   LONGITUDE,
//   ALTITUDE,
//   SPEED,
//   BEARING,
//   ACCURACY,
//   TIMESTAMP,
//   FIX_SVS,
//   VISIBLE_SVS,
//   __GNSS_FIX_MAX
// };

// extern const struct blobmsg_policy gnssFix_policy[__GNSS_FIX_MAX];

#define GNSS_EXTENDED_DATA "gnss_extended_data" // gps_hub.h
 
/*
!!! Каждый сервис сам принимиает решение о дальнейшей обработке набора данных на основе передаваемых битовых флагов
  
FLAGS:
GpsLocation has valid coords   -- GPS_LOCATION_HAS_LAT_LONG   0x0001
GpsLocation has valid altitude -- GPS_LOCATION_HAS_ALTITUDE   0x0002
GpsLocation has valid speed    -- GPS_LOCATION_HAS_SPEED      0x0004
GpsLocation has valid bearing  -- GPS_LOCATION_HAS_BEARING    0x0008
GpsLocation has valid accuracy -- GPS_LOCATION_HAS_ACCURACY   0x0010
*/
#define GNSS_NAV_MASK_ENABLED 0

enum {
    STATUS,
    FLAGS,
    LATITUDE,
    LONGITUDE,
    ALTITUDE,
    SPEED,
    BEARING,
    ACCURACY,
    TIMESTAMP,
    FIX_SVS,
    VISIBLE_SVS,
    SV_LIST,
#if GNSS_NAV_MASK_ENABLED
    EPHEMERIS_MASK,
    ALMANAC_MASK,
    FIX_MASK,
#endif
     __GNSS_EXTENDED_DATA_MAX
};
 
extern const struct blobmsg_policy gnss_extended_data_policy[__GNSS_EXTENDED_DATA_MAX];

enum {
    PRN,
    SNR,
    ELEVATION,
    AZIMUTH,
    USED_IN_FIX,
     __GNSS_SV_LIST_MAX
};