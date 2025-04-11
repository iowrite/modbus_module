#include <stdio.h>
#include "../app/modbus_dev_pc_slave.h"


int main() {

    modbus_dev_pc_slave_init();

    while(1) {
        modbus_dev_pc_slave_run();
    }




    return -1;

}