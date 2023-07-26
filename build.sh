#!/bin/bash

g++ nasmAPI/NASM_Interface.cpp nasmAPI/main.cpp -o llclnasm -g
g++ LLCL_lexer/main.cpp LLCL_lexer/lexer.cpp -o llcllexer -g
g++ LLCL_parser/main.cpp LLCL_parser/parser.cpp LLCL_lexer/lexer.cpp nasmAPI/NASM_Interface.cpp -o llclparser -g