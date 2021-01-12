#!/bin/sh
if [ $# -ge 1 ]; then
	echo $1 \; `objdump -h $1|grep .text|awk --non-decimal-data '{print ("0x"$3)+0}'`
else
	echo USAGE: $0 executable or object file
fi
