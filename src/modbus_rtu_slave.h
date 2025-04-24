#include "./modbus_rtu.h"


#ifndef _MODBUS_RTU_SLAVE_H_
#define _MODBUS_RTU_SLAVE_H_

void modbus_rtu_slave(stModbus_RTU_Handler *handler);
int8_t modbus_fun_parse_slave_03(stModbus_RTU_Handler *handler, uint8_t *buff, uint16_t len);
int8_t modbus_fun_parse_slave_06(stModbus_RTU_Handler *handler, uint8_t *buff, uint16_t len);

#endif