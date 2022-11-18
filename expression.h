//HEADER pro "expression.c"
//ifndef pro zaruceni idempotence
#ifndef EXPRESSION_H 
#define EXPRESSION_H

#include "parser.h"

//s

typedef enum{
    EQUAL, //TODO!!!!!!! : SPATNA LEXICAL!
    NOT_EQUAL, ///TODO -||-
    SMALLER_THAN,
    GREATER_THAN,
    SMALLER_OR_EQUAL,
    GREATER_OR_EQUAL,
    ADDITION,
    SUBTRACTION,
    CONCATENATE,
    MULTIPLY,
    DIVIDE,
    INT,
    FLOAT,
    STRING,
    IDENTIFIER,
    LBRACKET,
    RBRACKET,
    STOPPER,
    NON_TERMINAL,
    DOLLAR
} PrtableSymbolsEnum;


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
    RULE_ERROR
} PrtableRulesEnum;


























#endif