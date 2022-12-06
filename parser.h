#ifndef PARSER_H
#define PARSER_H

#include "lexical.h"
#include "str.h"
#include "stack.h"
#include "symtable.h"
#include "code_gen.h"

#define PARAM_FUNCTION      800 // case for checking arguments in declaration of function
#define PARAM_IF_WHILE      801  
#define PARAM_WRITE         802 // case for checking parameters in bult-in function write 
#define PARAM_READI         803 // case for checking parameters in bult-in function readi
#define PARAM_READS         804 // case for checking parameters in bult-in function reads
#define PARAM_READF         805 // case for checking parameters in bult-in function readf
#define PARAM_STRLEN        806 // case for checking parameters in bult-in function strlen
#define PARAM_SUBSTRING     807 // case for checking parameters in bult-in function substring
#define PARAM_ORD           808 // case for checking parameters in bult-in function ord
#define PARAM_CHR           809 // case for checking parameters in bult-in function chr
#define PARAM_RETURN        810 // case for checking expression after return
#define PARAM_FUNCTION_CALL 811 // case for checking parameters in function calls

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
#define EQ "EQ"
#define LT "LT"
#define GT "GT"
#define WRITE "WRITE"
#define READ "READ"

#define CREATEFRAME "CREATEFRAME"
#define PUSHFRAME "PUSHFRAME"
#define POPFRAME "POPFRAME"
#define DEFVAR "DEFVAR"
#define LABEL "LABEL"
#define JUMP "JUMP"
#define JUMPIFEQ "JUMPIFEQ"
#define JUMPIFNEQ "JUMPIFNEQ"
#define EXIT "EXIT"
#define CALL "CALL"
#define RETURN "RETURN"

#define STRING_TYPE "string"
#define INT_TYPE "int"
#define FLOAT_TYPE "float"
#define NIL "nil@nil"

#define STRLEN "STRLEN"
#define SUBSTRING "GETCHAR"
#define CONCAT "CONCAT"
#define INT2CHAR "INT2CHAR"
#define STRI2INT "STRI2INT"

//-------------------------------------//

/**
 * @brief function that handles all function calls and keyword calls
 * @param sToken active token passed around through all functions that need to work with it
 * @param fun_id stores information about the function we are currently in
 * @return returns SUCCESS if everything runs properly else returns coresponding error from errors.h
 */
int declrList(token *sToken, function_save *fun_id);

/**
 * @brief function that handles all the parts of code that aren't a declaration or a keyword call
 * @param sToken active token passed around through all functions that need to work with it
 * @param fun_id stores information about the function we are currently in
 * @return returns SUCCESS if everything runs properly else returns coresponding error from errors.h
 */
int statlist(token *sToken, function_save *fun_id);

/**
 * @brief recursive function that is called anytime we need to check the parametrs of a
 * functon or a keyword call, handles for example if condition
 * @param option option corresponding to what we need to handle for example PARAM_WHILE
 * @param repeat how many times we need to repeat a function, because it is called recursively
 * @param sToken token passed around through all functions that need to work with it
 * @param fun_id stores information about the function we are currently in
 * @return returns SUCCESS if everything runs properly else returns coresponding error from errors.h
 */
int parametrs(int option, int repeat, token *sToken, function_save *fun_id);

/**
 * @brief main function of parser, calls all other needed functions
 * @return returns value of first statlist call which can be SUCCESS or a error from errors.h
 */
int parse();


#endif //PROJEKT_IFJ_INTERPRET_2022_PARSER_H
