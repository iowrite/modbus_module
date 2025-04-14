#include <stddef.h>
#include "./modbus_rtu.h"

int8_t modbus_fun_request_03(stModbus_RTU_Handler *handler, stModbus_RTU_Sender *sender)
{
    uint8_t dev_addr = sender->dev_addr;
    uint8_t fun_code = sender->fun_code;
    uint16_t reg_addr = sender->reg_addr;
    uint16_t reg_num = sender->reg_num;
    handler->master_parse_addr = sender->reg_data;
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

int8_t modbus_fun_parse_03_master(stModbus_RTU_Handler *handler, uint8_t *buff, uint16_t len)
{
    int8_t ret = -1;


    return ret;
}


int8_t modbus_rtu_read_hold(emModebus_RTU_Bus bus, uint8_t dev_addr, uint16_t reg_addr, uint16_t reg_num, uint16_t *reg_data)
{
    if(bus == 0)
    {
        return -1;
    }
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

void modbus_rtu_master(stModbus_RTU_Handler *handler)
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
        if(handler->tx_len != 0)
        break;
    case emModbus_RTU_State_Receive:
        break;

    case emModbus_RTU_State_Send:
        break;
    default:
        break;
    }
}

