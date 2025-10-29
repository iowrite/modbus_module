#! /usr/bin/fish
echo "please input version:"
read ver
zip -r  modbus_module_$ver.zip src/ README.md  doc/ CHANGELOG 