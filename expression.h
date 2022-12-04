//HEADER pro "expression.c"
#ifndef EXPRESSION_H 
#define EXPRESSION_H


#include "parser.h"
#include "expstack.h"

#include "lexical.h"
#include "symtable.h"

//s

typedef enum{
    EQUAL = 0,
    NOT_EQUAL,
    SMALLER_THAN,
    GREATER_THAN,
    SMALLER_OR_EQUAL,
    GREATER_OR_EQUAL,
    ADDITION,
    SUBTRACTION,
    CONCATENATE,
    MULTIPLY,
    DIVIDE,
    SINT,
    SFLOAT,
    SSTRING,
    VARIABLE,
    LBRACKET,
    RBRACKET,
    DOLLAR,
    SHIFT,
    NON_TERMINAL,
    SYMBOLSENUMERROR
} PrtableSymbolsEnum;

typedef enum{
    DATATYPE_INT,
    DATATYPE_FLOAT,
    DATATYPE_STRING,
    DATATYPE_NONE,
    DATATYPEENUM_ERROR,
    DATATYPE_ENDER
}DataTypeEnum;

typedef enum {

    RULE_EQUAL,
    RULE_NOT_EQUAL,
    RULE_SMALLER_THAN,
    RULE_GREATER_THAN,
    RULE_SMALLER_OR_EQUAL,
    RULE_GREATER_OR_EQUAL,
    RULE_ADDITION,
    RULE_SUBTRACTION,
    RULE_CONCATENATE,
    RULE_MULTIPLY,
    RULE_DIVIDE,
    RULE_BRACKETS,
    RULE_I,
    RULE_ERROR,
    RULESENUMERROR
} PrtableRulesEnum;

#endif
