#!/bin/bash

mkdir build
cd ./build
cmake -G Ninja ..
ninja
cd ..


mv ./build/llclpp       ./llclpp
mv ./build/llcllexer    ./llcllexer
mv ./build/llclparser   ./llclparser
mv ./build/nasmapi      ./nasmapi

# g++ nasmAPI/NASM_Interface.cpp nasmAPI/main.cpp -o llclnasm -g
# g++ LLCL_lexer/main.cpp LLCL_lexer/lexer.cpp -o llcllexer -g
# g++ LLCL_parser/main.cpp LLCL_parser/parser.cpp LLCL_lexer/lexer.cpp nasmAPI/NASM_Interface.cpp -o llclparser -g
# g++ LLCL_preprocessor/preprocessor.cpp LLCL_lexer/lexer.cpp -o llclpp
