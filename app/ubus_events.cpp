#include <unistd.h>
#include <signal.h>
#include <string>
#include <string_view>
#include <cstdint>
#include <cmath>
#include <numbers>

#include <Logger/Logger.h>

extern "C" {
#include <ubox/uloop.h>
#include <ubox/ustream.h>
#include <ubox/utils.h>
#include <ubus/libubus.h>
#include <json-c/json.h>
#include <ubox/blobmsg_json.h>
}

#include "GPSData.h"
#include "mon.h"
#include "defines.h"
#include "ubus_base.h"
#include "ubus_events.h"
#include "ubus_common.h"
#include "ubus_policy.h" 


/*
    A GNSS event handler that parses GPS data from the ubus message
    and updates the current GPS data. If the bearing or distance has changed,
    a binary data packet in the Wialon format is constructed and sent.
*/
void ubus_gnss_event(struct ubus_context *ctx, struct ubus_event_handler *ev,
                             const char *type, struct blob_attr *msg)
{
    if (!msg)
    {
        return;
    }

    struct blob_attr *tb[__GNSS_EXTENDED_DATA_MAX];
    blobmsg_parse(gnss_extended_data_policy, __GNSS_EXTENDED_DATA_MAX, tb, blob_data(msg), blob_len(msg));

    get_currGps().status = static_cast<uint8_t>(blobmsg_get_u16(tb[STATUS]));
    get_currGps().flags = static_cast<uint16_t>(blobmsg_get_u16(tb[FLAGS]));
    get_currGps().latitude = static_cast<uint64_t>(blobmsg_get_u64(tb[LATITUDE]) * 1e10);
    get_currGps().longitude = static_cast<uint64_t>(blobmsg_get_u64(tb[LONGITUDE]) * 1e10);
    get_currGps().altitude = static_cast<uint64_t>(blobmsg_get_u64(tb[ALTITUDE]) * 1e11);
    get_currGps().speed = static_cast<uint16_t>(blobmsg_get_u32(tb[SPEED]));
    get_currGps().bearing = static_cast<uint16_t>(std::round(blobmsg_get_u32(tb[BEARING]) / 100.0)); // division with rounding
    get_currGps().accuracy = static_cast<uint16_t>(blobmsg_get_u32(tb[ACCURACY]));
    get_currGps().timestamp = static_cast<uint32_t>(blobmsg_get_u64(tb[TIMESTAMP]) / 1000); // conversion to seconds
    get_currGps().fix_svs = static_cast<uint16_t>(blobmsg_get_u32(tb[FIX_SVS]));
    get_currGps().visible_svs = static_cast<uint8_t>(blobmsg_get_u32(tb[VISIBLE_SVS]));
    // SV_LIST
    LOGGER_INFO(kModuleName,"packet: status={:d}, flags={:d}, latitude={:d}, longitude={:d}, altitude={:d}, speed={:d}, bearing={:d}, accuracy={:d}, timestamp={:d}, fix_svs={:d}, visible_svs={:d}", 
        get_currGps().status, get_currGps().flags, get_currGps().latitude, get_currGps().longitude, get_currGps().altitude, get_currGps().speed, get_currGps().bearing, get_currGps().accuracy, get_currGps().timestamp, get_currGps().fix_svs, get_currGps().visible_svs);
    // {
    //     buildWialonBinary(get_currGps());
    //     sendWialonBinary();
    //     //state = MON_STATE::BEARING_CHANGED;
    // }
    // else if (distanceChanged() && get_currGps().flags == 31) // проверка delta distance
    // {
    //     buildWialonBinary(get_currGps());
    //     sendWialonBinary();
    // }
    // if (get_currGps().flags == 31)
    // {
    //     get_prevGps() = get_currGps();
    // }

    if (bearingChanged() || distanceChanged())
    {
        auto packet = buildWialonBinary(get_currGps());
        sendWialonBinary(packet);
        timeout_set();
    }
}

/*
    Ignition event handler that sends GPS data
    in Wialon format. If the flags of the current GPS data are 31 (full data),
    the current data is used; otherwise, the previous data is used.
*/
void ubus_ignition_event(struct ubus_context *ctx, struct ubus_event_handler *ev,
                         const char *type, struct blob_attr *msg)
{
    if (!msg)
    {
        return;
    }

    const GPSData& gpsToSend = (get_currGps().flags == 31) ? get_currGps() : get_prevGps();

    auto packet = buildWialonBinary(gpsToSend);

    sendWialonBinary(packet);
}

/*
    Registers event handlers for ubus: ignition event
    and extended GNSS data. Returns 0 if registration is successful, or -1 if an error occurs.
*/
int ubus_register_events(struct ubus_context *ctx)
{
	if (!ctx)
    {
        LOGGER_INFO("ubus_register_events", "ubus context error.");
        return -1;
    }

    int ret = 0;
    
    static struct ubus_event_handler ign_handler;
	memset(&ign_handler, 0, sizeof(ign_handler));
	ign_handler.cb = ubus_ignition_event;

	ret = ubus_register_event_handler(ctx, &ign_handler, UBUS_EVENT_IGNITION);

	if (ret)
	{
        LOGGER_INFO("ubus_register_events", " register event \"{:s}\" failed", UBUS_EVENT_IGNITION);
		return -1;
	}

    static struct ubus_event_handler gnss_handler;
	memset(&gnss_handler, 0, sizeof(gnss_handler));
	gnss_handler.cb = ubus_gnss_event;

	ret = ubus_register_event_handler(ctx, &gnss_handler, GNSS_EXTENDED_DATA);

	if (ret)
	{
        LOGGER_INFO("ubus_register_events", " register event \"{:s}\" failed", GNSS_EXTENDED_DATA);
		return -1;
	}

	return 0;
}

int send_start_event(void)
{
    BlobBufBuilder blob;

    auto res = blob.add("svc", SERVICE_NAME).get();
    LOGGER_INFO(SERVICE_NAME, "{:s}> start_event type: {:s}",kModuleName, UBUS_EVENT_START);
    return ubus_send_event(get_ubus_ctx(), UBUS_EVENT_START, res.head);
}
