#!/bin/bash
#
cd build || exit 1
make || exit 1
cd ../ || exit 1
./build/lab3
