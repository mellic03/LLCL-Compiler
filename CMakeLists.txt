cmake_minimum_required(VERSION 3.9)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_BUILD_PARALLEL_LEVEL 8)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wuninitialized -Winit-self -Wmaybe-uninitialized -Werror -fsanitize=undefined -fdiagnostics-color=always -Ofast -lstdc++fs")

add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



# project(nasmapi)
# add_executable(
#     ${PROJECT_NAME}
#     "src/common/tokenize.cpp"
#     "src/nasmAPI/NASM_Interface.cpp"
#     "src/nasmAPI/main.cpp"
# )


project(llclpp)
add_executable(
    ${PROJECT_NAME}
    "src/common/tokenize.cpp"
    "src/LLCL_preprocessor/preprocessor.cpp"
    "src/LLCL_preprocessor/main.cpp"
)


# project(llcllexer)
# add_executable(
#     ${PROJECT_NAME}
#     "src/common/tokenize.cpp"
#     "src/common/syntax.cpp"
#     "src/LLCL_lexer/lexer.cpp"
#     "src/LLCL_lexer/main.cpp"
# )


# project(llclparser)
# add_executable(
#     ${PROJECT_NAME}
#     "src/common/tokenize.cpp"
#     "src/common/syntax.cpp"
#     "src/nasmAPI/NASM_Interface.cpp"
#     "src/LLCL_parser/parser.cpp"
#     "src/LLCL_parser/main.cpp"
# )


project(llclc)
add_executable(
    ${PROJECT_NAME}
    "src/common/tokenize.cpp"
    "src/common/syntax.cpp"
    "src/LLCL_lexer/lexer.cpp"
    "src/nasmAPI/NASM_Interface.cpp"
    "src/LLCL_parser/function.cpp"
    "src/LLCL_parser/write.cpp"
    "src/LLCL_parser/parser.cpp"
    "src/main.cpp"
)

