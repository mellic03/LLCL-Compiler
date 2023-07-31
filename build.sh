#!/bin/bash

mkdir build
cd ./build
cmake -G Ninja ..
ninja
cd ..


mv ./build/llclpp       ./llclpp
# mv ./build/llcllexer    ./llcllexer
# mv ./build/llclparser   ./llclparser
# mv ./build/nasmapi      ./nasmapi

mv ./build/llclc ./llclc

