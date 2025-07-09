#include <unistd.h>
#include <signal.h>

#include "defines.h"
#include "ubus_base.h"
#include "ubus_methods.h"
#include "main.h"
#include "system.h"
#include "ubus_policy.h"

extern "C" {
#include <ubox/uloop.h>
#include <ubox/ustream.h>
#include <ubox/utils.h>
#include <ubus/libubus.h>
#include <json-c/json.h>
#include <ubox/blobmsg_json.h>
}

#include "ubus_policy.h"

const struct blobmsg_policy hello_method_policy[__HELLO_MAX] = {
	[HELLO_ID] = { .name = "id", .type = BLOBMSG_TYPE_INT32 },
	[HELLO_MSG] = { .name = "msg", .type = BLOBMSG_TYPE_STRING },
};

const struct blobmsg_policy set_debug_level_method_policy[__DEBUG_LEVEL_MAX] = {
        [DEBUG_LEVEL] = { .name = "debug_level", .type = BLOBMSG_TYPE_INT32 }
};

const struct blobmsg_policy ignition_event_policy[__IGNITION_EVENT_MAX] = {
        [IGNITION_EVENT_STATE] = { .name = "ignition", .type = BLOBMSG_TYPE_INT32 },
        [IGNITION_EVENT_SVC] = { .name = "svc", .type = BLOBMSG_TYPE_STRING },
};

// const struct blobmsg_policy gnssFix_policy[gnss_fix::__GNSS_FIX_MAX] = {
//         [gnss_fix::STATUS] = { .name = "status", .type = BLOBMSG_TYPE_INT16 },
//         [gnss_fix::FLAGS] = { .name = "flags", .type = BLOBMSG_TYPE_INT16 },
//         [gnss_fix::LATITUDE] = { .name = "latitude", .type = BLOBMSG_TYPE_INT64 },
//         [gnss_fix::LONGITUDE] = { .name = "longitude", .type = BLOBMSG_TYPE_INT64 },
//         [gnss_fix::ALTITUDE] = { .name = "altitude", .type = BLOBMSG_TYPE_INT64 },
//         [gnss_fix::SPEED] = { .name = "speed", .type = BLOBMSG_TYPE_INT32 },
//         [gnss_fix::BEARING] = { .name = "bearing", .type = BLOBMSG_TYPE_INT32 },
//         [gnss_fix::ACCURACY] = { .name = "accuracy", .type = BLOBMSG_TYPE_INT32 },
//         [gnss_fix::TIMESTAMP] = { .name = "timestamp", .type = BLOBMSG_TYPE_INT64 },
//         [gnss_fix::FIX_SVS] = { .name = "fix_svs", .type = BLOBMSG_TYPE_INT32 },
//         [gnss_fix::VISIBLE_SVS] = { .name = "visible_svs", .type = BLOBMSG_TYPE_INT32 },
// };


const struct blobmsg_policy gnss_extended_data_policy[__GNSS_EXTENDED_DATA_MAX] = {
    [STATUS]      = { .name = "status",      .type = BLOBMSG_TYPE_INT16 },
    [FLAGS]       = { .name = "flags",       .type = BLOBMSG_TYPE_INT16 },
    [LATITUDE]    = { .name = "latitude",    .type = BLOBMSG_TYPE_INT64 },
    [LONGITUDE]   = { .name = "longitude",   .type = BLOBMSG_TYPE_INT64 },
    [ALTITUDE]    = { .name = "altitude",    .type = BLOBMSG_TYPE_INT64 },
    [SPEED]       = { .name = "speed",       .type = BLOBMSG_TYPE_INT32 },
    [BEARING]     = { .name = "bearing",     .type = BLOBMSG_TYPE_INT32 },
    [ACCURACY]    = { .name = "accuracy",    .type = BLOBMSG_TYPE_INT32 },
    [TIMESTAMP]   = { .name = "timestamp",   .type = BLOBMSG_TYPE_INT64 },
    [FIX_SVS]     = { .name = "fix_svs",     .type = BLOBMSG_TYPE_INT32 },
    [VISIBLE_SVS] = { .name = "visible_svs", .type = BLOBMSG_TYPE_INT32 },
    [SV_LIST]     = { .name = "sv_list",     .type = BLOBMSG_TYPE_ARRAY },
#if GNSS_NAV_MASK_ENABLED
    [EPHEMERIS_MASK] = { .name = "ephemeris_mask", .type = BLOBMSG_TYPE_INT32 },
    [ALMANAC_MASK]   = { .name = "almanac_mask",   .type = BLOBMSG_TYPE_INT32 },
    [FIX_MASK]       = { .name = "fix_mask",       .type = BLOBMSG_TYPE_INT32 },
#endif
};

static const struct blobmsg_policy sv_list_policy[__GNSS_SV_LIST_MAX] = {
    [PRN]         = { .name = "prn",         .type = BLOBMSG_TYPE_INT32 },
    [SNR]         = { .name = "snr",         .type = BLOBMSG_TYPE_INT32 },
    [ELEVATION]   = { .name = "elevation",   .type = BLOBMSG_TYPE_INT32 },
    [AZIMUTH]     = { .name = "azimuth",     .type = BLOBMSG_TYPE_INT32 },
    [USED_IN_FIX] = { .name = "used_in_fix", .type = BLOBMSG_TYPE_INT8  },
};