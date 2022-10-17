#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "stack.h"

/**
 * @brief function creates new node in SymTable
 * @param token token from which we put data
 */
void StackInit(tStack *stack){
    stack->topPtr = NULL;
}
/**
 * @brief function creates new node in SymTable
 * @param token token from which we put data
 * @return returns newly created node
 */
void StackPush(tStack *stack, token token){
    tElement *element = malloc(sizeof(tElement));
    if(element ==NULL){
        fprintf(stderr, "Chyba pri alokaci prvku");
        return INT_ERROR; 
    }
    element->nextPtr = stack->topPtr;
    stack->topPtr = element;
    stack->topPtr->content = token.content;
    stack->topPtr->type = token.type;
}

void StackPop(tStack *stack){
    tElement *element;
    if (stack->topPtr != NULL) {
        element = stack->topPtr;
        stack->topPtr = stack->topPtr->nextPtr;
        free(element);
    }
}

void StackFree(tStack *stack){
    if(stack->topPtr != NULL){
        tElement *remove = stack->topPtr;
        while(remove != NULL)
        {
            remove = remove->nextPtr;
            free(remove);
        }
    }
}

void StackTop(tStack *stack){
    return (stack->topPtr);
}