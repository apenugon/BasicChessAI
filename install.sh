#!/bin/sh

make clean;

if [ "$1" != "" ]; then
    make profile;
    echo n | ./AIProject a a;
    make optbin;
else
    make;
fi
