#include <stdio.h>
#include "../app/modbus_dev_pc_master.h"


int main() {
    
    modbus_dev_pc_master_init();

    while(1) {
        modbus_dev_pc_master_run();
    }




    return -1;

}