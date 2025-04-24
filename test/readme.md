# how to test

## 1. create pty pair for test

```
socat -d -d pty,raw,echo=0 pty,raw,echo=0

```

## 2. start modbus server

```
modbus_test_slave  /dev/pts/x
modbus_test_master  /dev/pts/x

```