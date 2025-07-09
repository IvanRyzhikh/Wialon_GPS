#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef SYSSVCD_DEBUG_H
#define SYSSVCD_DEBUG_H

struct UbusCounters  {
    uint32_t ubusMethod;
    uint32_t ubusMethodErr;
};

#endif //SYSSVCD_DEBUG_H