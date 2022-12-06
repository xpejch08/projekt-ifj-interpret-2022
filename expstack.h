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

int prtableDataTypeToTokenType(DataTypeEnum type);


PrtableRulesEnum pickRule(StackElement *op1, StackElement *op2, StackElement *op3);
DataTypeEnum getDataType(token *sToken, TRoot *someTree);
DataTypeEnum checkTypeForRule(PrtableRulesEnum rule, StackElement *op1, StackElement *op2, StackElement *op3);
int countSymbols(Stack *stack);
DataTypeEnum reduceExpression(Stack *stack, bool in_function);

int precedenceAction(TRoot *someTree, token *sToken, Stack *stack, bool in_function,int iforass, bool* chooseexp);

#endif