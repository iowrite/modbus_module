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

int8_t modbus_fun_request_04(stModbus_RTU_Handler *handler, stModbus_RTU_Sender *sender)
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

int8_t modbus_fun_request_06(stModbus_RTU_Handler *handler, stModbus_RTU_Sender *sender)
{
    uint8_t dev_addr = sender->dev_addr;
    uint8_t fun_code = sender->fun_code;
    uint16_t reg_addr = sender->reg_addr;
    uint16_t reg_num = sender->reg_num;
    uint16_t *reg_data = sender->reg_data;
    uint8_t *buff = handler->tx_buff;

    buff[0] = dev_addr;
    buff[1] = fun_code;
    buff[2] = reg_addr>>8;
    buff[3] = (uint8_t)reg_addr;
    buff[4] = reg_data[0]>>8;
    buff[5] = (uint8_t)reg_data[0];
    uint16_t crc = modbus_crc_cal(buff, 6);

    buff[6] = crc>>8;
    buff[7] = (uint8_t)crc;

    handler->tx_len = 8;

    return 0;
}

int8_t modbus_fun_request_10(stModbus_RTU_Handler *handler, stModbus_RTU_Sender *sender)
{
    uint8_t dev_addr = sender->dev_addr;
    uint8_t fun_code = sender->fun_code;
    uint16_t reg_addr = sender->reg_addr;
    uint16_t reg_num = sender->reg_num;
    uint16_t *reg_data = sender->reg_data;
    uint8_t *buff = handler->tx_buff;

    buff[0] = dev_addr;
    buff[1] = fun_code;
    buff[2] = reg_addr>>8;
    buff[3] = (uint8_t)reg_addr;
    buff[4] = reg_num>>8;
    buff[5] = (uint8_t)reg_num;
    buff[6] = reg_num*2;
    for(int i = 0; i < reg_num; i++)        // big endian
    {
        buff[i*2+7] = reg_data[i]>>8;
        buff[i*2+8] = (uint8_t)reg_data[i];
    }
    uint16_t crc = modbus_crc_cal(buff, 7+2*reg_num);

    buff[7+2*reg_num] = crc>>8;
    buff[8+2*reg_num] = (uint8_t)crc;

    handler->tx_len = 9+reg_num*2;

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
    if(ret == 0)                                                        /// XXX 考虑广播与扩展功能码的情况,可能不需要这么做
    {
        handler->master_request_addr = sender.dev_addr;
        handler->master_request_code = sender.fun_code;
    }
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
        handler->master_request_rw_len = reg_num;
        handler->master_parse_addr = output;
    }
    return ret;
    
}


int8_t modbus_rtu_write_hold(emModebus_RTU_Bus bus, uint8_t dev_addr, uint16_t reg_addr, uint16_t reg_num, uint16_t *input)
{
    if(bus == 0)
    {
        return -1;
    }
    stModbus_RTU_Sender sender;
    sender.dev_addr = dev_addr;
    sender.reg_addr = reg_addr;
    sender.reg_num = reg_num;
    if(reg_num == 1)
    {
        sender.fun_code = 0x06;
        memcpy(sender.reg_data, input, 2);
    }else{
        sender.fun_code = 0x10;
        memcpy(sender.reg_data, input, reg_num*2);
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
    if(handler->tx_len)
    {
        // send busy
        return -1;
    }

    int8_t ret = modbus_rtu_send(handler, sender);
    if(ret == 0)
    {

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
        return -1;
    }
    if(handler->last_state == emModbus_RTU_State_Receive && handler->state == emModbus_RTU_State_IDLE)
    {
        return 0;
    }
    return 1;
}


int8_t modbus_fun_parse_03_master(stModbus_RTU_Handler *handler, uint8_t *buff, uint16_t len)
{
    int8_t ret = 0;
    uint8_t dev_addr = buff[0];
    uint8_t f_code = buff[1];
    uint16_t value_len = buff[2];


    if(f_code == 0x83)
    {
        if(len != 5)
        {
            return Modebus_RTU_Erno_FRAME_FORMAT_ERROR;
        }else {
            return buff[2];             // return slave status
        }
    }else if(f_code != 0x03)
    {
        return Modebus_RTU_Erno_MASTER_REQUEST_ADDR_NOT_MATCH;
    }
    if(len != 5+value_len)
    {
        return Modebus_RTU_Erno_FRAME_FORMAT_ERROR;
    }

    if(value_len != 2*handler->master_request_rw_len)
    {
        return Modebus_RTU_Erno_FRAME_FORMAT_ERROR;
    }

    for(int i = 0; i < len-5; i+=2){
        handler->master_parse_addr[i/2] = (buff[4+i]<<8) | buff[3+i];       // big endian to uin16_t
    }
        
    

    return ret;
}



int8_t modbus_fun_parse_04_master(stModbus_RTU_Handler *handler, uint8_t *buff, uint16_t len)
{
    int8_t ret = 0;
    uint8_t dev_addr = buff[0];
    uint8_t f_code = buff[1];
    uint16_t value_len = buff[2];


    if(f_code == 0x84)
    {
        if(len != 5)
        {
            return Modebus_RTU_Erno_FRAME_FORMAT_ERROR;
        }else {
            return buff[2];             // return slave status
        }
    }else if(f_code != 0x04)
    {
        return Modebus_RTU_Erno_MASTER_REQUEST_ADDR_NOT_MATCH;
    }
    if(len != 5+value_len)
    {
        return Modebus_RTU_Erno_FRAME_FORMAT_ERROR;
    }

    if(value_len != 2*handler->master_request_rw_len)
    {
        return Modebus_RTU_Erno_FRAME_FORMAT_ERROR;
    }

    for(int i = 0; i < len-5; i+=2){
        handler->master_parse_addr[i/2] = (buff[4+i]<<8) | buff[3+i];       // big endian to uin16_t
    }
        
    return ret;
}



int8_t modbus_fun_parse_06_master(stModbus_RTU_Handler *handler, uint8_t *buff, uint16_t len)
{
    int8_t ret = 0;
    uint8_t dev_addr = buff[0];
    uint8_t f_code = buff[1];


    if(f_code == 0x86)
    {
        if(len != 5)
        {
            return Modebus_RTU_Erno_FRAME_FORMAT_ERROR;
        }else {
            return buff[2];             // return slave status
        }
    }else if(f_code != 0x06)
    {
        return Modebus_RTU_Erno_MASTER_REQUEST_ADDR_NOT_MATCH;
    }
    if(len != 8)
    {
        return Modebus_RTU_Erno_FRAME_FORMAT_ERROR;
    }

    if(memcmp(buff, handler->tx_buff, 10))
    {
        return Modebus_RTU_Erno_FRAME_FORMAT_ERROR;
    }
        
    return ret;
}

int8_t modbus_fun_parse_10_master(stModbus_RTU_Handler *handler, uint8_t *buff, uint16_t len)
{
    int8_t ret = 0;
    uint8_t dev_addr = buff[0];
    uint8_t f_code = buff[1];
    uint16_t value_len = buff[6];

    if(f_code == 0x90)
    {
        if(len != 5)
        {
            return Modebus_RTU_Erno_FRAME_FORMAT_ERROR;
        }else {
            return buff[2];             // return slave status
        }
    }else if(f_code != 0x10)
    {
        return Modebus_RTU_Erno_MASTER_REQUEST_ADDR_NOT_MATCH;
    }
    if(len != 9+value_len)
    {
        return Modebus_RTU_Erno_FRAME_FORMAT_ERROR;
    }

    if(memcmp(buff, handler->tx_buff, len))
    {
        return Modebus_RTU_Erno_FRAME_FORMAT_ERROR;
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
        if(dev_addr == handler->master_request_addr){                                               /// XXX 考虑广播的情况       
            uint8_t f_code = buff[1];
            int8_t (*parse)(stModbus_RTU_Handler *handler, uint8_t *buff, uint16_t len) = NULL;
            for(int i = 0; i < handler->fun_table_items; i++)
            {
                if(handler->master_request_code == handler->fun_table[i].fcode)
                {
                    parse = handler->fun_table[i].master_parse;
                    break;
                }
            }
            if(parse == NULL)
            {
                ret = Modebus_RTU_Erno_MASTER_PARSE_FUN_NOT_FOUND;
            }else{
                ret = parse(handler, buff, len);
            }
        }else{
            ret = Modebus_RTU_Erno_MASTER_REQUEST_ADDR_NOT_MATCH;
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
            handler->tx_len = 0;
            handler->state = emModbus_RTU_State_IDLE;
            break;
        }
        if(handler->recv(handler->rx_buff, &handler->rx_len))
        {
            int8_t ret = modbus_fun_parse_master(handler, handler->rx_buff, handler->rx_len);                   /// XXX 考虑广播的情况
            if(ret == 0)                                                                // success
            {
                handler->last_state = emModbus_RTU_State_Receive;
                handler->state = emModbus_RTU_State_IDLE;
                handler->tx_len = 0;
                break;
            }else if(ret == Modebus_RTU_Erno_FRAME_FORMAT_ERROR ||                      // fail
                     ret == Modebus_RTU_Erno_FUN_CODE_NOT_FOUND || 
                     ret == Modebus_RTU_Erno_REG_ADDR_INVALID   ||
                     ret == Modebus_RTU_Erno_REG_VALUE_INVALID    ){
                        handler->state = emModbus_RTU_State_IDLE;
                        handler->tx_len = 0;
                        break; 
            }else if(ret == Modebus_RTU_Erno_MASTER_REQUEST_ADDR_NOT_MATCH){
                break;                                                          // parse next frame(may be fail with time out if no next frame)
            }
        }
        break;
    case emModbus_RTU_State_Send:
        handler->send(handler->tx_buff, handler->tx_len);
        mylog("bus send\n");
        handler->Master_Wait_Count = modbus_port_get_time_ms();
        handler->last_state = emModbus_RTU_State_Send;
        handler->state = emModbus_RTU_State_Receive;
        break;
    default:
        break;
    }
}

