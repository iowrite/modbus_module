#include "./modbus_port.h"

#ifndef __MODBUS_RTU_H__
#define __MODBUS_RTU_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes -------------------------------------------------------------------------------------*/


/* Exported defines -----------------------------------------------------------------------------*/


/* Exported typedef -----------------------------------------------------------------------------*/
typedef enum Modebus_RTU_Erno
{
    Modebus_RTU_Erno_START,
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
}stModbus_RTU_Sender;


typedef struct Modbus_RTU_Handler stModbus_RTU_Handler;                         // 前向声明 stModbus_RTU_Handler 与 Modbus_RTU_Handler 交叉声明
typedef struct Modebus_RTU_Fun_Table
{
    uint8_t fcode;
    int8_t (*request)(stModbus_RTU_Handler *handler, stModbus_RTU_Sender *sender);
    int8_t (*slave_parse)(stModbus_RTU_Handler *handler, uint8_t *buff, uint16_t len);
    int8_t (*master_parse)(stModbus_RTU_Handler *handler, uint8_t *buff, uint16_t len);

}stModebus_RTU_Fun_Table;

typedef struct Modbus_RTU_Handler
{
    uint8_t dev_addr;
    stModbus_RTU_State state, last_state;               // send/receive switch judge
    emModebus_RTU_Mode mode, last_mode;                 // slave/master switch judge
    uint32_t last_call_tick;
    uint8_t tx_buff[300];
    uint16_t tx_len;
    uint8_t rx_buff[300];
    uint16_t rx_len;
    uint16_t *master_parse_addr;                             // master parse address(master mode use only)
    uint32_t Master_Wait_Count;                              // master wait recv time count(master mode use only)
    uint32_t Master_Wait_Recv_Limt;                         // master wait recv limit(master mode use only)
    stModebus_RTU_Fun_Table *fun_table;                    // callback function table
    uint8_t fun_table_items;
    int8_t (*send)(uint8_t *buff, uint16_t len);                   // 驱动层读接口 
    int8_t (*recv)(uint8_t *buff, uint16_t *len);                   //　驱动层写接口
    int8_t reg_map_id;                                          // reserve for select register map table
    int8_t (*read_input)(stModbus_RTU_InputReader *reader);         //　应用层读输入寄存器回调
    int8_t (*read_hold)(stModbus_RTU_HoldReader *reader);           //　应用层读保持寄存器回调
    int8_t (*write_hold)(stModbus_RTU_HoldWriter *writer);          //　应用层写保持寄存器回调

}stModbus_RTU_Handler;

 
typedef struct Modbus_RTU_Handler_Attribute
{
    uint8_t dev_addr;
    emModebus_RTU_Mode mode;
    int8_t (*send)(uint8_t *buff, uint16_t len);                    
    int8_t (*recv)(uint8_t *buff, uint16_t *len);
    int8_t reg_map_id;                                          // reserve for select register map table
    int8_t (*read_input)(stModbus_RTU_InputReader *reader);
    int8_t (*read_hold)(stModbus_RTU_HoldReader *reader);
    int8_t (*write_hold)(stModbus_RTU_HoldWriter *writer);
    stModebus_RTU_Fun_Table *fun_table;                    // callback function table
    uint8_t fun_table_items;

}stModbus_RTU_Handler_Attr;


typedef struct Modbus_Interface_Bind
{
    emModebus_RTU_Bus bus;
    stModbus_RTU_Handler *handler;

}stModbus_Interface_Bind;




extern stModbus_Interface_Bind stModbus_Interface_Bind_Table[10];
#define MODBUS_INTERFACE_BIND_TABLE_ITEMS (sizeof(stModbus_Interface_Bind_Table)/sizeof(stModbus_Interface_Bind))


/* Exported functions prototypes ----------------------------------------------------------------*/
uint16_t modbus_crc_cal(uint8_t *buff, uint32_t len);

int8_t modbus_rtu_send(stModbus_RTU_Handler *handler, stModbus_RTU_Sender sender);
int8_t modbus_rtu_set_send(emModebus_RTU_Bus bus, int8_t (*send)(uint8_t *, uint16_t));
int8_t modbus_rtu_set_recv(emModebus_RTU_Bus bus, int8_t (*recv)(uint8_t *, uint16_t *));



/**
 * @brief  modbus rtu init
 * @param  handler: modbus rtu handler
 * @param  bus: modbus rtu bus
 * @param  attr: modbus rtu handler attr
 * 
 * @return 0: success, -1: fail
 */
int8_t modbus_rtu_init(stModbus_RTU_Handler *handler, emModebus_RTU_Bus bus, stModbus_RTU_Handler_Attr *attr);

/**
 * @brief  modbus rtu run
 * @param  handler: modbus rtu handler
 */
void modbus_rtu_run(stModbus_RTU_Handler *handler);

#ifdef __cplusplus
}
#endif

#endif
/******************************************* END OF FILE *****************************************/
