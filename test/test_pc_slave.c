#include <stdio.h>
#include "../app/modbus_dev_pc_slave.h"


int main(int argc, char *argv[]) {
    
    printf("open %s as slave \n", argv[1]);
    modbus_dev_pc_slave_init(argv[1]);

    while(1) {
        modbus_dev_pc_slave_run();
    }

    return -1;

}