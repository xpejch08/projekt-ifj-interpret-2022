#ifndef PARSER_H
#define PARSER_H

#include "lexical.h"
#include "str.h"
#include "stack.h"
#include "symtable.h"
#include "code_gen.h"

#define PARAM_FUNCTION      800
#define PARAM_IF_WHILE      801
#define PARAM_WRITE         802
#define PARAM_READI         803
#define PARAM_READS         804
#define PARAM_READF         805
#define PARAM_STRLEN        806
#define PARAM_SUBSTRING     807
#define PARAM_ORD           808
#define PARAM_CHR           809
#define PARAM_RETURN        810
#define PARAM_FUNCTION_CALL 811

//instructions
//-----------------------------------//
#define MOVE "MOVE"
#define ADD "ADD"
#define SUB "SUB"
#define MUL "MUL"
#define DIV "DIV"
#define IDIV "IDIV"
#define PUSH "PUSH"
#define POP "POP"
#define LT "LT"
#define GT "GT"
#define WRITE "WRITE"
#define READ "READ"

#define CREATEFRAME "CRATEFRAME"
#define PUSHFRAME "PUSHFRAME"
#define POPFRAME "POPFRAME"
#define DEFVAR "DEFVAR"
#define LABEL "LABEL"
#define JUMP "JUMP"
#define JUMPIFEQ "JUMPIFEQ"
#define JUMPIFNEQ "JUMPIFNEQ"
#define EXIT "EXIT"

#define STRING_TYPE "string"
#define INT_TYPE "int"
#define FLOAT_TYPE "float"
#define NIL "nil@nil"

#define STRLEN "STRLEN"

//-------------------------------------//

int declrList(token *sToken, function_save *fun_id);
int statlist(token *sToken, function_save *fun_id);
int parametrs(int option, int repeat, token *sToken, function_save *fun_id);
int program();
int parse();


#endif //PROJEKT_IFJ_INTERPRET_2022_PARSER_H
