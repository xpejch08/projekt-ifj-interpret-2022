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

/**
 * @brief function function initializes new stack
 * @param stack pointer to first element
 */
void StackInit(tStack *stack);
/**
 * @brief function pushes on top new element
 * @param stack stack
 * @param token token from which we insert data
 */
void StackPush(tStack *stack, token token);
/**
 * @brief function pops element from top
 * @param stack stack
 */
void StackPop(tStack *stack);
/**
 * @brief function which free whole stack
 * @param stack stack
 */
void StackFree(tStack *stack);
/**
 * @brief function returns top of stack
 * @param stack stack
 * @return returns head of stack
 */
void StackTop(tStack *stack);

#endif