#include "./modbus_rtu.h"

#ifndef _MODBUS_RTU_SLAVE_H_
#define _MODBUS_RTU_SLAVE_H_
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes -------------------------------------------------------------------------------------*/

/* Exported defines -----------------------------------------------------------------------------*/

/* Exported typedef -----------------------------------------------------------------------------*/

/* Exported functions prototypes ----------------------------------------------------------------*/
void modbus_rtu_slave(stModbus_RTU_Handler_def *handler);
int8_t modbus_fun_parse_slave_03(stModbus_RTU_Handler_def *handler, uint8_t *buff, uint16_t len);
int8_t modbus_fun_parse_slave_04(stModbus_RTU_Handler_def *handler, uint8_t *buff, uint16_t len);
int8_t modbus_fun_parse_slave_06(stModbus_RTU_Handler_def *handler, uint8_t *buff, uint16_t len);
int8_t modbus_fun_parse_slave_10(stModbus_RTU_Handler_def *handler, uint8_t *buff, uint16_t len);

#ifdef __cplusplus
}
#endif
#endif