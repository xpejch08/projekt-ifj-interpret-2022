#ifndef STACK_H
#define STACK_H

#include "lexical.h"

typedef struct element {
    tokenType type;
    tokenContent content;
    struct element *nextPtr; // pointer on following token
}tElement;

typedef struct stack
{
    tElement *topPtr; // pointer on top of the stack
}tStack;

void StackInit(tStack *stack);

void StackPush(tStack *stack, token token);

void StackPop(tStack *stack);

void StackFree(tStack *stack);

void StackTop(tStack *stack);

#endif