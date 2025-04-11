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
#include "./modbus.h"
#include "./modbus_rtu.h"
#include "./modbus_port.h"
/* Private define -------------------------------------------------------------------------------*/

/* Private typedef ------------------------------------------------------------------------------*/







 




typedef struct Modbus_Interface_Bind
{
    emModebus_RTU_Bus bus;
    stModbus_RTU_Handler *handler;

}stModbus_Interface_Bind;


/* Private functions prototypes -----------------------------------------------------------------*/



int8_t modbus_fun_request_03(stModbus_RTU_Handler *pstModbus_RTU_Handler, stModbus_RTU_Sender *sender);
/* Private variable -----------------------------------------------------------------------------*/







stModbus_Interface_Bind stModbus_Interface_Bind_Table[10];

#define MODBUS_INTERFACE_BIND_TABLE_ITEMS (sizeof(stModbus_Interface_Bind_Table)/sizeof(stModbus_Interface_Bind))
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

void modbus_rtu_slave(stModbus_RTU_Handler *handler)
{

    emModebus_RTU_Mode mode = handler->mode;
    if(handler->mode != handler->last_mode)
    {
        mode = emModbus_RTU_State_init;
        handler->mode = emModbus_RTU_State_init;
    }
    switch (mode)
    {
    case emModbus_RTU_State_init:
        
        break;
    case emModbus_RTU_State_IDLE:
        if(handler->recv(handler->rx_buff, handler->rx_len))
        {

        }
        
        break;
    case emModbus_RTU_State_Receive:
        break;

    case emModbus_RTU_State_Send:
        break;
    default:
        break;
    }
}


void modbus_rtu_master(stModbus_RTU_Handler *pstModbus_RTU_Handler)
{   
    emModebus_RTU_Mode mode = pstModbus_RTU_Handler->mode;
    if(pstModbus_RTU_Handler->mode != pstModbus_RTU_Handler->last_mode)
    {
        mode = emModbus_RTU_State_init;
        pstModbus_RTU_Handler->mode = emModbus_RTU_State_init;
    }
    switch (mode)
    {
    case emModbus_RTU_State_init:
        
        break;
    case emModbus_RTU_State_IDLE:
        if(pstModbus_RTU_Handler->tx_len != 0)
        break;
    case emModbus_RTU_State_Receive:
        break;

    case emModbus_RTU_State_Send:
        break;
    default:
        break;
    }
}


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
            match_item.Reqst = handler->fun_table[i].Reqst;
            break;
        }
    }

    match_item.Reqst(handler, &sender);
    if(match_item.Reqst_CB)
    {
        match_item.Reqst_CB(NULL, 0); 
    }


}


int8_t modbus_rtu_read_hold(emModebus_RTU_Bus bus, uint8_t dev_addr, uint16_t reg_addr, uint16_t reg_num, uint16_t *reg_data)
{
    stModbus_RTU_Sender sender;
    sender.dev_addr = dev_addr;
    sender.fun_code = 0x03;
    sender.reg_addr = reg_addr;
    sender.reg_num = reg_num;

    stModbus_RTU_Handler *handler = NULL;
    for(int i = 0; i < MODBUS_INTERFACE_BIND_TABLE_ITEMS; i++)
    {
        if(stModbus_Interface_Bind_Table[i].bus == bus)
        {
            handler = stModbus_Interface_Bind_Table[i].handler;
            break;
        }
    }
    modbus_rtu_send(handler, sender);
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


int8_t modbus_crc_cal(uint8_t *buff, uint16_t len)
{




}




int8_t modbus_fun_request_03(stModbus_RTU_Handler *handler, stModbus_RTU_Sender *sender)
{
    uint8_t dev_addr = sender->dev_addr;
    uint8_t fun_code = sender->fun_code;
    uint16_t reg_addr = sender->reg_addr;
    uint16_t reg_num = sender->reg_num;

    uint8_t *buff = handler->rx_buff;

    buff[0] = dev_addr;
    buff[1] = fun_code;
    buff[2] = reg_addr>>8;
    buff[3] = (uint8_t)reg_addr;
    buff[4] = reg_num>>8;
    buff[5] = (uint8_t)reg_num;
    uint16_t crc = modbus_crc_cal(buff, 6);

    buff[6] = crc>>8;
    buff[8] = (uint8_t)crc;

    handler->rx_len = 9;
    if(handler->state == emModbus_RTU_State_IDLE)
    {
        handler->state = emModbus_RTU_State_Send;
    }
    

}



int8_t modbus_fun_response(stModbus_RTU_Handler *handler, uint8_t *buff, uint16_t len)
{
    int8_t ret = -1;
    // adu and addr parse
    if(ret == 0){
        uint8_t dev_addr = buff[0];
        if(dev_addr == 0 || dev_addr == handler->dev_addr)
        {
            uint8_t f_code = buff[1];
            bool match = false;
            int8_t (*Parse)(stModbus_RTU_Handler *handler, uint8_t *buff, uint16_t len) = NULL;
            int8_t (*Parse_CB)(uint8_t *buff, uint16_t len) = NULL;
            for(int i = 0; i < handler->fun_table_items; i++)
            {
                if(f_code == handler->fun_table[i].fcode)
                {
                    match =true;
                    Parse = handler->fun_table[i].Parse;
                    Parse_CB = handler->fun_table[i].Parse_CB;
                }
                if(match)
                {
                    ret = Parse(handler, buff, len);
                    if(ret == 0)
                    {
                        if(Parse_CB)
                        {
                            Parse_CB(buff, len);
                        }
                    }
                }else{
                    ret = Modebus_RTU_Erno_FUN_CODE_NOT_FOUND;
                }
            }
        }
    }

    return ret;
}



int8_t modbus_fun_response_03(stModbus_RTU_Handler *handler, uint8_t *buff, uint16_t len)
{
    int8_t ret = -1;
    // pdu parse
    uint16_t reg_addr = buff[2]<<8|buff[3];
    uint16_t read_len = buff[4]<<8|buff[5];

    stModbus_RTU_HoldReader reader;
    reader.reg_map_id = handler->reg_map_id;
    reader.reg_addr = reg_addr;
    reader.reg_num = read_len;

    ret = handler->read_hold(&reader);
    if(ret == 0)
    {
        handler->tx_buff[0] = handler->dev_addr;
        handler->tx_buff[1] = 0x03;
        handler->tx_buff[2] = 2*read_len;
        memcpy(&handler->tx_buff[3], reader.reg_data, 2*read_len);
        uint16_t crc_cal = modbus_crc_cal(handler->tx_buff, 3+2*read_len);
        handler->tx_buff[3+2*read_len] = (uint8_t)(crc_cal>>8);
        handler->tx_buff[3+2*read_len+1] = (uint8_t)crc_cal;
        handler->tx_len = 3+2*read_len+2;
    }

    return ret;
}

int8_t modbus_rtu_init(stModbus_RTU_Handler *handler, emModebus_RTU_Bus bus, stModbus_RTU_Handler_Attr *attr)
{
    if(bus =- 0)
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
    handler->dev_addr = attr->dev_addr;
    handler->mode = attr->mode;
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

int8_t modbus_rtu_set_recv(emModebus_RTU_Bus bus, int8_t (*recv)(uint8_t *, uint16_t))
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
