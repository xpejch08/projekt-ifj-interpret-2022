#ifndef LEXICAL_H
#define LEXICAL_H

#include "str.h"
#include "errors.h"

#define   KEYWORD_ELSE          100
#define   KEYWORD_IF            101
#define   KEYWORD_FUNCTION      102
#define   KEYWORD_FLOAT         103
#define   KEYWORD_INT           104
#define   KEYWORD_NULL          105
#define   KEYWORD_RETURN        106
#define   KEYWORD_STRING        107
#define   KEYWORD_VOID          108
#define   KEYWORD_WHILE         109




#define   TYPE_INITIAL           110
#define   TYPE_VARIABLE          112
#define   TYPE_INTEGER_NUMBER    113
#define   TYPE_DOUBLE_NUMBER     114
#define   TYPE_EXPONENT_NUMBER   115
#define   TYPE_IDENTIFIER        116
#define   TYPE_STRING            117
#define   TYPE_MULTIPLY          118
#define   TYPE_DIVIDE            119
#define   TYPE_ADDITION          120
#define   TYPE_SUBTRACTION       121
#define   TYPE_CONCATENATE       122
#define   TYPE_SMALLER_THAN      123
#define   TYPE_GREATER_THAN      124
#define   TYPE_SMALLER_OR_EQUAL  125
#define   TYPE_GREATER_OR_EQUAL  126
#define   TYPE_EQUAL             127
#define   TYPE_NOT_EQUAL         128
#define   TYPE_ASSIGN            129
#define   TYPE_END_OF_FILE       130
#define   TYPE_LVINCULUM         131
#define   TYPE_RVINCULUM         132
#define   TYPE_LBRACKET          133
#define   TYPE_RBRACKET          134
#define   TYPE_SEMICOLON         135
#define   TYPE_COLON             136
#define   TYPE_COMMA             137
#define   TYPE_SAME_DATA         138
#define   TYPE_NOT_SAME_DATA     139


typedef union{
    string *integerNumber;
    string *doubleNumber;
    string *str;
}tokenContent;

typedef struct{
    tokenContent content;
    int type;
}token;


void setSourceFile(FILE *f);
int getNextToken(token *attr);
void initToken(token *attr);

#endif
