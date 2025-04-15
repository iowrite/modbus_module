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
 #ifndef __MODBUS__PORT_PC_MASTER_H__
 #define __MODBUS__PORT_PC_MASTER_H__
 
 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
 /* Includes -------------------------------------------------------------------------------------*/
 #include <stdint.h>
 /* Exported defines -----------------------------------------------------------------------------*/
 /*modbus Indicates the function length of a frame*/
 
 
 /* Exported typedef -----------------------------------------------------------------------------*/
 
 
 
 
 
 /* Exported functions prototypes ----------------------------------------------------------------*/
 int8_t modbus_dev_pc_master_init();
 int8_t modbus_dev_pc_master_run();
 
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif /* __C_TEMPLATE_H__ */
 /******************************************* END OF FILE *****************************************/
