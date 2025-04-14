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
  * 2025.12.12 >> V0.0.1 >> HDY(8030987) >> 创建文件
  *************************************************************************************************
  */
/* Includes -------------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h> 
#include <stdio.h>
#include "./modbus.h"
#include "./modbus_rtu.h"
#include "./modbus_rtu_slave.h"
#include "./modbus_rtu_master.h"
#include "./modbus_port.h"
/* Private define -------------------------------------------------------------------------------*/

/* Private typedef ------------------------------------------------------------------------------*/



/* Private functions prototypes -----------------------------------------------------------------*/



int8_t modbus_fun_request_03(stModbus_RTU_Handler *pstModbus_RTU_Handler, stModbus_RTU_Sender *sender);
/* Private variable -----------------------------------------------------------------------------*/

stModbus_Interface_Bind stModbus_Interface_Bind_Table[10];






/* Global  variable -----------------------------------------------------------------------------*/

/* Extern  variable -----------------------------------------------------------------------------*/

/* Function body --------------------------------------------------------------------------------*/

/**
  *************************************************************************************************
  * @Function    : main
  * @Description : 
  * @Parameter   : 
  * @Return      : 
  * @Author      : HDY(803098)
  * @Date        : 2024.11.24
  *************************************************************************************************
  * @Edit History
  * 时间       >> 姓名（工号）>> 修改内容
  * 2025.12.12 >> HDY(803098) >> 创建函数
  *************************************************************************************************
  */







void modbus_rtu_run(stModbus_RTU_Handler *handler)
{
    handler->last_call_tick = modbus_port_get_tick();

    if(handler->mode == emModebus_RTU_Mode_Master)
    {
        modbus_rtu_master(handler);
    }else if(handler->mode == emModebus_RTU_Mode_Slave){
        modbus_rtu_slave(handler);
    }

}




int8_t modbus_rtu_set_mode(stModbus_RTU_Handler *handler, emModebus_RTU_Mode mode)
{
    handler->mode = mode;
}
















int8_t modbus_rtu_send(stModbus_RTU_Handler *handler, stModbus_RTU_Sender sender)
{
    stModebus_RTU_Fun_Table match_item = {0};
    for(int i = 0; i < handler->fun_table_items; i++)
    {
        if(handler->fun_table[i].fcode == sender.fun_code)
        {
            match_item.fcode = handler->fun_table[i].fcode;
            match_item.request = handler->fun_table[i].request;
            break;
        }
    }

    match_item.request(handler, &sender);

}




int8_t modbus_rtu_opt_status(emModebus_RTU_Bus bus)
{
    if(bus == 0)
    {
        return -1;
    }
    stModbus_RTU_Handler *handler = NULL;
    for(int i = 0; i < MODBUS_INTERFACE_BIND_TABLE_ITEMS; i++)
    {
        if(stModbus_Interface_Bind_Table[i].bus == bus)
        {
            handler = stModbus_Interface_Bind_Table[i].handler;
            break;
        }
    }
    if(handler->mode == emModebus_RTU_Mode_Slave)
    {
        return -1;
    }
    if(handler->last_state == emModbus_RTU_State_Receive && handler->state == emModbus_RTU_State_Send)
    {
        return 0;
    }
}

/*
函数名：Modbus_CRC16
作者：詹候林
日期：2024-8
功能：modbus接收数据解析
返回参数：无
修改记录：
*/
static const uint8_t ucCrc16_HighTable[256] =
{
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

static const uint8_t ucCrc16_LowTable[256] =
{
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

uint16_t modbus_crc_cal(uint8_t *pData, uint32_t Size)
{
    uint8_t ucCRCHi = 0xFFU;
    uint8_t ucCRCLo = 0xFFU;
    uint8_t ucIndex = 0U;

    if ((NULL == pData) || (0U == Size))
    {
        return 0;
    }

    while (Size--)
    {
        ucIndex = ucCRCLo ^ *pData++;
        ucCRCLo = ucCRCHi ^ ucCrc16_HighTable[ucIndex];
        ucCRCHi = ucCrc16_LowTable[ucIndex];
    }
    uint16_t crc = (uint16_t)(((uint16_t)ucCRCLo << 8)|ucCRCHi);
    return crc;
}







int8_t modbus_rtu_init(stModbus_RTU_Handler *handler, emModebus_RTU_Bus bus, stModbus_RTU_Handler_Attr *attr)
{
    if(bus = 0)
    {
        return -1;
    }
    for(int i = 0; i < MODBUS_INTERFACE_BIND_TABLE_ITEMS; i++)
    {
        if(stModbus_Interface_Bind_Table[i].bus == 0)
        {
            stModbus_Interface_Bind_Table[i].bus = bus;
            stModbus_Interface_Bind_Table[i].handler = handler;
            break;
        }
    }
    handler->state = emModbus_RTU_State_init;
    handler->last_state = handler->state;
    handler->dev_addr = attr->dev_addr;
    handler->mode = attr->mode;
    handler->last_mode = handler->mode;
    handler->reg_map_id = attr->reg_map_id;
    handler->send = attr->send;
    handler->recv = attr->recv;
    handler->read_input = attr->read_input;
    handler->read_hold = attr->read_hold;
    handler->write_hold = attr->write_hold;
    handler->fun_table = attr->fun_table;
    handler->fun_table_items = attr->fun_table_items;

}



int8_t modbus_rtu_set_send(emModebus_RTU_Bus bus, int8_t (*send)(uint8_t *, uint16_t))
{
    if(bus == 0)
    {
        return -1;
    }
    stModbus_RTU_Handler *handler = NULL;
    for(int i = 0; i < MODBUS_INTERFACE_BIND_TABLE_ITEMS; i++)
    {
        if(stModbus_Interface_Bind_Table[i].bus == bus)
        {
            handler = stModbus_Interface_Bind_Table[i].handler;
            break;
        }
    }
    handler->send = send;
}

int8_t modbus_rtu_set_recv(emModebus_RTU_Bus bus, int8_t (*recv)(uint8_t *, uint16_t *))
{
    if(bus == 0)
    {
        return -1;
    }
    stModbus_RTU_Handler *handler = NULL;
    for(int i = 0; i < MODBUS_INTERFACE_BIND_TABLE_ITEMS; i++)
    {
        if(stModbus_Interface_Bind_Table[i].bus == bus)
        {
            handler = stModbus_Interface_Bind_Table[i].handler;
            break;
        }
    }
    handler->recv = recv;
}

int8_t modbus_rtu_set_dev_addr(emModebus_RTU_Bus bus, int8_t dev_addr)
{
    if(bus == 0)
    {
        return -1;
    }
    stModbus_RTU_Handler *handler = NULL;
    for(int i = 0; i < MODBUS_INTERFACE_BIND_TABLE_ITEMS; i++)
    {
        if(stModbus_Interface_Bind_Table[i].bus == bus)
        {
            handler = stModbus_Interface_Bind_Table[i].handler;
            break;
        }
    }
    handler->dev_addr = dev_addr;
}







/******************************************* END OF FILE *****************************************/
