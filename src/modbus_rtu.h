 /**
  *************************************************************************************************
  * @file name : c_template.h
  * @encoding  : UTF-8
  * @author    : HDY(803098)
  * @date      : 2025.1.1
  * @brief     : This file provides
  * @attention 
  * Copyright (c) 2024 HOYPower.
  * All rights reserved.
  *************************************************************************************************
  */
#ifndef __MODBUS_RTU_H__
#define __MODBUS_RTU_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes -------------------------------------------------------------------------------------*/
#include "./modbus_port.h"
/* Exported defines -----------------------------------------------------------------------------*/
/*modbus Indicates the function length of a frame*/


/* Exported typedef -----------------------------------------------------------------------------*/
typedef enum Modebus_RTU_Erno
{
    Modebus_RTU_Erno_FUN_CODE_NOT_FOUND,
    
}emModebus_RTU_Erno;

typedef enum stModbus_RTU_State
{
    emModbus_RTU_State_init,
    emModbus_RTU_State_IDLE,
    emModbus_RTU_State_Receive,
    emModbus_RTU_State_Send,
    emModbus_RTU_State_NUM
}stModbus_RTU_State;


typedef enum Modebus_RTU_Mode
{
    emModebus_RTU_Mode_Slave,
    emModebus_RTU_Mode_Master,
    emModebus_RTU_Mode_NUM
}emModebus_RTU_Mode;
typedef struct Modbus_RTU_InputReader
{
    uint8_t reg_map_id;
    uint16_t reg_addr;
    uint8_t reg_num;
    uint8_t reg_data[300];
}stModbus_RTU_InputReader;

typedef struct Modbus_RTU_HoldReader
{
    uint8_t reg_map_id;
    uint16_t reg_addr;
    uint8_t reg_num;
    uint8_t reg_data[300];

}stModbus_RTU_HoldReader;

typedef struct Modbus_RTU_HoldWriter
{
    uint8_t reg_map_id;
    uint16_t reg_addr;
    uint8_t reg_num;
    uint8_t reg_data[300];

}stModbus_RTU_HoldWriter;

typedef struct Modbus_RTU_Sender
{
    uint8_t dev_addr;
    uint8_t fun_code;
    uint16_t reg_addr;
    uint8_t reg_num;
    uint8_t reg_data[300];
}stModbus_RTU_Sender;


typedef struct Modbus_RTU_Handler stModbus_RTU_Handler;                         // 前向声明 stModbus_RTU_Handler 与 Modbus_RTU_Handler 交叉声明
typedef struct Modebus_RTU_Fun_Table
{
    uint8_t fcode;
    int8_t (*Parse)(stModbus_RTU_Handler *handler, uint8_t *buff, uint16_t len);
    int8_t (*Reqst)(stModbus_RTU_Handler *handler, stModbus_RTU_Sender *sender);
    int8_t (*Parse_CB)(uint8_t *buff, uint16_t len);
    int8_t (*Reqst_CB)(uint8_t *buff, uint16_t len);
}stModebus_RTU_Fun_Table;

typedef struct Modbus_RTU_Handler
{
    uint8_t dev_addr;
    stModbus_RTU_State state, last_state;
    emModebus_RTU_Mode mode, last_mode;
    uint32_t last_call_tick;
    uint8_t tx_buff[300];
    uint16_t tx_len;
    uint8_t rx_buff[300];
    uint16_t rx_len;
    uint32_t Master_Wait_Recv_Limt;
    stModebus_RTU_Fun_Table *fun_table;                    // callback function table
    uint8_t fun_table_items;
    int8_t (*send)(uint8_t *buff, uint16_t len);                    
    int8_t (*recv)(uint8_t *buff, uint16_t len);
    int8_t reg_map_id;                                          // reserve for select register map table
    int8_t (*read_input)(stModbus_RTU_InputReader *reader);
    int8_t (*read_hold)(stModbus_RTU_HoldReader *reader);
    int8_t (*write_hold)(stModbus_RTU_HoldWriter *writer);

}stModbus_RTU_Handler;

 
typedef struct Modbus_RTU_Handler_Attribute
{
    uint8_t dev_addr;
    emModebus_RTU_Mode mode;
    int8_t (*send)(uint8_t *buff, uint16_t len);                    
    int8_t (*recv)(uint8_t *buff, uint16_t len);
    int8_t reg_map_id;                                          // reserve for select register map table
    int8_t (*read_input)(stModbus_RTU_InputReader *reader);
    int8_t (*read_hold)(stModbus_RTU_HoldReader *reader);
    int8_t (*write_hold)(stModbus_RTU_HoldWriter *writer);
    stModebus_RTU_Fun_Table *fun_table;                    // callback function table
    uint8_t fun_table_items;

}stModbus_RTU_Handler_Attr;







/* Exported functions prototypes ----------------------------------------------------------------*/
int8_t modbus_rtu_set_send(emModebus_RTU_Bus bus, int8_t (*send)(uint8_t *, uint16_t));
int8_t modbus_rtu_set_recv(emModebus_RTU_Bus bus, int8_t (*recv)(uint8_t *, uint16_t));

int8_t modbus_rtu_init(stModbus_RTU_Handler *handler, emModebus_RTU_Bus bus, stModbus_RTU_Handler_Attr *attr);
int8_t modbus_fun_request_03(stModbus_RTU_Handler *pstModbus_RTU_Handler, stModbus_RTU_Sender *sender);
int8_t modbus_fun_response_03(stModbus_RTU_Handler *handler, uint8_t *buff, uint16_t len);


void modbus_rtu_run(stModbus_RTU_Handler *handler);

#ifdef __cplusplus
}
#endif

#endif
/******************************************* END OF FILE *****************************************/
