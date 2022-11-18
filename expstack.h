#ifndef EXPSTACK_H
#define EXPSTACK_H

#include "lexical.h"
#include "expression.h"
typedef struct StackElement {
	
    PrtableSymbolsEnum symbol;
	//struct stack *previousElement;
	struct StackElement *nextElement;
} *StackElementPtr;

typedef struct ExpStack{
	StackElementPtr top;
} stack;


void stackInit( stack *stack );

void stackInsertFirst( stack *stack, token token );

int stackPush( stack *stack, PrtableSymbolsEnum symbol );

int stackPop( stack *stack, int n ); //kolikrat popnout

StackElementPtr stackGetTopSymbol( stack *stack );

int insertAfterTop( stack *stack, PrtableSymbolsEnum symbol );

StackElementPtr stackGetTopTerminal( stack *stack );

void stackDispose( stack *stack );



#endif