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
    eModebus_RTU_Erno_START = -1,
    eModebus_RTU_Erno_SUCCESS = 0,
    eModebus_RTU_Erno_FUN_CODE_NOT_FOUND = 1,            // std error code
    eModebus_RTU_Erno_REG_ADDR_INVALID = 2,              // std error code
    eModebus_RTU_Erno_REG_VALUE_INVALID = 3,             // std error code
    eModebus_RTU_Erno_PERMISSION_DENIED = 12,        //Custom error codes: wirte more than once to a  hold register that can only be written once, or wirte a hold register that can only be read.
    eModebus_RTU_Erno_FRAME_FORMAT_ERROR = 13,           // 自定义错误码：帧格式错误                 // master use too
    eModebus_RTU_Erno_MASTER_BUS_BUSY = 14,              // 自定义错误码：主机模式总线忙         
    eModebus_RTU_Erno_MASTER_REQUEST_ADDR_NOT_MATCH = 15, // 自定义错误码：主机模式请求地址与当前地址不匹配
    eModebus_RTU_Erno_MASTER_REQUEST_FUN_NOT_FOUND = 16, // 自定义错误码：未找到主机发送函数
    eModebus_RTU_Erno_MASTER_PARSE_FUN_NOT_FOUND = 17, // 自定义错误码：未找到主机解析函数
    eModebus_RTU_Erno_SLAVE_PARSE_FUN_NOT_FOUND = 18, // 自定义错误码：未找到从机解析函数
    eModebus_RTU_Erno_END,
}eModebus_RTU_Erno_def;

typedef enum eModbus_RTU_State_def
{
    eModbus_RTU_State_Init,
    eModbus_RTU_State_IDLE,
    eModbus_RTU_State_Receive,
    eModbus_RTU_State_Send,
    eModbus_RTU_State_NUM
}eModbus_RTU_State_def;


typedef enum Modebus_RTU_Mode
{
    eModebus_RTU_Mode_Slave,
    eModebus_RTU_Mode_Master,
    eModebus_RTU_Mode_NUM
}eModebus_RTU_Mode_def;

typedef struct Modbus_RTU_InputReader
{
    uint8_t ucReg_map_id;
    uint8_t ucReg_num;
    uint16_t usReg_addr;
    union{
        uint16_t usReg_data[128];
        uint8_t ucReg_data_byte[256];
    };
}stModbus_RTU_InputReader_def;

typedef struct Modbus_RTU_HoldReader
{
    uint8_t ucReg_map_id;
    uint16_t usReg_addr;
    uint8_t ucReg_num;
    union{
        uint16_t usReg_data[128];
        uint8_t ucReg_data_byte[256];
    };
}stModbus_RTU_HoldReader_def;

typedef struct Modbus_RTU_HoldWriter
{
    uint8_t ucReg_map_id;
    uint16_t usReg_addr;
    uint8_t ucReg_num;
    union{
        uint16_t usReg_data[128];
        uint8_t ucReg_data_byte[256];
    };

}stModbus_RTU_HoldWriter_def;

typedef struct Modbus_RTU_Sender
{
    uint8_t ucDev_addr;
    uint8_t ucFun_code;
    uint16_t usReg_addr;
    uint8_t ucReg_num;
    uint16_t usReg_data[128];
}stModbus_RTU_Sender_def;


typedef struct Modbus_RTU_Handler stModbus_RTU_Handler_def;                         // 前向声明 stModbus_RTU_Handler 与 Modbus_RTU_Handler 交叉声明
typedef struct Modebus_RTU_Fun_Table
{
    uint8_t ucFcode;
    int8_t (*pRequest_f)(stModbus_RTU_Handler_def *handler, stModbus_RTU_Sender_def *sender);
    int8_t (*pSlave_parse_f)(stModbus_RTU_Handler_def *handler, uint8_t *buff, uint16_t len);
    int8_t (*pMaster_parse_f)(stModbus_RTU_Handler_def *handler, uint8_t *buff, uint16_t len);

}stModebus_RTU_Fun_Table_def;

typedef struct Modbus_RTU_Handler
{
    eModbus_RTU_State_def eState, eLast_state;               // pSend_f/receive switch judge
    eModebus_RTU_Mode_def eMode, eLast_mode;                 // slave/master switch judge
    uint8_t ucTx_buff[300];
    uint16_t usTx_len;
    uint8_t ucRx_buff[300];
    uint16_t ucRx_len;
    uint32_t uiLast_call_tick;
    stModebus_RTU_Fun_Table_def *pstFun_table;                    // callback function table
    uint8_t ucFun_table_items;
    int8_t (*pSend_f)(uint8_t *buff, uint16_t len);                   // 驱动层读接口 
    int8_t (*pRecv_f)(uint8_t *buff, uint16_t *len);                   //　驱动层写接口
    union 
    {
        struct                                          // slave eMode use only
        {
            uint8_t ucDev_addr;
            int8_t ucReg_map_id;                                          // reserve for select register map table
            int8_t (*pRead_input_def)(stModbus_RTU_InputReader_def *reader);         //　应用层读输入寄存器回调
            int8_t (*pRead_hold_def)(stModbus_RTU_HoldReader_def *reader);           //　应用层读保持寄存器回调
            int8_t (*pWrite_hold_def)(stModbus_RTU_HoldWriter_def *writer);          //　应用层写保持寄存器回调
        };
        struct{                                                // master eMode use only
            uint16_t *pusMaster_parse_addr;                             // master parse address(master eMode use only)
            uint32_t uiMaster_Wait_Count;                              // master wait pRecv_f time count(master eMode use only)
            uint32_t uiMaster_Wait_Recv_Limt;                         // master wait pRecv_f limit(master eMode use only)
            uint8_t ucMaster_request_addr;                               // master pRequest_f code(master eMode use only)
            uint8_t ucMaster_request_code;                               // master pRequest_f code(master eMode use only)
            uint8_t ucMaster_request_rw_len;                             // master pRequest_f read/write len(master eMode use only)
        };
    };
}stModbus_RTU_Handler_def;

 
typedef struct Modbus_RTU_Handler_Attribute
{
    uint8_t ucDev_addr;
    eModebus_RTU_Mode_def eMode;
    int8_t (*pSend_f)(uint8_t *buff, uint16_t len);                    
    int8_t (*pRecv_f)(uint8_t *buff, uint16_t *len);
    int8_t ucReg_map_id;                                          // reserve for select register map table
    int8_t (*pRead_input_def)(stModbus_RTU_InputReader_def *reader);
    int8_t (*pRead_hold_def)(stModbus_RTU_HoldReader_def *reader);
    int8_t (*pWrite_hold_def)(stModbus_RTU_HoldWriter_def *writer);
    stModebus_RTU_Fun_Table_def *pstFun_table;                    // callback function table
    uint8_t ucFun_table_items;
    uint16_t usMaster_recv_wait_limt;
}stModbus_RTU_Handler_Attr;


typedef struct Modbus_Interface_Bind
{
    eModebus_RTU_Bus_def eBus;
    stModbus_RTU_Handler_def *pstHandler;

}stModbus_Interface_Bind_def;




extern stModbus_Interface_Bind_def stModbus_Interface_Bind_Table[10];
#define MODBUS_INTERFACE_BIND_TABLE_ITEMS (sizeof(stModbus_Interface_Bind_Table)/sizeof(stModbus_Interface_Bind_def))


/* Exported functions prototypes ----------------------------------------------------------------*/
uint16_t modbus_crc_cal(uint8_t *buff, uint32_t len);

int8_t modbus_rtu_send(stModbus_RTU_Handler_def *handler, stModbus_RTU_Sender_def sender);
int8_t modbus_rtu_set_send(eModebus_RTU_Bus_def eBus, int8_t (*pSend_f)(uint8_t *, uint16_t));
int8_t modbus_rtu_set_recv(eModebus_RTU_Bus_def eBus, int8_t (*pRecv_f)(uint8_t *, uint16_t *));



/**
 * @brief  modbus rtu init
 * @param  handler: modbus rtu handler
 * @param  eBus: modbus rtu eBus
 * @param  attr: modbus rtu handler attr
 * 
 * @return 0: success, -1: fail
 */
int8_t modbus_rtu_init(stModbus_RTU_Handler_def *handler, eModebus_RTU_Bus_def eBus, stModbus_RTU_Handler_Attr *attr);

/**
 * @brief  modbus rtu run
 * @param  handler: modbus rtu handler
 */
void modbus_rtu_run(stModbus_RTU_Handler_def *handler);

#ifdef __cplusplus
}
#endif

#endif
/******************************************* END OF FILE *****************************************/
