#ifndef _UBUS_BASE_H_
#define _UBUS_BASE_H_

enum {
    UBUS_RETURN_OK,
    UBUS_RETURN_ERROR
};

struct ubus_context * get_ubus_ctx(void);
int ubus_init(const char * path);
int ubus_main(void);


#endif