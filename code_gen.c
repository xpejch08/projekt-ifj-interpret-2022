#include <stdio.h>
#include <stdlib.h>
#include "code_gen.h"
#include "lexical.h"

void instructionInit(TInstList *instrList)
{
    instrList->active = NULL;
    instrList->first = NULL;
    instrList->last = NULL;
}

void instructionFree(TInstList *instrList)
{
    
while(instrList->first != NULL)
{
    TItemList *tmp = instrList->first;
    instrList->first = instrList->first->next;
    free(tmp);
}
}