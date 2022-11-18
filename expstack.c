#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "expstack.h"
#include "expression.h"

void stackInit(stack *stack) {
	stack->top = NULL;
}
int stackPush(stack *stack, PrtableSymbolsEnum symbol) {
	StackElementPtr newElement = malloc(sizeof(struct StackElement));
	if (newElement == NULL) {
		return 1;
	}
	newElement->symbol = symbol;
	newElement->nextElement = stack->top;
	stack->top = newElement;
}
int stackPop(stack *stack, int n) {
	for (int i = 0; i<n; i++){
		if (stack->top == NULL){
			return 1;
		}
		StackElementPtr deletingElement = stack->top;
		stack->top = deletingElement->nextElement;
		free(deletingElement);
		return 0;
	}
}
StackElementPtr stackGetTopTerminal(stack *stack) {
	for(StackElementPtr elem = stack->top; elem != NULL; elem = elem->nextElement ){
		if (elem->symbol < STOPPER){
			return elem;
		}

	}
	return NULL;
}

int stackGetTopTerminalIndex(stack *stack, PrtableSymbolsEnum symbol) {
	
	StackElementPtr previousElement = NULL;

	for(StackElementPtr elem = stack->top; elem != NULL; elem = elem->nextElement ){
		if (elem->symbol < STOPPER){
			StackElementPtr newElement = malloc(sizeof(struct StackElement));
			if (newElement == NULL) {
				return 1;
			}
			newElement->symbol = symbol;
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

StackElementPtr stackGetTopSymbol(stack *stack) {
	return stack->top;
}

void stackDispose(stack *stack){
	
	while (1){
		stackPop(stack, 1);
	}
}