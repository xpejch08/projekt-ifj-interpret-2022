cmake_minimum_required(VERSION 3.23)
project(testing2 C)

set(CMAKE_C_STANDARD 11)

include_directories(IFJ22_Tester-master)

add_executable(testing2
        errors.h
        expression.c
        expression.h
        expstack.c
        expstack.h
        lexical.c
        lexical.h
        main.c
        parser.c
        parser.h
        semantic.c
        semantic.h
        stack.c
        stack.h
        str.c
        str.h
        symtable.c
        symtable.h
        code_gen.c
        code_gen.h)
