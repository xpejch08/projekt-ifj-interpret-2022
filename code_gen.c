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

TInst setActiveInstruction(int instrID, void *leftOp, void *rightOp)
{
    TInst *activeInstruction;
    activeInstruction->instrID = instrID;
    activeInstruction->leftOp = leftOp;
    activeInstruction->rightOp = rightOp;
    activeInstruction->result = NULL;
    return *activeInstruction;

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
        fprintf(stderr,"Chyba alokace prvku");
        return INT_ERROR;
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

void instructionActiveFirst(TInstList *instrList)
{
    if(instrList->first != NULL)
    {
        instrList->active = instrList->first;
    }
}

void instructionActiveNext(TInstList *instrList)
{
    if(instrList->first != NULL)
    {
        if(instrList->active->next != NULL)
        {
        instrList->active = instrList->active->next;
        }
    }
}

TInst *instructionGetData(TInstList *instrList)
{
    if(instrList->active != NULL)
    {
        return &(instrList->active->instruction);
    }
}