#!/bin/bash

# Building App for linux systems

rm -rf build
cmake -S . -B build
cmake --build build
