#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "./modbus_config.h"
#include "./modbus_rtu.h"

int8_t modbus_fun_request_03(stModbus_RTU_Handler_def *pstHandler, stModbus_RTU_Sender_def *sender)
{
    uint8_t ucDev_addr = sender->ucDev_addr;
    uint8_t ucFun_code = sender->ucFun_code;
    uint16_t usReg_addr = sender->usReg_addr;
    uint16_t ucReg_num = sender->ucReg_num;
    uint8_t *buff = pstHandler->ucTx_buff;

    buff[0] = ucDev_addr;
    buff[1] = ucFun_code;
    buff[2] = usReg_addr>>8;
    buff[3] = (uint8_t)usReg_addr;
    buff[4] = ucReg_num>>8;
    buff[5] = (uint8_t)ucReg_num;
    uint16_t crc = modbus_crc_cal(buff, 6);

    buff[6] = crc>>8;
    buff[7] = (uint8_t)crc;

    pstHandler->usTx_len = 8;

    return 0;
}

int8_t modbus_fun_request_04(stModbus_RTU_Handler_def *pstHandler, stModbus_RTU_Sender_def *sender)
{
    uint8_t ucDev_addr = sender->ucDev_addr;
    uint8_t ucFun_code = sender->ucFun_code;
    uint16_t usReg_addr = sender->usReg_addr;
    uint16_t ucReg_num = sender->ucReg_num;
    uint8_t *buff = pstHandler->ucTx_buff;

    buff[0] = ucDev_addr;
    buff[1] = ucFun_code;
    buff[2] = usReg_addr>>8;
    buff[3] = (uint8_t)usReg_addr;
    buff[4] = ucReg_num>>8;
    buff[5] = (uint8_t)ucReg_num;
    uint16_t crc = modbus_crc_cal(buff, 6);

    buff[6] = crc>>8;
    buff[7] = (uint8_t)crc;

    pstHandler->usTx_len = 8;

    return 0;
}

int8_t modbus_fun_request_06(stModbus_RTU_Handler_def *pstHandler, stModbus_RTU_Sender_def *sender)
{
    uint8_t ucDev_addr = sender->ucDev_addr;
    uint8_t ucFun_code = sender->ucFun_code;
    uint16_t usReg_addr = sender->usReg_addr;
    uint16_t ucReg_num = sender->ucReg_num;
    uint16_t *usReg_data = sender->usReg_data;
    uint8_t *buff = pstHandler->ucTx_buff;

    buff[0] = ucDev_addr;
    buff[1] = ucFun_code;
    buff[2] = usReg_addr>>8;
    buff[3] = (uint8_t)usReg_addr;
    buff[4] = usReg_data[0]>>8;
    buff[5] = (uint8_t)usReg_data[0];
    uint16_t crc = modbus_crc_cal(buff, 6);

    buff[6] = crc>>8;
    buff[7] = (uint8_t)crc;

    pstHandler->usTx_len = 8;

    return 0;
}

int8_t modbus_fun_request_10(stModbus_RTU_Handler_def *pstHandler, stModbus_RTU_Sender_def *sender)
{
    uint8_t ucDev_addr = sender->ucDev_addr;
    uint8_t ucFun_code = sender->ucFun_code;
    uint16_t usReg_addr = sender->usReg_addr;
    uint16_t ucReg_num = sender->ucReg_num;
    uint16_t *usReg_data = sender->usReg_data;
    uint8_t *buff = pstHandler->ucTx_buff;

    buff[0] = ucDev_addr;
    buff[1] = ucFun_code;
    buff[2] = usReg_addr>>8;
    buff[3] = (uint8_t)usReg_addr;
    buff[4] = ucReg_num>>8;
    buff[5] = (uint8_t)ucReg_num;
    buff[6] = ucReg_num*2;
    for(int i = 0; i < ucReg_num; i++)        // big endian
    {
        buff[i*2+7] = usReg_data[i]>>8;
        buff[i*2+8] = (uint8_t)usReg_data[i];
    }
    uint16_t crc = modbus_crc_cal(buff, 7+2*ucReg_num);

    buff[7+2*ucReg_num] = crc>>8;
    buff[8+2*ucReg_num] = (uint8_t)crc;

    pstHandler->usTx_len = 9+ucReg_num*2;

    return 0;
}



int8_t modbus_rtu_send(stModbus_RTU_Handler_def *pstHandler, stModbus_RTU_Sender_def sender)
{
    stModebus_RTU_Fun_Table_def match_item = {0};
    bool match = false;
    for(int i = 0; i < pstHandler->ucFun_table_items; i++)
    {
        if(pstHandler->pstFun_table[i].ucFcode == sender.ucFun_code)
        {
            match = true;
            match_item.ucFcode = pstHandler->pstFun_table[i].ucFcode;
            match_item.pRequest_f = pstHandler->pstFun_table[i].pRequest_f;
            break;
        }
    }
    if(!match || match_item.pRequest_f == NULL)
    {
        return -1;
    }
    int8_t ret = match_item.pRequest_f(pstHandler, &sender);
    if(ret == 0)                                                        /// XXX 考虑广播与扩展功能码的情况,可能不需要这么做
    {
        pstHandler->master_request_addr = sender.ucDev_addr;
        pstHandler->master_request_code = sender.ucFun_code;
    }
    return ret;
}


int8_t modbus_rtu_read_input(eModebus_RTU_Bus_def bus, uint8_t ucDev_addr, uint16_t usReg_addr, uint16_t ucReg_num, uint16_t *output)
{
    if(bus == 0)
    {
        return -1;
    }
    stModbus_RTU_Sender_def sender;
    sender.ucDev_addr = ucDev_addr;
    sender.ucFun_code = 0x04;
    sender.usReg_addr = usReg_addr;
    sender.ucReg_num = ucReg_num;

    stModbus_RTU_Handler_def *pstHandler = NULL;
    for(int i = 0; i < MODBUS_INTERFACE_BIND_TABLE_ITEMS; i++)
    {
        if(stModbus_Interface_Bind_Table[i].bus == bus)
        {
            pstHandler = stModbus_Interface_Bind_Table[i].pstHandler;
            break;
        }
    }
    if(pstHandler->usTx_len)
    {
        // pSend_f busy
        return -1;
    }

    int8_t ret = modbus_rtu_send(pstHandler, sender);
    if(ret == 0)
    {
        pstHandler->master_request_rw_len = ucReg_num;
        pstHandler->master_parse_addr = output;
    }
    return ret;
    
}



int8_t modbus_rtu_read_hold(eModebus_RTU_Bus_def bus, uint8_t ucDev_addr, uint16_t usReg_addr, uint16_t ucReg_num, uint16_t *output)
{
    if(bus == 0)
    {
        return -1;
    }
    stModbus_RTU_Sender_def sender;
    sender.ucDev_addr = ucDev_addr;
    sender.ucFun_code = 0x03;
    sender.usReg_addr = usReg_addr;
    sender.ucReg_num = ucReg_num;

    stModbus_RTU_Handler_def *pstHandler = NULL;
    for(int i = 0; i < MODBUS_INTERFACE_BIND_TABLE_ITEMS; i++)
    {
        if(stModbus_Interface_Bind_Table[i].bus == bus)
        {
            pstHandler = stModbus_Interface_Bind_Table[i].pstHandler;
            break;
        }
    }
    if(pstHandler->usTx_len)
    {
        // pSend_f busy
        return -1;
    }

    int8_t ret = modbus_rtu_send(pstHandler, sender);
    if(ret == 0)
    {
        pstHandler->master_request_rw_len = ucReg_num;
        pstHandler->master_parse_addr = output;
    }
    return ret;
    
}


int8_t modbus_rtu_write_hold(eModebus_RTU_Bus_def bus, uint8_t ucDev_addr, uint16_t usReg_addr, uint16_t ucReg_num, uint16_t *input)
{
    if(bus == 0)
    {
        return -1;
    }
    stModbus_RTU_Sender_def sender;
    sender.ucDev_addr = ucDev_addr;
    sender.usReg_addr = usReg_addr;
    sender.ucReg_num = ucReg_num;
    if(ucReg_num == 1)
    {
        sender.ucFun_code = 0x06;
        memcpy(sender.usReg_data, input, 2);
    }else{
        sender.ucFun_code = 0x10;
        memcpy(sender.usReg_data, input, ucReg_num*2);
    }
    

    stModbus_RTU_Handler_def *pstHandler = NULL;
    for(int i = 0; i < MODBUS_INTERFACE_BIND_TABLE_ITEMS; i++)
    {
        if(stModbus_Interface_Bind_Table[i].bus == bus)
        {
            pstHandler = stModbus_Interface_Bind_Table[i].pstHandler;
            break;
        }
    }
    if(pstHandler->usTx_len)
    {
        // pSend_f busy
        return -1;
    }

    int8_t ret = modbus_rtu_send(pstHandler, sender);
    if(ret == 0)
    {

    }
    return ret;
    
}
int8_t modbus_rtu_opt_status(eModebus_RTU_Bus_def bus)
{
    if(bus == 0)
    {
        return -1;
    }
    stModbus_RTU_Handler_def *pstHandler = NULL;
    for(int i = 0; i < MODBUS_INTERFACE_BIND_TABLE_ITEMS; i++)
    {
        if(stModbus_Interface_Bind_Table[i].bus == bus)
        {
            pstHandler = stModbus_Interface_Bind_Table[i].pstHandler;
            break;
        }
    }
    if(pstHandler->eMode == eModebus_RTU_Mode_Slave)
    {
        return -1;
    }
    if(pstHandler->eLast_state == eModbus_RTU_State_Send && pstHandler->eState == eModbus_RTU_State_IDLE)
    {
        return -1;
    }
    if(pstHandler->eLast_state == eModbus_RTU_State_Receive && pstHandler->eState == eModbus_RTU_State_IDLE)
    {
        return 0;
    }
    return 1;
}


int8_t modbus_fun_parse_03_master(stModbus_RTU_Handler_def *pstHandler, uint8_t *buff, uint16_t len)
{
    int8_t ret = 0;
    uint8_t ucDev_addr = buff[0];
    uint8_t f_code = buff[1];
    uint16_t value_len = buff[2];


    if(f_code == 0x83)
    {
        if(len != 5)
        {
            return eModebus_RTU_Erno_FRAME_FORMAT_ERROR;
        }else {
            return buff[2];             // return slave status
        }
    }else if(f_code != 0x03)
    {
        return eModebus_RTU_Erno_MASTER_REQUEST_ADDR_NOT_MATCH;
    }
    if(len != 5+value_len)
    {
        return eModebus_RTU_Erno_FRAME_FORMAT_ERROR;
    }

    if(value_len != 2*pstHandler->master_request_rw_len)
    {
        return eModebus_RTU_Erno_FRAME_FORMAT_ERROR;
    }

    for(int i = 0; i < len-5; i+=2){
        pstHandler->master_parse_addr[i/2] = (buff[4+i]<<8) | buff[3+i];       // big endian to uin16_t
    }
        
    

    return ret;
}



int8_t modbus_fun_parse_04_master(stModbus_RTU_Handler_def *pstHandler, uint8_t *buff, uint16_t len)
{
    int8_t ret = 0;
    uint8_t ucDev_addr = buff[0];
    uint8_t f_code = buff[1];
    uint16_t value_len = buff[2];


    if(f_code == 0x84)
    {
        if(len != 5)
        {
            return eModebus_RTU_Erno_FRAME_FORMAT_ERROR;
        }else {
            return buff[2];             // return slave status
        }
    }else if(f_code != 0x04)
    {
        return eModebus_RTU_Erno_MASTER_REQUEST_ADDR_NOT_MATCH;
    }
    if(len != 5+value_len)
    {
        return eModebus_RTU_Erno_FRAME_FORMAT_ERROR;
    }

    if(value_len != 2*pstHandler->master_request_rw_len)
    {
        return eModebus_RTU_Erno_FRAME_FORMAT_ERROR;
    }

    for(int i = 0; i < len-5; i+=2){
        pstHandler->master_parse_addr[i/2] = (buff[4+i]<<8) | buff[3+i];       // big endian to uin16_t
    }
        
    return ret;
}



int8_t modbus_fun_parse_06_master(stModbus_RTU_Handler_def *pstHandler, uint8_t *buff, uint16_t len)
{
    int8_t ret = 0;
    uint8_t ucDev_addr = buff[0];
    uint8_t f_code = buff[1];


    if(f_code == 0x86)
    {
        if(len != 5)
        {
            return eModebus_RTU_Erno_FRAME_FORMAT_ERROR;
        }else {
            return buff[2];             // return slave status
        }
    }else if(f_code != 0x06)
    {
        return eModebus_RTU_Erno_MASTER_REQUEST_ADDR_NOT_MATCH;
    }
    if(len != 8)
    {
        return eModebus_RTU_Erno_FRAME_FORMAT_ERROR;
    }

    if(memcmp(buff, pstHandler->ucTx_buff, 10))
    {
        return eModebus_RTU_Erno_FRAME_FORMAT_ERROR;
    }
        
    return ret;
}

int8_t modbus_fun_parse_10_master(stModbus_RTU_Handler_def *pstHandler, uint8_t *buff, uint16_t len)
{
    int8_t ret = 0;
    uint8_t ucDev_addr = buff[0];
    uint8_t f_code = buff[1];
    uint16_t value_len = buff[6];

    if(f_code == 0x90)
    {
        if(len != 5)
        {
            return eModebus_RTU_Erno_FRAME_FORMAT_ERROR;
        }else {
            return buff[2];             // return slave status
        }
    }else if(f_code != 0x10)
    {
        return eModebus_RTU_Erno_MASTER_REQUEST_ADDR_NOT_MATCH;
    }
    if(len != 9+value_len)
    {
        return eModebus_RTU_Erno_FRAME_FORMAT_ERROR;
    }

    if(memcmp(buff, pstHandler->ucTx_buff, len))
    {
        return eModebus_RTU_Erno_FRAME_FORMAT_ERROR;
    }
        
    return ret;
}



int8_t modbus_fun_parse_master(stModbus_RTU_Handler_def *pstHandler, uint8_t *buff, uint16_t len)
{
    int8_t ret = 0;
    uint16_t crc_local = modbus_crc_cal(buff, len-2);
    uint16_t crc_remote = (buff[len-2]<<8) | buff[len-1];
    if(crc_local == crc_remote)
    {
        // adu and addr parse
        uint8_t ucDev_addr = buff[0];
        if(ucDev_addr == pstHandler->master_request_addr){                                               /// XXX 考虑广播的情况       
            uint8_t f_code = buff[1];
            int8_t (*parse)(stModbus_RTU_Handler_def *pstHandler, uint8_t *buff, uint16_t len) = NULL;
            for(int i = 0; i < pstHandler->ucFun_table_items; i++)
            {
                if(pstHandler->master_request_code == pstHandler->pstFun_table[i].ucFcode)
                {
                    parse = pstHandler->pstFun_table[i].pMaster_parse_f;
                    break;
                }
            }
            if(parse == NULL)
            {
                ret = eModebus_RTU_Erno_MASTER_PARSE_FUN_NOT_FOUND;
            }else{
                ret = parse(pstHandler, buff, len);
            }
        }else{
            ret = eModebus_RTU_Erno_MASTER_REQUEST_ADDR_NOT_MATCH;
        }
    }

    return ret;
}



void modbus_rtu_master(stModbus_RTU_Handler_def *pstHandler)
{   
    if(pstHandler->eMode != pstHandler->eLast_mode)
    {
        pstHandler->eState = eModbus_RTU_State_Init;
        pstHandler->eLast_mode = pstHandler->eMode;
    }
    switch (pstHandler->eState)
    {
    case eModbus_RTU_State_Init:
        mylog("bus init\n");
        pstHandler->eLast_state = eModbus_RTU_State_Init;
        pstHandler->eState = eModbus_RTU_State_IDLE;
        break;
    case eModbus_RTU_State_IDLE:
        if(pstHandler->usTx_len != 0)
        {
            pstHandler->eLast_mode = eModbus_RTU_State_IDLE;
            pstHandler->eState = eModbus_RTU_State_Send;
        }
        break;
    case eModbus_RTU_State_Receive:
        uint32_t now = modbus_port_get_time_ms();
        if(now - pstHandler->Master_Wait_Count > pstHandler->Master_Wait_Recv_Limt)
        {
            mylog("bus receive timeout\n");
            pstHandler->usTx_len = 0;
            pstHandler->eState = eModbus_RTU_State_IDLE;
            break;
        }
        if(pstHandler->pRecv_f(pstHandler->ucRx_buff, &pstHandler->ucRx_len))
        {
            int8_t ret = modbus_fun_parse_master(pstHandler, pstHandler->ucRx_buff, pstHandler->ucRx_len);                   /// XXX 考虑广播的情况
            if(ret == 0)                                                                // success
            {
                pstHandler->eLast_state = eModbus_RTU_State_Receive;
                pstHandler->eState = eModbus_RTU_State_IDLE;
                pstHandler->usTx_len = 0;
                break;
            }else if(ret == eModebus_RTU_Erno_FRAME_FORMAT_ERROR ||                      // fail
                     ret == eModebus_RTU_Erno_FUN_CODE_NOT_FOUND || 
                     ret == eModebus_RTU_Erno_REG_ADDR_INVALID   ||
                     ret == eModebus_RTU_Erno_REG_VALUE_INVALID    ){
                        pstHandler->eState = eModbus_RTU_State_IDLE;
                        pstHandler->usTx_len = 0;
                        break; 
            }else if(ret == eModebus_RTU_Erno_MASTER_REQUEST_ADDR_NOT_MATCH){
                break;                                                          // parse next frame(may be fail with time out if no next frame)
            }
        }
        break;
    case eModbus_RTU_State_Send:
        pstHandler->pSend_f(pstHandler->ucTx_buff, pstHandler->usTx_len);
        mylog("bus pSend_f\n");
        pstHandler->Master_Wait_Count = modbus_port_get_time_ms();
        pstHandler->eLast_state = eModbus_RTU_State_Send;
        pstHandler->eState = eModbus_RTU_State_Receive;
        break;
    default:
        break;
    }
}

