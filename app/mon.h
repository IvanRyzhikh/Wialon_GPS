#pragma once

#include <ubox/uloop.h>

#include <stdlib.h>
#include <cmath>
#include <numbers>
#include <iomanip>
#include <sstream>

#include <ubus_base.h>
#include <ubus_policy.h>
#include <ubus_events.h>
#include <ret_code.h>
#include "GPSData.h"
#include "PacketBuilder.h"
#include "defines.h"

extern struct uloop_timeout mon_timer;

GPSData& get_currGps();
GPSData& get_prevGps();

#define EARTH_RADIUS 6371000.0

bool bearingChanged();

double haversine_distance(double lat1, double lon1, double lat2, double lon2);

bool distanceChanged();

Status findUbusService(std::string_view service);


/*
    maybe a future implementation of gnss_fix
*/
/* bool isGnssTestmodeEnable = false;

enum {
  GNSS_CHECK_SPEED, //int
  __GNSS_CHECK_SPEED_MAX
};

const struct blobmsg_policy gnssCheckSpeed_policy[__GNSS_CHECK_SPEED_MAX] = {
    [GNSS_CHECK_SPEED] = { .name = "gnss_testmode_resp", .type = BLOBMSG_TYPE_INT32 },
};


static void gnssCheckSpeed_cb(struct ubus_request *req, int type, struct blob_attr *msg)
{
    struct blob_attr *tb[__GNSS_CHECK_SPEED_MAX];
    blobmsg_parse(gnssCheckSpeed_policy, __GNSS_CHECK_SPEED_MAX, tb, blob_data(msg), blob_len(msg));

    int a = static_cast<uint32_t>(blobmsg_get_u64(tb[GNSS_CHECK_SPEED]));
    int a = static_cast<uint32_t>(blobmsg_get_u64(tb[GNSS_CHECK_SPEED]));
} */
/* static void gnssFix_cb(struct ubus_request *req, int type, struct blob_attr *msg)
{
    struct blob_attr *tb[gnss_fix::__GNSS_FIX_MAX];
    blobmsg_parse(gnssFix_policy, __GNSS_FIX_MAX, tb, blob_data(msg), blob_len(msg));

    currGps.status = static_cast<uint8_t>(blobmsg_get_u16(tb[STATUS]));
    currGps.flags = static_cast<uint16_t>(blobmsg_get_u16(tb[FLAGS]));
    currGps.latitude = static_cast<uint32_t>(blobmsg_get_u64(tb[LATITUDE]));
    currGps.longitude = static_cast<uint32_t>(blobmsg_get_u64(tb[LONGITUDE]));
    currGps.altitude = static_cast<uint32_t>(blobmsg_get_u64(tb[ALTITUDE]));
    currGps.speed = static_cast<uint32_t>(blobmsg_get_u32(tb[SPEED]));
    currGps.bearing = static_cast<uint32_t>(blobmsg_get_u32(tb[BEARING]));
    currGps.accuracy = static_cast<uint32_t>(blobmsg_get_u32(tb[ACCURACY]));
    currGps.timestamp = static_cast<uint32_t>(blobmsg_get_u64(tb[TIMESTAMP]) / 1000);
    currGps.fix_svs = static_cast<uint16_t>(blobmsg_get_u32(tb[FIX_SVS]));
    currGps.visible_svs = static_cast<uint16_t>(blobmsg_get_u32(tb[VISIBLE_SVS]));
    LOGGER_INFO(kModuleName,"packet: status={:d}, flags={:d}, latitude={:d}, longitude={:d}, altitude={:d}, speed={:d}, bearing={:d}, accuracy={:d}, timestamp={:d}, fix_svs={:d}, visible_svs={:d}", 
        currGps.status, currGps.flags, currGps.latitude, currGps.longitude, currGps.altitude, currGps.speed, currGps.bearing, currGps.accuracy, currGps.timestamp, currGps.fix_svs, currGps.visible_svs);
}

Status invokeGnssFix()
{
    if (findUbusService("currGps").has_value())
    {
        ubus_invoke(get_ubus_ctx(), id, "gnss_fix", NULL, gnssFix_cb, NULL, 0);
        LOGGER_INFO(kModuleName,"gnss_fix invoked");
        return {};
    }
    else
    {
        return std::unexpected(ret_code::GENERIC);
    }  
} */

void timeout_set();
void reset_mon_timer(int msecs);

std::vector<unsigned char> buildWialonBinary(const GPSData& currGps);
void sendWialonBinary(const std::vector<unsigned char>& packet);

void monProcess();