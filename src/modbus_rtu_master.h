#include "./modbus_rtu.h"


#ifndef _MODBUS_RTU_MASTER_H_
#define _MODBUS_RTU_MASTER_H_




void modbus_rtu_master(stModbus_RTU_Handler *handler);
int8_t modbus_fun_request_03(stModbus_RTU_Handler *handler, stModbus_RTU_Sender *sender);
int8_t modbus_fun_parse_03_master(stModbus_RTU_Handler *handler, uint8_t *buff, uint16_t len);

#endif