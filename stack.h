#ifndef STACK_H
#define STACK_H

#include "lexical.h"

typedef struct DLLElement {
	tokenType type;
    tokenContent content;
	struct DLLElement *previousElement;
	struct DLLElement *nextElement;
} *DLLElementPtr;

typedef struct {
	DLLElementPtr firstElement;
	DLLElementPtr activeElement;
	DLLElementPtr lastElement;
} DLList;

void DLL_Init( DLList *list );

void DLL_InsertFirst( DLList *list, token token );

void DLL_InsertLast( DLList *list, token token );

void DLL_Free( DLList *list );

#endif