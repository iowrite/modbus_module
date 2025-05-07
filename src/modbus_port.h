#include <stdint.h>

#ifndef __MODBUS_PORT_H__
#define __MODBUS_PORT_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes -------------------------------------------------------------------------------------*/

/* Exported defines -----------------------------------------------------------------------------*/

/* Exported typedef -----------------------------------------------------------------------------*/

typedef enum Modebus_RTU_Bus
{
    eModebus_RTU_Bus_Start,
    eModebus_RTU_Bus_PC = 1,

    eModebus_RTU_Bus_End,

}eModebus_RTU_Bus_def;



typedef enum Gateway_Host_Protocol_Res
{
    RES_FAILED              = 0,
    RES_SUCCESS             = 1,
    RES_PASSTHROUGH         = 2
    
}eGateway_Host_Protocol_Res_def;

/*Function code definition*/

/* Exported functions prototypes ----------------------------------------------------------------*/

uint32_t modbus_port_get_time_ms();


#ifdef __cplusplus
}
#endif

#endif
/******************************************* END OF FILE *****************************************/
