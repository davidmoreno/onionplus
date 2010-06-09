#!/bin/sh
export ONIONDIR=../build/
mkdir -p ../build/
(cd ../build/ && cmake .. && make)
../build/src/onion example.config
