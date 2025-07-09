#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include <Logger/Logger.h>

// #include "mcu.h"
// #include "mcu_io_svc.h"
// #include "mcu_config_svc.h"
// #include "mcu_system_svc.h"
// #include "mcu_proto.h"

extern "C" {
#include <ubox/uloop.h>
#include <ubox/ustream.h>
#include <ubox/utils.h>
#include <ubus/libubus.h>
#include <json-c/json.h>
#include <ubox/blobmsg_json.h>
}

#include <magic_enum/magic_enum.hpp>

#include <law/UART/RS232/RS232.h>

#include <uci/uci.h>

#include "defines.h"
#include "ubus_events.h"
#include "ubus_methods.h"
#include "mon.h"

static struct ubus_context *ubus_ctx = NULL;
static struct uloop_fd tty_socket;

struct ubus_context * get_ubus_ctx(void)
{
    return ubus_ctx;
}

/**********************************************************************\
*   Function:       ubus_init
*   Description:    init UBUS context
*   Returns:        0 on success
\**********************************************************************/
int ubus_init(const char * path)
{
    static int first = 0;

	uloop_init();

	while (1)
	{
		ubus_ctx = ubus_connect(path);
		if (ubus_ctx == NULL)
		{
            signal(SIGINT, SIG_DFL);
			if (!first) 
			{
				printf("Failed to connect to ubus! Waiting connect..\n");
				first = 1;
			}			
			usleep(200000); //wait for next try
		}
		else 
			break;
	}

    ubus_add_uloop(ubus_ctx);

	printf("Ubus init OK\n");
    signal(SIGINT, SIG_DFL);
    return 0;
}

int ubus_main(void)
{
	static constexpr std::string_view kModuleName = "ubus_main";

	int ret = ubus_add_object(ubus_ctx, &service_object);
	if (ret)
	{
		fprintf(stderr, "Failed to add object: %s\n", ubus_strerror(ret));
		return -1;
	}
	
	if (ubus_register_events(ubus_ctx))
	{
		fprintf(stderr, "Failed to register events\n");
		return -1;
	}

    send_start_event();
    //Logger::Logger::getInstance().setCurrentPriority(Logger::MessagePriority::WARNING);

	monProcess();

	uloop_run();

	ubus_free(ubus_ctx);
	uloop_done();

    return 0;
}

