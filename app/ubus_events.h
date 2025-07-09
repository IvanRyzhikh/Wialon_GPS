#ifndef _UBUS_EVENTS_H_
#define _UBUS_EVENTS_H_
#include <string>
#include <string_view>
#include <cstdint>
void ubus_gnss_event(struct ubus_context *ctx, struct ubus_event_handler *ev,
                             const char *type, struct blob_attr *msg);
void ubus_ignition_event(struct ubus_context *ctx, struct ubus_event_handler *ev,
                             const char *type, struct blob_attr *msg);
int ubus_register_events(struct ubus_context *ctx);
int send_start_event(void);

#endif
