#include "./modbus_rtu.h"


#ifndef _MODBUS_RTU_MASTER_H_
#define _MODBUS_RTU_MASTER_H_
void modbus_rtu_master(stModbus_RTU_Handler_def *handler);

int8_t modbus_fun_request_03(stModbus_RTU_Handler_def *handler, stModbus_RTU_Sender_def *sender);
int8_t modbus_fun_parse_03_master(stModbus_RTU_Handler_def *handler, uint8_t *buff, uint16_t len);

int8_t modbus_fun_request_04(stModbus_RTU_Handler_def *handler, stModbus_RTU_Sender_def *sender);
int8_t modbus_fun_parse_04_master(stModbus_RTU_Handler_def *handler, uint8_t *buff, uint16_t len);

int8_t modbus_fun_request_06(stModbus_RTU_Handler_def *handler, stModbus_RTU_Sender_def *sender);
int8_t modbus_fun_parse_06_master(stModbus_RTU_Handler_def *handler, uint8_t *buff, uint16_t len);

int8_t modbus_fun_request_10(stModbus_RTU_Handler_def *handler, stModbus_RTU_Sender_def *sender);
int8_t modbus_fun_parse_10_master(stModbus_RTU_Handler_def *handler, uint8_t *buff, uint16_t len);

/**
 * @brief 读取寄存器
 * @param bus 总线编号
 * @param dev_addr 设备地址
 * @param usReg_addr 寄存器地址
 * @param ucReg_num 寄存器数量
 * @param reg_data 寄存器数据地址
 * 
 * @return 0 成功，-1 失败
 */
int8_t modbus_rtu_read_hold(eModebus_RTU_Bus_def bus, uint8_t dev_addr, uint16_t usReg_addr, uint16_t ucReg_num, uint16_t *reg_data);

int8_t modbus_rtu_read_input(eModebus_RTU_Bus_def bus, uint8_t dev_addr, uint16_t usReg_addr, uint16_t ucReg_num, uint16_t *output);

int8_t modbus_rtu_write_hold(eModebus_RTU_Bus_def bus, uint8_t dev_addr, uint16_t usReg_addr, uint16_t ucReg_num, uint16_t *reg_data);

/**
 * @brief 读取上次操作完成状态
 * @param bus 总线编号
 * 
 * @return 0 完成，1 未完成　-１失败
 */
int8_t modbus_rtu_opt_status(eModebus_RTU_Bus_def bus);
#endif