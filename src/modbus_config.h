#ifndef _MODBUS_CONFIG_H_
#define _MODBUS_CONFIG_H_

#define ENABLE_PRINTF           1

#define DEFAULT_MASTER_RECEVIE_TIMEOUT       500





























#if ENABLE_PRINTF
#include <stdio.h>
#define mylog(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define mylog(fmt, ...)
#endif


#endif