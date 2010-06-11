#!/bin/bash
export ONIONDIR=../build/
mkdir -p ../build/
(cd ../build/ && cmake .. && make -j3)
if [ "$?" == "0" ]; then
	../build/src/onion example.config
else
	exit 1
fi
