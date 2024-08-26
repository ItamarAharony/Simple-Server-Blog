#!/bin/bash

cd server
make clean
make
cd ..
./server/server 

