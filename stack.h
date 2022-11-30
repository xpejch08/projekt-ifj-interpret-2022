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


void DLL_Init( DLList *);

int DLL_InsertFirst( DLList *, token token );

int DLL_InsertLast( DLList *, token token );

void DLL_Free( DLList * );

void DLL_PrintLocal(DLList *);

void DLL_PrintGlobal(DLList *);

#endif