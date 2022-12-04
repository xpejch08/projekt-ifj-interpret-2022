#ifndef EXPSTACK_H
#define EXPSTACK_H

#include "expression.h"


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

PrtableSymbolsEnum prtableTokenToSymbol(token *sToken);

/*PrtableIndexEnum prtableSymbolToIndex(PrtableSymbolsEnum symb);
PrtableRulesEnum pickRule(StackElementPtr op1, StackElementPtr op2, StackElementPtr op3);
DataTypeEnum getDataType(token *sToken, TRoot *mainTree);
int checkTypeForRule(PrtableRulesEnum rule, StackElementPtr op1, StackElementPtr op2, StackElementPtr op3, DataTypeEnum* resulttype);
int countSymbols(Stack stack);
int reduceExpression(Stack stack);*/

int precedenceAction(TRoot *someTree, token *sToken, Stack stack);

#endif