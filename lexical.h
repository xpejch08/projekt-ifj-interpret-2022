#ifndef LEXICAL_H
#define LEXICAL_H

#include "str.h"
#include "errors.h"

/**
 * defining all possible tokens
 */

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
#define   TYPE_KEYWORD           111
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
#define   TYPE_FUNCTIONDECLARE   138
#define   TYPE_EPILOG            139


/**
 * tokenContent is a structure containing the string value of a token
 */
typedef union{
    string *integerNumber;
    string *doubleNumber;
    string *str;
}tokenContent;

typedef struct{
    string *content;
    int param_count;
    int ret_value;
}function_save;

/**
 * token structure, every token has a type defined above and it's content
 * defined in tokenContent structure
 */
typedef struct{
    tokenContent content;
    int type;
}token;

/**
 * @brief function for setting a FILE
 * @param f pointer to a file
 */
void setSourceFile(FILE *f);

/**
 * @brief main function of lexical analysis, sets token structure depending on the input from stdin
 * @param attr attr is type token defined above
 * @return returns SUCCESS if it finds a legitimate token else returns lex error
 * parses stdin input into separate tokens
 */
int getNextToken(token *attr);

//function for initializing a token
void initToken(token *attr);

/**
 * @brief function that checks beginning of the input for prolog, function calls getNextToken
 * @param str passing a initialized token to the function because of getNextToken
 * @return returns SUCCESS if prolog is correct else returns SYN_ERROR
 */
int prolog(token *str);

#endif
