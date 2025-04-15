#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "./modbus_config.h"
#include "./modbus_rtu.h"

int8_t modbus_fun_request_03(stModbus_RTU_Handler *handler, stModbus_RTU_Sender *sender)
{
    uint8_t dev_addr = sender->dev_addr;
    uint8_t fun_code = sender->fun_code;
    uint16_t reg_addr = sender->reg_addr;
    uint16_t reg_num = sender->reg_num;
    uint8_t *buff = handler->tx_buff;

    buff[0] = dev_addr;
    buff[1] = fun_code;
    buff[2] = reg_addr>>8;
    buff[3] = (uint8_t)reg_addr;
    buff[4] = reg_num>>8;
    buff[5] = (uint8_t)reg_num;
    uint16_t crc = modbus_crc_cal(buff, 6);

    buff[6] = crc>>8;
    buff[7] = (uint8_t)crc;

    handler->tx_len = 8;

    return 0;
}





int8_t modbus_rtu_send(stModbus_RTU_Handler *handler, stModbus_RTU_Sender sender)
{
    stModebus_RTU_Fun_Table match_item = {0};
    bool match = false;
    for(int i = 0; i < handler->fun_table_items; i++)
    {
        if(handler->fun_table[i].fcode == sender.fun_code)
        {
            match = true;
            match_item.fcode = handler->fun_table[i].fcode;
            match_item.request = handler->fun_table[i].request;
            break;
        }
    }
    if(!match || match_item.request == NULL)
    {
        return -1;
    }
    int8_t ret = match_item.request(handler, &sender);
    return ret;
}


int8_t modbus_rtu_read_hold(emModebus_RTU_Bus bus, uint8_t dev_addr, uint16_t reg_addr, uint16_t reg_num, uint16_t *output)
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
    if(handler->tx_len)
    {
        // send busy
        return -1;
    }

    int8_t ret = modbus_rtu_send(handler, sender);
    if(ret == 0)
    {
        handler->master_parse_addr = output;
    }
    return ret;
    
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
    if(handler->last_state == emModbus_RTU_State_Send && handler->state == emModbus_RTU_State_IDLE)
    {
        return 2;
    }
    if(handler->last_state == emModbus_RTU_State_Receive && handler->state == emModbus_RTU_State_IDLE)
    {
        return 0;
    }
    return 1;
}


int8_t modbus_fun_parse_03_master(stModbus_RTU_Handler *handler, uint8_t *buff, uint16_t len)
{
    int8_t ret = -1;
    uint16_t crc_local = modbus_crc_cal(buff, len-2);
    uint16_t crc_remote = (buff[len-2]<<8) | buff[len-1];
    if(crc_local == crc_remote)
    {
        ret = 0;
        for(int i = 0; i < len-5; i+=2){
            handler->master_parse_addr[i/2] = (buff[4+i]<<8) | buff[3+i];
        }
        
    }

    return ret;
}

int8_t modbus_fun_parse_master(stModbus_RTU_Handler *handler, uint8_t *buff, uint16_t len)
{
    int8_t ret = 0;
    uint16_t crc_local = modbus_crc_cal(buff, len-2);
    uint16_t crc_remote = (buff[len-2]<<8) | buff[len-1];
    if(crc_local == crc_remote)
    {
        // adu and addr parse
        uint8_t dev_addr = buff[0];
        uint8_t f_code = buff[1];
        bool match = false;
        int8_t (*parse)(stModbus_RTU_Handler *handler, uint8_t *buff, uint16_t len) = NULL;
        for(int i = 0; i < handler->fun_table_items; i++)
        {
            if(f_code == handler->fun_table[i].fcode)
            {
                match =true;
                parse = handler->fun_table[i].master_parse;
                break;
            }
        }
        if(match)
        {
            ret = parse(handler, buff, len);
        }else{
            ret = Modebus_RTU_Erno_FUN_CODE_NOT_FOUND;
        }
        
        
    }

    return ret;
}



void modbus_rtu_master(stModbus_RTU_Handler *handler)
{   
    if(handler->mode != handler->last_mode)
    {
        handler->state = emModbus_RTU_State_init;
        handler->last_mode = handler->mode;
    }
    switch (handler->state)
    {
    case emModbus_RTU_State_init:
        mylog("bus init\n");
        handler->last_state = emModbus_RTU_State_init;
        handler->state = emModbus_RTU_State_IDLE;
        break;
    case emModbus_RTU_State_IDLE:
        if(handler->tx_len != 0)
        {
            handler->last_mode = emModbus_RTU_State_IDLE;
            handler->state = emModbus_RTU_State_Send;
        }
        break;
    case emModbus_RTU_State_Receive:
        uint32_t now = modbus_port_get_time_ms();
        if(now - handler->Master_Wait_Count > handler->Master_Wait_Recv_Limt)
        {
            mylog("bus receive timeout\n");
            handler->Master_Wait_Count = 0;
            handler->state = emModbus_RTU_State_IDLE;
            break;
        }
        if(handler->recv(handler->rx_buff, &handler->rx_len))
        {
            int8_t ret = modbus_fun_parse_master(handler, handler->rx_buff, handler->rx_len);
            if(ret == 0)
            {
                handler->last_state = emModbus_RTU_State_Receive;
                handler->state = emModbus_RTU_State_IDLE;
            }
        }
        break;

    case emModbus_RTU_State_Send:
        handler->send(handler->tx_buff, handler->tx_len);
        handler->tx_len = 0;
        handler->Master_Wait_Count = modbus_port_get_time_ms();
        handler->last_state = emModbus_RTU_State_Send;
        handler->state = emModbus_RTU_State_Receive;
        break;
    default:
        break;
    }
}

