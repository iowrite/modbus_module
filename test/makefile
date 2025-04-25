.PHONY:  all clean 


all: 	build_dir \
		modbus_test_slave \
		modbus_test_master \
		libmodbus_module.a

build_dir:
	mkdir -p build_dir
clean:
	rm  -rf build_dir \
		modbus_test_slave \
		modbus_test_master \
		libmodbus_module.a

modbus_test_slave: 	libmodbus_module.a\
					build_dir/test_pc_slave.o \
					build_dir/modbus_dev_pc_slave.o 
	gcc build_dir/test_pc_slave.o \
		build_dir/modbus_dev_pc_slave.o \
		-L.\
		-lmodbus_module \
		-o modbus_test_slave -g



modbus_test_master: 	libmodbus_module.a\
						build_dir/test_pc_master.o \
						build_dir/modbus_dev_pc_master.o 
	gcc build_dir/test_pc_master.o \
		build_dir/modbus_dev_pc_master.o \
		-L.\
		-lmodbus_module \
		-o modbus_test_master -g


libmodbus_module.a: 	build_dir/modbus_port.o \
						build_dir/modbus_rtu_master.o \
						build_dir/modbus_rtu_slave.o \
						build_dir/modbus_rtu.o 
	ar rcs 	libmodbus_module.a		build_dir/modbus_port.o \
									build_dir/modbus_rtu_master.o \
									build_dir/modbus_rtu_slave.o \
									build_dir/modbus_rtu.o 








build_dir/modbus_port.o: ../src/modbus_port.c ../src/modbus_port.h ../src/modbus_config.h ../src/modbus_rtu.h
	gcc -c  ../src/modbus_port.c -o build_dir/modbus_port.o -g

build_dir/modbus_rtu_master.o: ../src/modbus_rtu_master.c ../src/modbus_rtu_master.h ../src/modbus_config.h ../src/modbus_rtu.h
	gcc -c  ../src/modbus_rtu_master.c -o build_dir/modbus_rtu_master.o -g

build_dir/modbus_rtu_slave.o: ../src/modbus_rtu_slave.c ../src/modbus_rtu_slave.h ../src/modbus_config.h ../src/modbus_rtu.h
	gcc -c  ../src/modbus_rtu_slave.c -o build_dir/modbus_rtu_slave.o -g

build_dir/modbus_rtu.o: ../src/modbus_rtu.c ../src/modbus_rtu.h ../src/modbus_config.h
	gcc -c  ../src/modbus_rtu.c -o build_dir/modbus_rtu.o -g

build_dir/modbus.o: ../src/modbus.c ../src/modbus.h ../src/modbus_config.h ../src/modbus_rtu.h
	gcc -c  ../src/modbus.c -o build_dir/modbus.o -g

build_dir/modbus_dev_pc_slave.o: ../app/modbus_dev_pc_slave.c ../app/modbus_dev_pc_slave.h ../src/modbus_config.h ../src/modbus_rtu.h
	gcc -c  ../app/modbus_dev_pc_slave.c -o build_dir/modbus_dev_pc_slave.o -g

build_dir/modbus_dev_pc_master.o: ../app/modbus_dev_pc_master.c ../app/modbus_dev_pc_master.h ../src/modbus_config.h ../src/modbus_rtu.h
	gcc -c  ../app/modbus_dev_pc_master.c -o build_dir/modbus_dev_pc_master.o -g

build_dir/test_pc_slave.o: test_pc_slave.c ../src/modbus_config.h ../src/modbus_rtu.h
	gcc -c  test_pc_slave.c -o build_dir/test_pc_slave.o -g

build_dir/test_pc_master.o: test_pc_master.c ../src/modbus_config.h ../src/modbus_rtu.h
	gcc -c  test_pc_master.c -o build_dir/test_pc_master.o -g





