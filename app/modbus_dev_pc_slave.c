 /**
  *************************************************************************************************
  * @file name : c_template.c
  * @encoding  : UTF-8
  * @author    : HDY(803098)
  * @version   : V0.0.1
  * @date      : 2025.1.1
  * @brief     : This file provides
  * @attention 
  * Copyright (c) 2024 HOYPower.
  * All rights reserved.
  *************************************************************************************************
  * @Edit History
  * 时间       >> 版本   >> 姓名（工号） >> 修改内容
  * 2025.12.12 >> V0.0.1 >> zzy(801922) >> 创建文件
  *************************************************************************************************
  */
/* Includes -------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../src/modbus.h"
#include "../src/modbus_rtu.h"
#include "../src/modbus_port.h"
/* Private define -------------------------------------------------------------------------------*/

/* Private typedef ------------------------------------------------------------------------------*/
/* Private functions prototypes -----------------------------------------------------------------*/
int8_t uart_pc_send(uint8_t *buff, uint16_t len);
int8_t uart_pc_recv(uint8_t *buff, uint16_t len);
int8_t rtu_pc_read_input(stModbus_RTU_InputReader *reader);
int8_t rtu_pc_read_hold(stModbus_RTU_HoldReader *reader);
int8_t rtu_pc_write_hold(stModbus_RTU_HoldWriter *writer);


/* Private variable -----------------------------------------------------------------------------*/
stModebus_RTU_Fun_Table rtu_fun_table_pc[] =
{
    {0x03, modbus_fun_response_03, modbus_fun_request_03, NULL, NULL},                // std: read hold 
    {0x04, NULL, NULL, NULL, NULL},                                                     // std: read input
    {0x06, NULL, NULL, NULL, NULL},                                                     // std: wirte single hold
    {0x10, NULL, NULL, NULL, NULL},                                                     // std: write multi  hold

};
#define RTU_FUN_TABLE_ITEMS (sizeof(rtu_fun_table_pc) / sizeof(stModebus_RTU_Fun_Table))

stModbus_RTU_Handler stModbus_RTU_Handler_PC ;
stModbus_RTU_Handler_Attr rtu_pc_attr = {
    .dev_addr = 1,
    .mode = emModebus_RTU_Mode_Slave,
    .reg_map_id = 0,
    .send = uart_pc_send,
    .recv = uart_pc_recv,
    .read_input = rtu_pc_read_input,
    .read_hold = rtu_pc_read_hold,
    .write_hold = rtu_pc_write_hold,
    .fun_table = rtu_fun_table_pc,
    .fun_table_items = RTU_FUN_TABLE_ITEMS,
};



/* Global  variable -----------------------------------------------------------------------------*/
/* Extern  variable -----------------------------------------------------------------------------*/
/* Function body --------------------------------------------------------------------------------*/
/**
  *************************************************************************************************
  * @Function    : main
  * @Description : 
  * @Parameter   : 
  * @Return      : 
  * @Author      : zzy(801922)
  * @Date        : 2024.11.24
  *************************************************************************************************
  * @Edit History
  * 时间       >> 姓名（工号）>> 修改内容
  * 2025.12.12 >> zzy(801922) >> 创建函数
  *************************************************************************************************
  */





  int8_t uart_pc_send(uint8_t *buff, uint16_t len)
  {
  
  }
  int8_t uart_pc_recv(uint8_t *buff, uint16_t len)
  {
      readline(buff);
  }
  


  int8_t rtu_pc_read_input(stModbus_RTU_InputReader *reader)
  {

  }
  int8_t rtu_pc_read_hold(stModbus_RTU_HoldReader *reader)
  {

  }
  int8_t rtu_pc_write_hold(stModbus_RTU_HoldWriter *writer)
  {

  }

int8_t modbus_dev_pc_slave_init()
{
    modbus_rtu_init(&stModbus_RTU_Handler_PC,  emModebus_RTU_Bus_PC, &rtu_pc_attr);
}

int8_t modbus_dev_pc_slave_run()
{
    modbus_rtu_run(&stModbus_RTU_Handler_PC);
}





/******************************************* END OF FILE *****************************************/
