#ifndef EXPSTACK_H
#define EXPSTACK_H

#include "lexical.h"
#include "expression.h"
#include "symtable.h"
#include "parser.h"
/*
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
*/