 /**
  *************************************************************************************************
  * @file name : c_template.h
  * @encoding  : UTF-8
  * @author    : HDY(803098)
  * @date      : 2025.1.1
  * @brief     : This file provides
  * @attention 
  * Copyright (c) 2024 HOYPower.
  * All rights reserved.
  *************************************************************************************************
  */
#ifndef __MODBUS_H__
#define __MODBUS_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdio.h>
#include "modbus_config.h"
#if ENABLE_PRINTF
#define mylog(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define mylog(fmt, ...)
#endif

/* Includes -------------------------------------------------------------------------------------*/
#include <stdint.h>
//#include "system_hoy.h"
#include "./modbus_rtu.h"
/* Exported defines -----------------------------------------------------------------------------*/

/* Exported typedef -----------------------------------------------------------------------------*/



/*Function code definition*/











/* Exported functions prototypes ----------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __C_TEMPLATE_H__ */
/******************************************* END OF FILE *****************************************/
