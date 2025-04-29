# modbus module自测报告

[test environment ](../test/readme.md)

## 1. 错误码

```
typedef enum Modebus_RTU_Erno
{
    Modebus_RTU_Erno_START = -1,
    Modebus_RTU_Erno_SUCCESS = 0,
    Modebus_RTU_Erno_FUN_CODE_NOT_FOUND = 1,            // std error code
    Modebus_RTU_Erno_REG_ADDR_INVALID = 2,              // std error code
    Modebus_RTU_Erno_REG_VALUE_INVALID = 3,             // std error code
    Modebus_RTU_Erno_PERMISSION_DENIED = 12,        //Custom error codes: wirte more than once to a  hold register that can only be written once, or wirte a hold register that can only be read.
    Modebus_RTU_Erno_FRAME_FORMAT_ERROR = 13,           // 自定义错误码：帧格式错误                 // master use too
    Modebus_RTU_Erno_MASTER_BUS_BUSY = 14,              // 自定义错误码：主机模式总线忙   
    Modebus_RTU_Erno_MASTER_REQUEST_ADDR_NOT_MATCH = 15, // 自定义错误码：主机模式请求地址与当前地址不匹配
    Modebus_RTU_Erno_MASTER_REQUEST_FUN_NOT_FOUND = 16, // 自定义错误码：未找到主机发送函数
    Modebus_RTU_Erno_MASTER_PARSE_FUN_NOT_FOUND = 17, // 自定义错误码：未找到主机解析函数
    Modebus_RTU_Erno_SLAVE_PARSE_FUN_NOT_FOUND = 18, // 自定义错误码：未找到从机解析函数


    Modebus_RTU_Erno_END,
  
}emModebus_RTU_Erno;
```

## 2. 从机模式测试用例

| id | function code | serve mod | case           | input                                                | output                                               | result | note                                           |
| -- | :------------ | --------- | -------------- | ---------------------------------------------------- | ---------------------------------------------------- | ------ | ---------------------------------------------- |
|    | /             | slave     | crc 校验错误   | [01][03][00][02][00][01][25][CB]                     | 无                                                   | v      |                                                |
|    | /             | salve     | 地址不匹配     | 02 03 00 02 00 01 25 F9                              | 无                                                   | v      |                                                |
|    | 0xff          | slave     | 不支持的功能码 | 01 FF 00 02 00 01 75 DE                              | 01 FF 01 A0 30                                       | v      |                                                |
|    | 0x03          | slave     | 正常读取       | [01][03][00][02][00][01][25][CA]                     | <01><03><02><00><00>`<B8>`<44>                     | v      |                                                |
|    |               | slave     | 地址不合法     | [01][03][00][00][00][0A][C5][CD]                     | <01><83><02>`<C0><F1>`                             | v      | 包括读多个寄存器,起始地址合法,加上长度后不合法 |
|    | 0x06          | slave     | 正常写入       | [01][06][00][69][00][7B][19][F5]                     | <01><06><00><69><00><7B><19>`<F5>`                 | v      |                                                |
|    |               | slave     | 地址不合法     | [01][06][00][78][00][7B][49][F0]                     | <01><86><02>`<C3><A1>`                             | v      |                                                |
|    |               | slave     | 数据不合法     | [01][06][00][69][04][63][1B][3F]                     | <01><86><03><02><61>                                 | v      |                                                |
|    | 0x10          | slave     | 正常写入       | [01][10][00][69][00][02][04][00][7B][00][7C][44][15] | <01><10><00><69><00><02><04><00><7B><00><7C><44><15> | v      |                                                |
|    |               |           |                |                                                      |                                                      |        |                                                |
|    |               |           |                |                                                      |                                                      |        |                                                |

## 3. 主机模式测试用例

| id | function code | serve mode | case              | send                    | call function         | receive                    | status function       | return value | result | note     |
| -- | ------------- | ---------- | ----------------- | ----------------------- | --------------------- | -------------------------- | --------------------- | ------------ | ------ | -------- |
|    | 03            | master     | 正常读取:单个     | 01 03 00 69 00 01 54 16 | modbus_rtu_read_hold  | 01 03 02 00 05 78 47       | modbus_rtu_opt_status | 0            | v      |          |
|    |               | master     | 正常读取:多个     | 01 03 00 69 00 02 14 17 | modbus_rtu_read_hold  | 01 03 04 00 05 00 06 6a 30 | modbus_rtu_opt_status | 0            | v      |          |
|    |               | master     | 无回复            | 01 03 00 69 00 01 54 16 | modbus_rtu_read_hold  | 无                         | modbus_rtu_opt_status | -1           | v      | 超时失败 |
|    |               | master     | 地址无效/长度过长 | 01 03 00 69 00 0a 15 d1 | modbus_rtu_read_hold  | 01 83 02 c0 f1             | modbus_rtu_opt_status | -1           | v      |          |
|    | 06            | master     | 正常写入          | 01 06 00 69 00 1e d9 de | modbus_rtu_write_hold | 01 06 00 69 00 1e d9 de    | modbus_rtu_opt_status | 0            | v      |          |
|    |               | master     | 无回复            | 01 06 00 69 00 1e d9 de | modbus_rtu_write_hold | 无                         | modbus_rtu_opt_status | -1           | v      | 超时失败 |
|    | 10            | master     | 正常写入一个      |                         |                       |                            |                       |              |        |          |
|    |               | master     | 正常写入多个      |                         |                       |                            |                       |              |        |          |
|    |               | master     | 地址无效/长度过长 |                         |                       |                            |                       |              |        |          |
|    |               | master     | 写入数据无效      |                         |                       |                            |                       |              |        |          |
|    |               | master     | 无回复            |                         |                       |                            |                       |              |        |          |
|    |               |            |                   |                         |                       |                            |                       |              |        |          |
