 /**
  *************************************************************************************************
  * @file name : c_template.c
  * @encoding  : UTF-8
  * @author    : HDY(803098)
  * @version   : V0.0.1
  * @date      : 2025.1.1
  * @brief     : This file provides
  * @attention 
  * Copyright (c) 2024 HOYPower.
  * All rights reserved.
  *************************************************************************************************
  * @Edit History
  * 时间       >> 版本   >> 姓名（工号） >> 修改内容
  * 2025.12.12 >> V0.0.1 >> zzy(801922) >> 创建文件
  *************************************************************************************************
  */
/* Includes -------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <time.h>
#include <sys/time.h>
#include "../src/modbus_config.h"
#include "../src/modbus_rtu.h"
#include "../src/modbus_rtu_master.h"
#include "../src/modbus_rtu_slave.h"
#include "../src/modbus_port.h"

/* Private define -------------------------------------------------------------------------------*/

/* Private typedef ------------------------------------------------------------------------------*/
/* Private functions prototypes -----------------------------------------------------------------*/
int8_t uart_pc_send(uint8_t *buff, uint16_t len);
int8_t uart_pc_recv(uint8_t *buff, uint16_t *len);
int8_t rtu_pc_read_input(stModbus_RTU_InputReader *reader);
int8_t rtu_pc_read_hold(stModbus_RTU_HoldReader *reader);
int8_t rtu_pc_write_hold(stModbus_RTU_HoldWriter *writer);


/* Private variable -----------------------------------------------------------------------------*/
int g_fd;
stModebus_RTU_Fun_Table rtu_fun_table_pc_master[] =
{
    {0x03, modbus_fun_request_03, NULL, modbus_fun_parse_03_master},                // std: read hold 
    {0x04, modbus_fun_request_04, NULL, modbus_fun_parse_04_master},                                                     // std: read input
    {0x06, modbus_fun_request_06, NULL, modbus_fun_parse_06_master},                                                     // std: wirte single hold
    {0x10, modbus_fun_request_10, NULL, modbus_fun_parse_10_master},                                                     // std: write multi  hold

};
#define RTU_FUN_TABLE_ITEMS (sizeof(rtu_fun_table_pc_master) / sizeof(stModebus_RTU_Fun_Table))

stModbus_RTU_Handler stModbus_RTU_Handler_PC_master ;
stModbus_RTU_Handler_Attr rtu_pc_attr_master = {
    .mode = emModebus_RTU_Mode_Master,
    .send = uart_pc_send,
    .recv = uart_pc_recv,
    .fun_table = rtu_fun_table_pc_master,
    .fun_table_items = RTU_FUN_TABLE_ITEMS,
};



/* Global  variable -----------------------------------------------------------------------------*/
/* Extern  variable -----------------------------------------------------------------------------*/
/* Function body --------------------------------------------------------------------------------*/
/**
  *************************************************************************************************
  * @Function    : main
  * @Description : 
  * @Parameter   : 
  * @Return      : 
  * @Author      : zzy(801922)
  * @Date        : 2024.11.24
  *************************************************************************************************
  * @Edit History
  * 时间       >> 姓名（工号）>> 修改内容
  * 2025.12.12 >> zzy(801922) >> 创建函数
  *************************************************************************************************
  */





  void print_time_ms(void)
  {
  
      struct timeval tv;
      struct tm *tm_info;
      char time_str[64];
  
      // 获取当前时间（秒 + 微秒）
      gettimeofday(&tv, NULL);
  
      // 转换为本地时间（tm 结构体）
      tm_info = localtime(&tv.tv_sec);
  
      // 格式化时间字符串（精确到秒）
      strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);
  
      // 打印时间
      mylog("[%s.%03ld] --> ", time_str, tv.tv_usec/1000);
  
  }



int8_t uart_pc_send(uint8_t *buff, uint16_t len)
{
    print_time_ms();
    fflush(stdout);
    int ret = write(g_fd, buff, len);
    fsync(g_fd);
    
    for (size_t i = 0; i < len; i++)
    {
        mylog("%02x ", buff[i]);
    }
    mylog("\n");
    
    if(ret < 0)
    {
        return -1;
    }else {
        return 0;
    }
}

int8_t uart_pc_recv(uint8_t *buff, uint16_t *len)
{
    int8_t ret = 0;

    static uint8_t recv_buff[256];
    static uint16_t recv_idx;
    static uint32_t last_read_time_ms, now_ms;

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(g_fd, &fds);
    #define BUF_SIZE 256
    uint8_t read_buf[BUF_SIZE];
    memset(read_buf, 0, BUF_SIZE);
    struct timeval timeout = { .tv_sec = 0, .tv_usec = 0 };
    if (select(g_fd + 1, &fds, NULL, NULL, &timeout) > 0) {
        ssize_t n = read(g_fd, read_buf, BUF_SIZE);
        if (n > 0) {
            struct timeval tv; 
            gettimeofday(&tv, NULL);
            last_read_time_ms = tv.tv_sec*1000+tv.tv_usec/1000;
            memcpy(recv_buff+recv_idx, read_buf, n);
            recv_idx += n;
        }
    }
    struct timeval tv; 
    gettimeofday(&tv, NULL);
    now_ms = tv.tv_sec*1000+tv.tv_usec/1000;
    if(now_ms - last_read_time_ms > 3)
    {
        if(recv_idx > 0)
        {
            print_time_ms();
            for(int i=0; i<recv_idx; i++)
            {
                mylog("%02x ", recv_buff[i]);
            }
            mylog("\n");
            memcpy(buff, recv_buff, recv_idx);
            *len = recv_idx;
            ret = recv_idx;
            recv_idx = 0;
        }
    }
    return ret;
}




int8_t rtu_pc_read_hold(stModbus_RTU_HoldReader *reader)
{

}

int8_t rtu_pc_read_input(stModbus_RTU_InputReader *reader)
{

}


int8_t rtu_pc_write_hold(stModbus_RTU_HoldWriter *writer)
{

}

int8_t modbus_dev_pc_master_init(char *dev_name)
{
    g_fd = open(dev_name, O_RDWR|O_NOCTTY);  
    if (g_fd == -1) {
        perror("open serial port");
        return -1;
    }
    modbus_rtu_init(&stModbus_RTU_Handler_PC_master,  emModebus_RTU_Bus_PC, &rtu_pc_attr_master);
}

int8_t modbus_dev_pc_master_run()
{
    modbus_rtu_run(&stModbus_RTU_Handler_PC_master);
}

int8_t modbus_dev_pc_master_exit()
{
    close(g_fd);
}






/******************************************* END OF FILE *****************************************/
