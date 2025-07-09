#include <stdlib.h>
#include <cmath>
#include <numbers>
#include <iomanip>
#include <sstream>

#include <Logger/Logger.h>

extern "C" {
#include <ubox/uloop.h>
#include <ubox/ustream.h>
#include <ubox/utils.h>
#include <ubus/libubus.h>
#include <ubox/blobmsg_json.h>
#include <json-c/json.h>
}

#include <ubus_base.h>
#include <ubus_policy.h>
#include <ubus_events.h>
#include <ret_code.h>
#include "GPSData.h"
#include "PacketBuilder.h"
#include "defines.h"
#include "config.h"

#include "config.h"
#include "mon.h"
#include "tcp.h"

// Function to the hex string for console output
std::string to_hex(const std::vector<unsigned char>& data) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (unsigned char byte : data) {
        oss << std::setw(2) << static_cast<int>(byte);
    }
    return oss.str();
}

static TcpClient tcp_client("212.33.235.243", 65262);

static uint32_t id;

GPSData currGps;
GPSData prevGps;

GPSData& get_currGps()
{
    return currGps;
}

GPSData& get_prevGps()
{
    return prevGps;
}

static bool prevGpsInitialized = false;

/*  
    the logic for the course that 
    will respond to its change if 
    it exceeds the delta
*/
bool bearingChanged()
{
    auto delta_bearing = getDeltaBearing();
    return std::abs(static_cast<int32_t>(prevGps.bearing) - static_cast<int32_t>(currGps.bearing)) > delta_bearing;
}

double haversine_distance(double lat1, double lon1, double lat2, double lon2) {
    const double deg_to_rad = std::numbers::pi / 180.0;

    lat1 *= deg_to_rad;
    lon1 *= deg_to_rad;
    lat2 *= deg_to_rad;
    lon2 *= deg_to_rad;

    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;

    double a = std::sin(dlat / 2) * std::sin(dlat / 2) +
               std::cos(lat1) * std::cos(lat2) *
               std::sin(dlon / 2) * std::sin(dlon / 2);

    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

    return EARTH_RADIUS * c;
}

/*  
    the logic for the distance(Lat & Lon) that 
    will respond to its change if it exceeds the delta
*/
bool distanceChanged()
{
    auto delta_distance = getDeltaDistace();

    double prev_lat = static_cast<double>(prevGps.latitude) / 1e16;
    double prev_lon = static_cast<double>(prevGps.longitude) / 1e16;
    double curr_lat = static_cast<double>(currGps.latitude) / 1e16;
    double curr_lon = static_cast<double>(currGps.longitude) / 1e16;

    double distance = haversine_distance(prev_lat, prev_lon, curr_lat, curr_lon);

    return distance > delta_distance;
}

Status findUbusService(std::string_view service)
{
    if (!get_ubus_ctx())
    {
        LOGGER_INFO(kModuleName, "findUbusService::Failed to connect to ubus");
        return std::unexpected(ret_code::GENERIC);
    }

    if (ubus_lookup_id(get_ubus_ctx(), service.data(), &id))
    {
        LOGGER_INFO(kModuleName, "findUbusService::Failed to look up {:s} in ubus", service.data());
        return std::unexpected(ret_code::GENERIC);
    }
    LOGGER_INFO(kModuleName, "findUbusService::ok -- {:s}", service.data());
    return {};
}

static void mon_cb(struct uloop_timeout *timeout)
{
    monProcess();
    return;
}

struct uloop_timeout mon_timer = {
    .cb = mon_cb,
};

/*
    timer if the course or distance exceeds its deltas
*/
void timeout_set()
{
    uloop_timeout_set(&mon_timer, 10);
}

/*
    timer for MON_PERIOD and RETRY_PERIOD
*/
void reset_mon_timer(int msecs)
{
    uloop_timeout_set(&mon_timer, msecs);
}

std::vector<unsigned char> buildWialonBinary(const GPSData& currGps)
{
    auto packet = currGps.makeWialonBinary();
    if (packet.empty()) {
        LOGGER_INFO(kModuleName, "buildWialonBinary: Generated empty packet.");
    }
    return packet;
}

/*
    connecting, sending a login package, sending a GPS data package
*/
void sendWialonBinary(const std::vector<unsigned char>& packet) {
    if (packet.empty()) {
        LOGGER_INFO(kModuleName, "sendWialonBinary: Empty packet");
        return;
    }
    static bool login_sent = false;

    if (!tcp_client.isConnected()) {
        if (tcp_client.connectToServer()) {
            LOGGER_INFO(kModuleName, "Successfully connected to the server!");
        } else {
            LOGGER_ERROR(kModuleName, "Failed to connect to the server.");
            return;
        }
    }

    if (!login_sent) {
        if (tcp_client.sendLoginPacket()) {
            LOGGER_INFO(kModuleName, "Login packet sent successfully.");

            std::vector<unsigned char> response_buffer(1024);
            if (tcp_client.receiveData(response_buffer, 1024)) {
                std::string hex_response = to_hex(response_buffer);
                LOGGER_INFO(kModuleName, "Server response after login (hex): {}", hex_response);
            } else {
                LOGGER_ERROR(kModuleName, "Failed to receive response after login.");
            }

            login_sent = true;
        } else {
            LOGGER_ERROR(kModuleName, "Failed to send login packet.");
            tcp_client.disconnectFromServer();
            return;
        }
    }

    std::string hex_packet = to_hex(packet);
    LOGGER_INFO(kModuleName, "GPS Packet (hex): {}", hex_packet);

    if (tcp_client.sendData(packet)) {
        std::vector<unsigned char> response_buffer(1024);
        if (tcp_client.receiveData(response_buffer, 1024)) {
            std::string hex_response = to_hex(response_buffer);
            LOGGER_INFO(kModuleName, "Server response after data (hex): {}", hex_response);
        } else {
            LOGGER_ERROR(kModuleName, "Failed to receive response after data.");
        }
    } else {
        LOGGER_ERROR(kModuleName, "Failed to send Wialon binary packet.");
        login_sent = false;
    }
}

void monProcess()
{
    if (currGps.flags == 31)
    {
        if (!prevGpsInitialized)
        {
            prevGps = currGps;
            prevGpsInitialized = true;
        }
        auto packet = buildWialonBinary(currGps); 
        sendWialonBinary(packet);
        prevGps = currGps;
        reset_mon_timer(getMonPeriod());
    }
    else
    {
        reset_mon_timer(getRetryPeriod());
    }
}