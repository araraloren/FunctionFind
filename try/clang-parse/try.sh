#!/usr/bin/env sh

mkdir build
cd build
cmake ..
make
cp fftry ..
cd ..
rm -rf build
exec "./fftry"
