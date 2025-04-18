#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <sys/time.h>
#include "../app/modbus_dev_pc_master.h"
#include "../src/modbus.h"



int main(int argc, char *argv[]) {
        
    printf("open %s as master \n", argv[1]);
    modbus_dev_pc_master_init(argv[1]);
    struct timeval tv; 
    gettimeofday(&tv, NULL);
    time_t now_ms = tv.tv_sec*1000 + tv.tv_usec/1000;
    time_t last_call_ms = now_ms;
    bool send = false;
    uint16_t reg_data;
    while(1) {
        modbus_dev_pc_master_run();
        gettimeofday(&tv, NULL);
        now_ms = tv.tv_sec*1000 + tv.tv_usec/1000;

        if(now_ms -last_call_ms > 1000)
        {
            last_call_ms = now_ms;
            modbus_rtu_read_hold(emModebus_RTU_Bus_PC, 0x01, 0x0000, 1, &reg_data);
            send = true;
        }
        if(send){
            int8_t ret = modbus_rtu_opt_status(emModebus_RTU_Bus_PC);
            if(ret == 0)
            {
                printf("master poll data success: %d %d\n", reg_data>>8, (uint8_t)reg_data);
                send = false;
            }else if(ret == 2)
            {
                send = false;
                printf("master poll data time out\n");
            }
        }
 
    }

    return -1;

}