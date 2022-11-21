#include <stdio.h>
#include <stdlib.h>
#include "code_gen.h"
#include "lexical.h"


//test

TInst setActiveInstruction(void* instrID ,void* symb1, void* symb2, void* var, int uniqueNumber)
{   
    TInst activeInstruction;
    if(instrID != 0){
        activeInstruction.instrID = instrID;
    }
    if(symb1 != NULL)
    {
        activeInstruction.symb1 = symb1;
    }
    if(symb2 != NULL)
    {
        activeInstruction.symb2 = symb2;
    }
    if(var != NULL)
    {
        activeInstruction.var = var;
    }
    if(uniqueNumber != NULL)
    {
        activeInstruction.uniqueNumber = uniqueNumber;
    }
    return activeInstruction;
}


void sortInstructions(TInst* activeInstruction, TInst* activeInstruction2 ){
    
    if(activeInstruction->instrID == MULS || activeInstruction->instrID == DIVS ){
        return;
    }
    
    if(activeInstruction->instrID == ADDS || activeInstruction->instrID == SUBS){
        
        if(activeInstruction2->instrID == MULS || activeInstruction2 == DIVS){

            activeInstruction->rightOp = activeInstruction2->result;
        }
        else
        {
            return;
        }
    }
    if(activeInstruction->instrID == LTS || activeInstruction->instrID == GTS){

        if(activeInstruction2->instrID == ADDS || activeInstruction2->instrID == SUBS || activeInstruction2->instrID == MULS || activeInstruction2->instrID == DIVS){
            
            activeInstruction->rightOp = activeInstruction2->result;
        }
    }
    // TODO KONKATENACE ???

}


void generateInstruction(TInst activeInstruction, int addrprev, int addract, int addrnext)
{   

if(addrprev == NULL)
{
    if(addract == NULL && addrnext == NULL && addrprev == NULL)
    {
        instructionInsertLast(list, activeInstruction);
    }
//TODO ?

}

}

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

void instructionInsertActive(TInstList *instrList, TInst instruction)
{
    TItemList *newInst = malloc(sizeof(TItemList));
    if(newInst == NULL)
    {
        return INT_ERROR;
    }
    newInst->activeInstruction = instruction;
    if (instrList->first != NULL)
    {
    instrList->active->next = newInst;
    instrList->active = newInst;
    }
    else{
        instrList->first = newInst;
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
        return &(instrList->active->activeInstruction);
    }
}

void instructionPrint(TInstList *instrList)
{   
    if(instrList->first != NULL)
    {
        while(instrList->active != NULL)
        {
            if(instrList->active->activeInstruction.symb1 == NULL &&
             instrList->active->activeInstruction.symb2 == NULL &&
              instrList->active->activeInstruction.var == NULL)
            {
                printf("%s\n", instrList->active->activeInstruction.instrID);
            }

            if(instrList->active->activeInstruction.symb1 != NULL &&
             instrList->active->activeInstruction.symb2 == NULL &&
              instrList->active->activeInstruction.var == NULL &&
              instrList->active->activeInstruction.uniqueNumber != NULL)
            {
                printf("%s %s%d\n", instrList->active->activeInstruction.instrID,instrList->active->activeInstruction.symb1,
                 instrList->active->activeInstruction.uniqueNumber);
            }
             if(instrList->active->activeInstruction.symb1 != NULL &&
             instrList->active->activeInstruction.symb2 == NULL &&
              instrList->active->activeInstruction.var == NULL &&
              instrList->active->activeInstruction.uniqueNumber == NULL)
            {
                printf("%s %s\n", instrList->active->activeInstruction.instrID,instrList->active->activeInstruction.symb1);
            }

            if(instrList->active->activeInstruction.symb1 != NULL &&
             instrList->active->activeInstruction.symb2 == NULL &&
              instrList->active->activeInstruction.var != NULL)
            {
                printf("%s %s %s\n", instrList->active->activeInstruction.instrID,
                 instrList->active->activeInstruction.var, instrList->active->activeInstruction.symb1);
            }

            if(instrList->active->activeInstruction.symb1 != NULL &&
             instrList->active->activeInstruction.symb2 != NULL &&
              instrList->active->activeInstruction.var != NULL)
            {
                printf("%s %s %s %s\n", instrList->active->activeInstruction.instrID,
                 instrList->active->activeInstruction.var, instrList->active->activeInstruction.symb1,
                  instrList->active->activeInstruction.symb2);
            }
            instrList->active = instrList->active->next;
        }
    }
}
