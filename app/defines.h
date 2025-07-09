#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <law/law.h>

#define SERVICE_NAME    "mon"

#define SERVICE_CONFIG "/etc/config/mon_config"
#define SERVICE_NAME_CONFIG "mon"


#define CONFIG_LOAD_ATTEMPTS    2
#define CONFIG_LOAD_ATTEMPT_TIMEOUT 2 //sec

#define VERSION_MAJOR   0
#define VERSION_MINOR   1

static constexpr std::string_view kModuleName = "mon";

#endif