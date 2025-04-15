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
#ifndef __MODBUS_PORT_H__
#define __MODBUS_PORT_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes -------------------------------------------------------------------------------------*/
#include <stdint.h>
//#include "system_hoy.h"
/* Exported defines -----------------------------------------------------------------------------*/

/* Exported typedef -----------------------------------------------------------------------------*/
typedef enum Modebus_RTU_Bus
{
    emModebus_RTU_Bus_Start,
    emModebus_RTU_Bus_PC = 1,

    emModebus_RTU_Bus_End,

}emModebus_RTU_Bus;



typedef enum
{
    RES_FAILED              = 0,
    RES_SUCCESS             = 1,
    RES_PASSTHROUGH         = 2
    
}GATEWAY_HOST_PROTOCOL_RES_T;

/*Function code definition*/











/* Exported functions prototypes ----------------------------------------------------------------*/


uint32_t modbus_port_get_time_ms();


#ifdef __cplusplus
}
#endif

#endif /* __C_TEMPLATE_H__ */
/******************************************* END OF FILE *****************************************/
