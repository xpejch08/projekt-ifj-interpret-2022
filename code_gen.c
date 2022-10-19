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

void instructionInsertLast(TInstList *instrList, TInst I)
{
    TItemList *new = malloc(sizeof(TItemList));
    if(new == NULL)
    {
        return 1;
    }
    new->instruction = I;
    if (instrList->first != NULL)
    {
    instrList->last->next = new;
    instrList->last = new;
    }
    else{
        instrList->first = new;
    }
}

void instructionInsertFirst(TInstList *instrList, TInst I)
{
     TItemList *new = malloc(sizeof(TItemList));
    if(new == NULL)
    {
        return 1;
    }
    new->instruction = I;
    if (instrList->first != NULL)
    {
    new->next = instrList->first;
    instrList->first = new;
    }
    else{
        instrList->first = new;
    }
}