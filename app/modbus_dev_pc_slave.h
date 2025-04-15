 #include <stdint.h>
 #ifndef __MODBUS__PORT_PC_SLAVE_H__
 #define __MODBUS__PORT_PC_SLAVE_H__
 
 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
 /* Includes -------------------------------------------------------------------------------------*/

 /* Exported defines -----------------------------------------------------------------------------*/
 /*modbus Indicates the function length of a frame*/
 
 
 /* Exported typedef -----------------------------------------------------------------------------*/
 
 
 
 
 
 /* Exported functions prototypes ----------------------------------------------------------------*/
 int8_t modbus_dev_pc_slave_init(char *dev_name);
 int8_t modbus_dev_pc_slave_run();
 
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif /* __C_TEMPLATE_H__ */
 /******************************************* END OF FILE *****************************************/
