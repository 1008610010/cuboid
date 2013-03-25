#!/bin/sh

test -d 'build' || mkdir -p 'build'

cd build
cmake ..

cd ../nginx-1.3.14
./configure --add-module=../ngx_leveldb
make

