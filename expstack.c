#include <stdio.h>
#include <stdlib.h>
//#include <malloc.h>
#include "expstack.h"
#include "expression.h"

void stackInit(Stack *stack) {
	stack->top = NULL;
}
int stackPush(Stack *stack, PrtableSymbolsEnum symbol, DataTypeEnum datatype) {
	StackElementPtr newElement = malloc(sizeof(struct StackElement));
	if (newElement == NULL) {
		return 1;
	}
	newElement->symbol = symbol;
	newElement->datatype = datatype;
	newElement->nextElement = stack->top;
	stack->top = newElement;
	return 0;
}
int stackPop(Stack *stack, int n) {
	for (int i = 0; i<n; i++){
		if (stack->top == NULL){
			return 1;
		}
		StackElementPtr deletingElement = stack->top;
		stack->top = deletingElement->nextElement;
		free(deletingElement);
		
	}
	return 0;
}
StackElementPtr stackGetTopTerminal(Stack *stack) {
	for(StackElementPtr elem = stack->top; elem != NULL; elem = elem->nextElement ){
		if (elem->symbol < SHIFT){
			return elem;
		}

	}
	return NULL;
}

int stackInsertAfterTop(Stack *stack, PrtableSymbolsEnum symbol, DataTypeEnum datatype) {
	
	StackElementPtr previousElement = NULL;

	for(StackElementPtr elem = stack->top; elem != NULL; elem = elem->nextElement ){
		if (elem->symbol < SHIFT){
			StackElementPtr newElement = malloc(sizeof(struct StackElement));
			if (newElement == NULL) {
				return 1;
			}
			newElement->symbol = symbol;
			newElement->datatype = datatype;
			if (previousElement != NULL){
				newElement->nextElement = previousElement->nextElement;
				previousElement->nextElement = newElement;
			}
			else
			{
				newElement->nextElement = stack->top;
				stack->top = newElement;
			}
			return 0;
		}
		previousElement = elem;
	}
	return 1;
}

StackElementPtr stackGetTopSymbol(Stack *stack) {
	return stack->top;
}

void stackDispose(Stack *stack){

	while (1){
		stackPop(stack, 1);
	}
}

