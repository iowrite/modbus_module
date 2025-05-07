#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "./modbus_config.h"
#include "./modbus_rtu.h"

int8_t modbus_fun_parse_slave(stModbus_RTU_Handler_def *handler, uint8_t *buff, uint16_t len)
{
    int8_t ret = 0;
    uint16_t crc_local = modbus_crc_cal(buff, len-2);
    uint16_t crc_remote = (buff[len-2]<<8) | buff[len-1];
    if(crc_local == crc_remote)
    {
        // adu and addr parse
        uint8_t ucDev_addr = buff[0];
        if(ucDev_addr == 0 || ucDev_addr == handler->ucDev_addr)
        {
            uint8_t f_code = buff[1];
            bool match = false;
            int8_t (*parse)(stModbus_RTU_Handler_def *handler, uint8_t *buff, uint16_t len) = NULL;
            for(int i = 0; i < handler->fun_table_items; i++)
            {
                if(f_code == handler->fun_table[i].ucFcode)
                {
                    match =true;
                    parse = handler->fun_table[i].slave_parse;
                    break;
                }
            }
            if(match)
            {
                ret = parse(handler, buff, len);
            }else{
                ret = eModebus_RTU_Erno_FUN_CODE_NOT_FOUND;
            }
            if(ret)
            {
                handler->tx_buff[0] = handler->ucDev_addr;
                handler->tx_buff[1] = 0x80 | f_code;
                handler->tx_buff[2] = ret;
                uint16_t crc_cal = modbus_crc_cal(handler->tx_buff, 3);
                handler->tx_buff[3] = (uint8_t)(crc_cal>>8);
                handler->tx_buff[4] = (uint8_t)crc_cal;
                handler->tx_len = 5;
            }
        }
    }

    return ret;
}






int8_t modbus_fun_parse_slave_03(stModbus_RTU_Handler_def *handler, uint8_t *buff, uint16_t len)
{
    int8_t ret = -1;
    if(len != 8)
    {
        return eModebus_RTU_Erno_FRAME_FORMAT_ERROR;
    }
    // pdu parse
    uint16_t usReg_addr = buff[2]<<8|buff[3];     // big endian
    uint16_t read_len = buff[4]<<8|buff[5];     // big endian

    stModbus_RTU_HoldReader_def reader;
    reader.ucReg_map_id = handler->ucReg_map_id;
    reader.usReg_addr = usReg_addr;
    reader.ucReg_num = read_len;

    ret = handler->read_hold(&reader);
    if(ret == 0)
    {
        handler->tx_buff[0] = handler->ucDev_addr;
        handler->tx_buff[1] = 0x03;
        handler->tx_buff[2] = 2*read_len;
        memcpy(&handler->tx_buff[3], reader.ucReg_data_byte, 2*read_len);
        uint16_t crc_cal = modbus_crc_cal(handler->tx_buff, 3+2*read_len);
        handler->tx_buff[3+2*read_len] = (uint8_t)(crc_cal>>8);
        handler->tx_buff[3+2*read_len+1] = (uint8_t)crc_cal;
        handler->tx_len = 3+2*read_len+2;
    }

    return ret;
}


int8_t modbus_fun_parse_slave_04(stModbus_RTU_Handler_def *handler, uint8_t *buff, uint16_t len)
{
    int8_t ret = -1;
    if(len != 8)
    {
        return eModebus_RTU_Erno_FRAME_FORMAT_ERROR;
    }
    // pdu parse
    uint16_t usReg_addr = buff[2]<<8|buff[3];     // big endian
    uint16_t read_len = buff[4]<<8|buff[5];     // big endian

    stModbus_RTU_InputReader_def reader;
    reader.ucReg_map_id = handler->ucReg_map_id;
    reader.usReg_addr = usReg_addr;
    reader.ucReg_num = read_len;

    ret = handler->read_input(&reader);
    if(ret == 0)
    {
        handler->tx_buff[0] = handler->ucDev_addr;
        handler->tx_buff[1] = 0x04;
        handler->tx_buff[2] = 2*read_len;
        memcpy(&handler->tx_buff[3], reader.ucReg_data_byte, 2*read_len);
        uint16_t crc_cal = modbus_crc_cal(handler->tx_buff, 3+2*read_len);
        handler->tx_buff[3+2*read_len] = (uint8_t)(crc_cal>>8);
        handler->tx_buff[3+2*read_len+1] = (uint8_t)crc_cal;
        handler->tx_len = 3+2*read_len+2;
    }

    return ret;
}



int8_t modbus_fun_parse_slave_06(stModbus_RTU_Handler_def *handler, uint8_t *buff, uint16_t len)
{
    int8_t ret = -1;
    if(len != 8)
    {
        return eModebus_RTU_Erno_FRAME_FORMAT_ERROR;
    }
    // pdu parse
    uint16_t usReg_addr = buff[2]<<8|buff[3];         // big endian
    uint16_t write_value = buff[4]<<8|buff[5];       // big endian

    stModbus_RTU_HoldWriter_def writer;
    writer.ucReg_map_id = handler->ucReg_map_id;
    writer.usReg_addr = usReg_addr;
    writer.ucReg_num = 1;
    writer.usReg_data[0] = write_value;

    ret = handler->write_hold(&writer);
    if(ret == 0)
    {
        memcpy(&handler->tx_buff[0], buff, 8);
        handler->tx_len = 8;
    }

    return ret;
}

int8_t modbus_fun_parse_slave_10(stModbus_RTU_Handler_def *handler, uint8_t *buff, uint16_t len)
{
    int8_t ret = -1;
    
    // pdu parse
    uint16_t usReg_addr = buff[2]<<8|buff[3];         // big endian
    uint16_t write_num = buff[4]<<8|buff[5];       // big endian
    uint16_t write_len = buff[6];

    if(write_len != write_num*2)
    {
        return eModebus_RTU_Erno_FRAME_FORMAT_ERROR;
    }
    if(len != 9+write_len)
    {
        return eModebus_RTU_Erno_FRAME_FORMAT_ERROR;
    }
    stModbus_RTU_HoldWriter_def writer;
    writer.ucReg_map_id = handler->ucReg_map_id;
    writer.usReg_addr = usReg_addr;
    writer.ucReg_num = write_num;
    for(int i = 0; i < write_num; i++)
    {
        writer.usReg_data[i] = buff[7+2*i]<<8|buff[7+2*i+1];
    }

    ret = handler->write_hold(&writer);
    if(ret == 0)
    {
        memcpy(&handler->tx_buff[0], buff, len);
        handler->tx_len = len;
    }

    return ret;
}





void modbus_rtu_slave(stModbus_RTU_Handler_def *handler)
{

    if(handler->mode != handler->last_mode)
    {
        handler->state = eModbus_RTU_State_Init;
        handler->last_mode = handler->mode;
    }
    switch (handler->state)
    {
    case eModbus_RTU_State_Init:
        mylog("bus init\n");
        handler->state = eModbus_RTU_State_IDLE;
        break;
    case eModbus_RTU_State_IDLE:
        if(handler->recv(handler->rx_buff, &handler->rx_len))
        {
            mylog("bus recv\n");
            handler->state = eModbus_RTU_State_Receive;
        }
        break;
    case eModbus_RTU_State_Receive:
    {
        int8_t ret = modbus_fun_parse_slave(handler, handler->rx_buff, handler->rx_len);
        if(handler->tx_len > 0)
        {
            handler->state = eModbus_RTU_State_Send;
        }else{
            handler->state = eModbus_RTU_State_IDLE;
        }
    }
        break;
    case eModbus_RTU_State_Send:
        int8_t ret = handler->send(handler->tx_buff, handler->tx_len);
        mylog("bus send\n");
        handler->tx_len = 0;
        handler->state = eModbus_RTU_State_IDLE;
        break;
    default:
        break;
    }
}