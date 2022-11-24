//HEADER pro "expression.c"
#ifndef EXPRESSION_H 
#define EXPRESSION_H

#include "parser.h"
//#include "expstack.h"
#include "lexical.h"
#include "symtable.h"
#include "parser.c"
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
    SINT,
    SFLOAT,
    SSTRING,
    IDENTIFIER,
    LBRACKET,
    RBRACKET,
    DOLLAR,
    STOPPER,
    NON_TERMINAL
} PrtableSymbolsEnum;

typedef enum{
    DATATYPE_INT,
    DATATYPE_FLOAT,
    DATATYPE_STRING,
    DATATYPE_ERROR
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
    RULE_ERROR
} PrtableRulesEnum;

//EXPSTACK FUNKCE
typedef struct StackElement {
	
    PrtableSymbolsEnum symbol;
	DataTypeEnum datatype;
	struct StackElement *nextElement;
} *StackElementPtr;

typedef struct ExpStack{
	StackElementPtr top;
} Stack;


void stackInit(Stack *stack );

int stackPush( Stack *stack, PrtableSymbolsEnum symbol, DataTypeEnum datatype );

int stackInsertAfterTop( Stack *stack, PrtableSymbolsEnum symbol, DataTypeEnum datatype );

int stackPop( Stack *stack, int n ); //kolikrat popnout

StackElementPtr stackGetTopSymbol( Stack *stack );

StackElementPtr stackGetTopTerminal( Stack *stack );

void stackDispose( Stack *stack );


#endif
