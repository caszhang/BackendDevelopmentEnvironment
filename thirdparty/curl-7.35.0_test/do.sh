#!/bin/sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.:../curl-7.35.0/lib 
g++ -g -I ../curl-7.35.0/include/ -L ../curl-7.35.0/lib/ -lcurl main.cc 
