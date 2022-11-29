#ifndef STACK_H
#define STACK_H

#include "lexical.h"

typedef struct DLLElement {
	int type;
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

int DLL_InsertFirst( DLList *list, token token );

int DLL_InsertLast( DLList *list, token token );

void DLL_Free( DLList *list );

void DLL_PrintLocal(DLList *list);

void DLL_PrintGlobal(DLList *list);

#endif