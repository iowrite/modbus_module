# modbus module自测报告

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
    Modebus_RTU_Erno_FRAME_FORMAT_ERROR = 13,           // 自定义错误码：帧格式错误
    Modebus_RTU_Erno_MASTER_BUS_BUSY = 14,              // 自定义错误码：主机模式总线忙
    Modebus_RTU_Erno_END,
  
}emModebus_RTU_Erno;
```

## 2. 测试用例

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
