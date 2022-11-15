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

TInst *setActiveInstruction(int instrID, void *leftOp, void *rightOp)
{
    TInst *activeInstruction;
    activeInstruction->instrID = instrID;
    activeInstruction->leftOp = leftOp;
    activeInstruction->rightOp = rightOp;
    activeInstruction->result = NULL;
    return activeInstruction;
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

void instructionInsertActive(TInstList *instrList, TInst *instruction)
{
    TItemList *newInst = malloc(sizeof(TItemList));
    if(newInst == NULL)
    {
        fprintf(stderr,"Chyba alokace prvku");
        return INT_ERROR;
    }
    newInst->instruction = instruction;
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
        return &(instrList->active->instruction);
    }
}

void instructionArgs(TInstList *instrList, TInst *instruction)
{
    //TInst *instruction;
    instrList->active = instruction;
 if(instrList->active != NULL)
 {
     switch (instruction->instrID)
     {
     case 400: // MOVE
     if(instruction->leftOp == NULL || instruction->rightOp == NULL)
     {
        return INCOMPLETE;
     }
     else
      return COMPLETE;

     case 401: // ADD
            if(instruction->leftOp == NULL || instruction->rightOp == NULL)
            {
                return INCOMPLETE;
            }
     else
      return COMPLETE;

     case 402: // SUB
             if(instruction->leftOp == NULL || instruction->rightOp == NULL)
            {
                return INCOMPLETE;
            }
     else
      return COMPLETE; 

     /*case 403: // MUL
             if(instruction.leftOp == NULL || instruction.rightOp == NULL)
            {
                return INCOMPLETE;
            }
     else
      return COMPLETE;

     case 404: // DIV
             if(instruction.leftOp == NULL || instruction.rightOp == NULL)
            {
                return INCOMPLETE;
            }

            if(&instruction.leftOp != )
       

      case 405: // IDIV
       if(instruction.leftOp == NULL || instruction.rightOp == NULL)
            {
                return INCOMPLETE;
            }
     else
      return COMPLETE;

     case 406: // PUSH
             if(instruction.leftOp == NULL || instruction.rightOp == NULL)
            {
                return INCOMPLETE;
            }
     else
      return COMPLETE;
     case 407: // POP
             if(instruction.leftOp == NULL || instruction.rightOp == NULL)
            {
                return INCOMPLETE;
            }
     else
      return COMPLETE;
      */
     case 408: // LT

     case 409: // GT

     case 410: // WRITE
     default:
        break;
     }
 }
}