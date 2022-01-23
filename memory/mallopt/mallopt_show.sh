#!/usr/bin/bash

gcc -o mallopt mallopt.c
./mallopt
echo "Check that the double free was detected"
echo "exporting MALLOC_CHECK_=1"
MALLOC_CHECK_=1 ./mallopt