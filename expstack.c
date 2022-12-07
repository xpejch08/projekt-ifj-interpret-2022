//////////////////////////////////////////////////////////////////
//    @@ Implementace překladače imperativního jazyka IFJ22 @@  //
//                                                              //
//       author: xpejch08 (Štěpán Pejchar)                      //
//       author: xceska07 (Ondřej Češka)                        //
//                                                              //
//                                                              //
//                                                              //
//////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "expstack.h"
#include "expression.h"

void stackInit(Stack *stack) {
    stack->top = NULL;
}
int stackPush(Stack *stack, PrtableSymbolsEnum symbol, DataTypeEnum datatype, string codename, int orig) {
    StackElement *newElement = malloc(sizeof(StackElement));

    if (newElement == NULL) {
        return 1;
    }

    newElement->codename = codename;
    newElement->symbol = symbol;
    newElement->datatype = datatype;
    newElement->nextElement = stack->top;
    newElement->orig = orig;
    stack->top = newElement;
    return 0;
}
int stackPop(Stack *stack, int n) {

    for (int i = 0; i<n; i++){
        if (!stack->top){
            return 1;
        }
        //StackElement *temp = stack->top;
        stack->top = stack->top->nextElement;
        //free(temp);
    }
    return 0;
}

StackElement* stackGetTopTerminal(Stack *stack) {
    StackElement *elem = stack->top;
    while(elem != NULL){
        if (elem->symbol < SHIFT){
            return elem;
        }
        elem = elem->nextElement;
    }
    return NULL;
}

int stackInsertShift(Stack *stack, PrtableSymbolsEnum symbol, DataTypeEnum datatype) {

    StackElement *previousElement = NULL;
    StackElement *elem = stack->top;
    while(elem != NULL){
        if (elem->symbol < SHIFT){
            StackElement *newElement = malloc(sizeof( struct StackElement));
            if (newElement == NULL) {
                return 1;
            }
            newElement->symbol = symbol;
            newElement->datatype = datatype;
            
            if (previousElement != NULL){
                newElement->nextElement = previousElement->nextElement;
                previousElement->nextElement = newElement;
            }
            else {
                newElement->nextElement = stack->top;
                stack->top = newElement;
            }
            return 0;
        }
        previousElement = elem;
        elem = elem->nextElement;
    }
    return 1;
}

StackElement *stackGetTop(Stack *stack) {
    return stack->top;
}

void stackDispose(Stack *stack){

    while (stack->top != NULL){
        stackPop(stack, 1);
    }
}

