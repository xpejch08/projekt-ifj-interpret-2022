#ifndef EXPSTACK_H
#define EXPSTACK_H
#include <stdbool.h>
#include "expression.h"
#include "str.h"


//EXPSTACK FUNKCE
typedef struct StackElement {
	
    PrtableSymbolsEnum symbol;
	DataTypeEnum datatype;
    string codename;
    int orig;
	struct StackElement *nextElement;
} StackElement;


typedef struct{
	StackElement *top;
} Stack;


void stackInit(Stack *stack );

int stackPush( Stack *stack, PrtableSymbolsEnum symbol, DataTypeEnum datatype, string codename, int orig );

int stackInsertAfterTopTerminal( Stack *stack, PrtableSymbolsEnum symbol, DataTypeEnum datatype, string codename, int orig );

int stackPop( Stack *stack, int n ); //kolikrat popnout

StackElement* stackGetTop( Stack *stack );

StackElement* stackGetTopTerminal( Stack *stack );

void stackDispose( Stack *stack );

PrtableSymbolsEnum prtableTokenToSymbol(token *sToken, int iforass);

/*PrtableIndexEnum prtableSymbolToIndex(PrtableSymbolsEnum symb);
PrtableRulesEnum pickRule(StackElementPtr op1, StackElementPtr op2, StackElementPtr op3);
DataTypeEnum getDataType(token *sToken, TRoot *mainTree);
int checkTypeForRule(PrtableRulesEnum rule, StackElementPtr op1, StackElementPtr op2, StackElementPtr op3, DataTypeEnum* resulttype);
int countSymbols(Stack stack);
int reduceExpression(Stack stack);*/

int precedenceAction(TRoot *someTree, token *sToken, Stack *stack, bool in_function,int iforass);

#endif