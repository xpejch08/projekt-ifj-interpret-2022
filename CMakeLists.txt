cmake_minimum_required(VERSION 3.23)
project(projekt_ifj_interpret_2022 C)

set(CMAKE_C_STANDARD 99)

include_directories(.)

add_executable(projekt_ifj_interpret_2022
        code_gen.c
        code_gen.h
        errors.h
        lexical.c
        lexical.h
        parser.c
        parser.h
        stack.c
        stack.h
        str.c
        str.h
        symtable.c
        symtable.h)
