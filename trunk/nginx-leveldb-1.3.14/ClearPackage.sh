#!/bin/sh

if [ -f Makefile ]; then
    make clean
fi

rm -f doc/html/*

rm -rf CMakeCache.txt CMakeFiles DartTestfile.txt Makefile Testing install_manifest.txt cmake_install.cmake progress.make *.so
rm build-stamp configure-stamp
rm -rf debian/nginx-leveldb
rm -rf debian/tmp
rm debian/nginx-leveldb.debhelper.log
rm debian/nginx-leveldb.substvars

