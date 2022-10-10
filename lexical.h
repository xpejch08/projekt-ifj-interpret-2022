#ifndef LEXICAL_H
#define LEXICAL_H

#include "str.h"
#include "errors.h"

typedef enum {
    KEYWORD_ELSE, //100
    KEYWORD_IF, //101
    KEYWORD_FUNCTION, //102
    KEYWORD_FLOAT, //103
    KEYWORD_INT, //104
    KEYWORD_NULL, //105
    KEYWORD_RETURN,//106
    KEYWORD_STRING, //107
    KEYWORD_VOID, //108
    KEYWORD_WHILE //109
}keyword;

typedef enum{
    TYPE_INITIAL,
    TYPE_KEYWORD,
    TYPE_VARIABLE,
    TYPE_INTEGER_NUMBER,
    TYPE_DOUBLE_NUMBER,
    TYPE_EXPONENT_NUMBER,
    TYPE_IDENTIFIER,
    TYPE_STRING,
    TYPE_MULTIPLY,
    TYPE_DIVIDE,
    TYPE_ADDITION,
    TYPE_SUBTRACTION,
    TYPE_CONCATENATE,
    TYPE_SMALLER_THAN,
    TYPE_GREATER_THAN,
    TYPE_SMALLER_OR_EQUAL,
    TYPE_GREATER_OR_EQUAL,
    TYPE_EQUAL,
    TYPE_NOT_EQUAL,
    TYPE_ASSIGN


}tokenType;

typedef union{
    int integerNumber;
    double doubleNumber;
    string *str;
    keyword keyword;
}tokenContent;

typedef struct{
    tokenContent content;
    tokenType type;
}token;


void setSourceFile(FILE *f);
int getNextToken(token *attr);
void initToken(token *attr);
#endif