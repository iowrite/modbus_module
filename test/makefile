.PHONY:  all clean 


all: build_dir modbus_test

build_dir:
	mkdir -p build_dir
clean:
	rm  -rf build_dir

modbus_test: 	build_dir/test.o \
				build_dir/modbus_port.o \
				build_dir/modbus_rtu_master.o \
				build_dir/modbus_rtu_slave.o \
				build_dir/modbus_rtu.o \
				build_dir/modbus_dev_pc_slave.o \

	gcc build_dir/test.o \
		build_dir/modbus_port.o \
		build_dir/modbus_rtu_master.o \
		build_dir/modbus_rtu_slave.o \
		build_dir/modbus_rtu.o \
		build_dir/modbus_dev_pc_slave.o \
			-o modbus_test -g






build_dir/modbus_port.o: ../src/modbus_port.c ../src/modbus_port.h
	gcc -c  ../src/modbus_port.c -o build_dir/modbus_port.o -g

build_dir/modbus_rtu_master.o: ../src/modbus_rtu_master.c ../src/modbus_rtu_master.h 
	gcc -c  ../src/modbus_rtu_master.c -o build_dir/modbus_rtu_master.o -g

build_dir/modbus_rtu_slave.o: ../src/modbus_rtu_slave.c ../src/modbus_rtu_slave.h
	gcc -c  ../src/modbus_rtu_slave.c -o build_dir/modbus_rtu_slave.o -g

build_dir/modbus_rtu.o: ../src/modbus_rtu.c ../src/modbus_rtu.h
	gcc -c  ../src/modbus_rtu.c -o build_dir/modbus_rtu.o -g

build_dir/modbus.o: ../src/modbus.c ../src/modbus.h
	gcc -c  ../src/modbus.c -o build_dir/modbus.o -g

build_dir/modbus_dev_pc_slave.o: ../app/modbus_dev_pc_slave.c ../app/modbus_dev_pc_slave.h
	gcc -c  ../app/modbus_dev_pc_slave.c -o build_dir/modbus_dev_pc_slave.o -g

build_dir/test.o: test.c
	gcc -c  test.c -o build_dir/test.o -g





