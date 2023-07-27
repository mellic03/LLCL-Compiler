cmake_minimum_required(VERSION 3.9)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_BUILD_PARALLEL_LEVEL 8)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wuninitialized -Winit-self -Wmaybe-uninitialized -Werror -fsanitize=undefined -fdiagnostics-color=always -Ofast -lstdc++fs")

add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



project(nasmapi)
add_executable(
    ${PROJECT_NAME}
    "common/tokenize.cpp"
    "nasmAPI/NASM_Interface.cpp"
    "nasmAPI/main.cpp"
)


project(llclpp)
add_executable(
    ${PROJECT_NAME}
    "common/tokenize.cpp"
    "LLCL_preprocessor/main.cpp"
)


project(llcllexer)
add_executable(
    ${PROJECT_NAME}
    "common/tokenize.cpp"
    "LLCL_lexer/lexer.cpp"
    "LLCL_lexer/main.cpp"
)


project(llclparser)
add_executable(
    ${PROJECT_NAME}
    "common/tokenize.cpp"
    "LLCL_lexer/lexer.cpp"
    "nasmAPI/NASM_Interface.cpp"
    "LLCL_parser/parser.cpp"
    "LLCL_parser/main.cpp"
)
