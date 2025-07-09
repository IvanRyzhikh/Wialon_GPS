#include <Logger/Logger.h>

extern "C" {
#include <ubox/uloop.h>
#include <ubox/ustream.h>
#include <ubox/utils.h>
#include <ubus/libubus.h>
#include <json-c/json.h>
#include <ubox/blobmsg_json.h>
}

#include <uci/uci.h>

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include "defines.h"
#include "ubus_base.h"
#include "ubus_events.h"
#include "ubus_methods.h"
#include "system.h"
#include "config.h"
#include "tcp.h"
#include "mon.h"

static Settings settings;

static uint64_t start_ts = 0;

uint64_t get_start_time(void)
{
	return start_ts;
}

int init_main(void)
{
	make_default_config(settings);
	if (load_config(settings))
	{
		return -1;
	}

	apply_config(settings);
	return 0;
}

static void service_main(void)
{
    int ret;
    if (init_main())
    {
        fprintf(stderr, "Service \"%s\" init failed!\n", SERVICE_NAME);
        return;
    }

    start_ts = get_current_time_ms();

    if (ubus_main())
    {
        fprintf(stderr, "Service \"%s\" ubus start failed!\n", SERVICE_NAME);
        return;
    }
}

int main(int argc, char **argv)
{
    const char *ubus_socket = "/var/run/ubus.sock";
    int ch;

    while ((ch = getopt(argc, argv, "cs:")) != -1) {
        switch (ch) {
        case 's':
            ubus_socket = optarg;
            break;
        default:
            break;
        }
    }

    printf("Start service: \"%s\"\n", SERVICE_NAME);
    printf("Use ubus socket path: %s\n", ubus_socket);

    ubus_init(ubus_socket);
    service_main();

    return 0;
}
