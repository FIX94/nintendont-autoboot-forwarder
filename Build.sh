#!/bin/sh

make clean

cd app_booter
echo ""
echo Building App Booter
echo ""
make
mv app_booter.bin ../data

cd ..
echo ""
echo Building Loader
echo ""
make
