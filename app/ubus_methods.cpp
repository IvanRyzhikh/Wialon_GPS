#include <unistd.h>
#include <signal.h>

#include "defines.h"
#include "ubus_base.h"
#include "ubus_methods.h"
#include "main.h"
#include "system.h"
#include <Logger/Logger.h>
#include "ret_code.h"
extern "C" {
#include <ubox/uloop.h>
#include <ubox/ustream.h>
#include <ubox/utils.h>
#include <ubus/libubus.h>
#include <json-c/json.h>
#include <ubox/blobmsg.h>
#include <ubox/blobmsg_json.h>
}
#include "ubus_policy.h"
#include "debug.h"

UbusCounters DebugCounters;

//static constexpr std::string_view kModuleName2 = "audio_test";
static int hello_method(struct ubus_context *ctx, struct ubus_object *obj,
    struct ubus_request_data *req, const char *method,
    struct blob_attr *msg)
{
    struct blob_attr *tb[__HELLO_MAX];

    blobmsg_parse(hello_method_policy, ARRAY_SIZE(hello_method_policy), tb, blob_data(msg), blob_len(msg));

    // if there was a message present, print it.
    if (tb[HELLO_ID]) {
        printf ("Recieved ID: %d\n", blobmsg_get_u32(tb[HELLO_ID]));
    }

    if (tb[HELLO_MSG]) {
        printf ("Received %s\n", blobmsg_get_string(tb[HELLO_MSG]));
    }

    return 0;
}

static struct blob_buf b;

static int version_method(struct ubus_context *ctx, struct ubus_object *obj,
                          struct ubus_request_data *req, const char *method,
                          struct blob_attr *msg)
{
    char temp[64];

    blob_buf_init(&b, 0);

    std::string ver_h = std::to_string(VERSION_MAJOR);
    std::string ver_l = std::to_string(VERSION_MINOR);

    std::string res = ver_h + "." + ver_l;
    blobmsg_add_string(&b, "version", res.c_str());

    memset(temp, 0, sizeof(temp));
    strcpy(temp, kBuildDate);
    strcat(temp," ");
    strcat(temp, kBuildTime);
    blobmsg_add_string(&b, "build", temp);

    ubus_send_reply(ctx, req, b.head);

    return 0;
}

struct uloop_process process = {0};

void my_process_handler(struct uloop_process *c, int ret)
{
    //while (1)
    {
        printf("child ret = %d\n", ret);
        //sleep(1);
    }
}

static void start_event_cb(struct uloop_timeout *timeout)
{
    printf("start event \n");

    pid_t pid = fork();

    if(pid == 0)
    {
        printf("child %d\n",getpid());
        while (1)
        {
            sleep(1);
            printf("child %d\n",getpid());
        }
        // exit(0);
        // process.pid = getpid();
        // process.cb = my_process_handler;

        // uloop_process_add(&process);

        //exit(0);
    }
    else {
        printf("parent %d \n",getpid());
    process.pid = pid;
    process.cb = my_process_handler;

    uloop_process_add(&process);
    }

}

static int ext_pwr_down(struct ubus_context *ctx, struct ubus_object *obj,
    struct ubus_request_data *req, const char *method,
    struct blob_attr *msg)
{
    blob_buf_init(&b, 0);
    blobmsg_add_string(&b, "test_mode", "ext_pwr_down");
    ubus_send_reply(ctx, req, b.head);
    
    // notifyExtPwrDown();

    return 0;
}

static void kill_cb(struct uloop_timeout *timeout)
{
    printf("Service \"%s\" killed!\n", SERVICE_NAME);
    uloop_end();
    return;
}

static struct uloop_timeout kill_timer = {
	.cb = kill_cb,
};


static int kill_method(struct ubus_context *ctx, struct ubus_object *obj,
    struct ubus_request_data *req, const char *method,
    struct blob_attr *msg)
{
    //monLogFileClose();
    blob_buf_init(&b, 0);

    blobmsg_add_u16(&b, "return", UBUS_RETURN_OK);
    blobmsg_add_string(&b, "msg", "OK");
    
    ubus_send_reply(ctx, req, b.head);

    uloop_timeout_set(&kill_timer, 500);
    return 0;
}

static int reinit_method(struct ubus_context *ctx, struct ubus_object *obj,
    struct ubus_request_data *req, const char *method,
    struct blob_attr *msg)
{
    int ret = init_main();
    
    blob_buf_init(&b, 0);

    if (!ret)
    {
        blobmsg_add_u16(&b, "return", UBUS_RETURN_OK);
        blobmsg_add_string(&b, "msg", "OK");
    }
    else {
        blobmsg_add_u16(&b, "return", UBUS_RETURN_ERROR);
        blobmsg_add_string(&b, "msg", "ERROR");
    }
    
    ubus_send_reply(ctx, req, b.head);

    return 0;
}

static int uptime_method(struct ubus_context *ctx, struct ubus_object *obj,
    struct ubus_request_data *req, const char *method,
    struct blob_attr *msg)
{
    uint64_t uptime = (get_current_time_ms() - get_start_time())/1000;

    //int ret = init_main();

    blob_buf_init(&b, 0);

    blobmsg_add_u64(&b, "uptime seconds", uptime);

    ubus_send_reply(ctx, req, b.head);

    return 0;
}

static int get_last_fix_method(struct ubus_context *ctx, struct ubus_object *obj,
                          struct ubus_request_data *req, const char *method,
                          struct blob_attr *msg)
{
    blob_buf_init(&b, 0);
    uint32_t timestamp = 0;
    //timestamp = getTimestamp();
    blobmsg_add_u32(&b, "Timestamp", timestamp);
    // ...
    ubus_send_reply(ctx, req, b.head);

    return 0;
}

static int set_debug_level_method(struct ubus_context *ctx, struct ubus_object *obj,
                                  struct ubus_request_data *req, const char *method,
                                  struct blob_attr *msg)
{
    struct blob_attr *tb[__DEBUG_LEVEL_MAX];
    uint32_t var, value;

    blobmsg_parse(set_debug_level_method_policy, ARRAY_SIZE(set_debug_level_method_policy), tb, blob_data(msg), blob_len(msg));

    // if there was a message present, print it.
    if (tb[DEBUG_LEVEL]) {
        var = blobmsg_get_u32(tb[DEBUG_LEVEL]);
        LOGGER_WARNING(SERVICE_NAME,"{:s}> Recieved debug level {:d}",kModuleName, var);
        Logger::Logger::getInstance().setCurrentPriority(Logger::MessagePriority(var));
    }

    return 0;
}

static int get_debug_level_method(struct ubus_context *ctx, struct ubus_object *obj,
                                  struct ubus_request_data *req, const char *method,
                                  struct blob_attr *msg)
{
    static struct blob_buf b;

    blob_buf_init(&b, 0);

    uint32_t var = static_cast<uint32_t>(Logger::Logger::getInstance().getCurrentPriority());

    blobmsg_add_u32(&b, "debug_level", var);
    ubus_send_reply(ctx, req, b.head);

    return 0;
}

static int get_debug_counts_method(struct ubus_context *ctx, struct ubus_object *obj,
                                  struct ubus_request_data *req, const char *method,
                                  struct blob_attr *msg)
{
    static struct blob_buf b;

    blob_buf_init(&b, 0);

    blobmsg_add_u32(&b, "ubusMcuState", DebugCounters.ubusMethod);
    blobmsg_add_u32(&b, "ubusMcuStateErr", DebugCounters.ubusMethodErr);
    // ...
    ubus_send_reply(ctx, req, b.head);

    return 0;
}

static int clear_debug_counts_method(struct ubus_context *ctx, struct ubus_object *obj,
                                   struct ubus_request_data *req, const char *method,
                                   struct blob_attr *msg)
{
    static struct blob_buf b;

    blob_buf_init(&b, 0);

    DebugCounters.ubusMethod = 0;
    DebugCounters.ubusMethodErr = 0;
    // ...

    blobmsg_add_string(&b, "msg", "OK");
    ubus_send_reply(ctx, req, b.head);

    return 0;
}

static const struct ubus_method service_methods[] = {
	UBUS_METHOD("hello", hello_method, hello_method_policy),
    UBUS_METHOD_NOARG("version", version_method),
    UBUS_METHOD_NOARG("kill", kill_method),
    UBUS_METHOD_NOARG("reinit", reinit_method),
    UBUS_METHOD_NOARG("uptime", uptime_method),
    UBUS_METHOD_NOARG("ext_pwr_down", ext_pwr_down),
    UBUS_METHOD("set_debug_level", set_debug_level_method, set_debug_level_method_policy),
    UBUS_METHOD_NOARG("get_debug_level", get_debug_level_method),
    UBUS_METHOD_NOARG("get_debug_counts", get_debug_counts_method),
    UBUS_METHOD_NOARG("clear_debug_counts", clear_debug_counts_method)
};

static struct ubus_object_type service_object_type =
	UBUS_OBJECT_TYPE("methods", service_methods);

struct ubus_object service_object = {
	.name = SERVICE_NAME,
	.type = &service_object_type,
	.methods = service_methods,
	.n_methods = ARRAY_SIZE(service_methods),
};


