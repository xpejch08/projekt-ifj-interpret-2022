#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include "symtable.h"

void BVSInit(TRoot *SymTable){
    SymTable->rootPtr = NULL;
}
/////////////////// VARIABLE //////////////////////
void BVSCreate(token token){
    TNode *newPtr = malloc(sizeof(struct tnode));
    if(newPtr == NULL){
        return INT_ERROR; 
    }
    newPtr->leftPtr = NULL;
    newPtr->rightPtr = NULL;
    newPtr->type = token.type;
    newPtr->content = *token.content;
}

void BVSInsert(TNode *rootPtr, token token){
    if(rootPtr == NULL){
        BVSCreate(token);
    }
    else{
        if((strCmpStr(&(token.content), &(rootPtr->content))) < 0){
            BVSInsert(rootPtr->leftPtr, token);
        }
        else if((strCmpStr(&(token.content), &(rootPtr->content))) > 0){
            BVSInsert(rootPtr->rightPtr, token);
        }
    }
}

TNode *BVSSearch(TNode *rootPtr, token token){
    if(rootPtr == NULL){
        return NULL;
    }
    else{
        if((strCmpStr(&(token.content), &(rootPtr->content))) < 0){
            rootPtr->leftPtr = BVSSearch(rootPtr->leftPtr, token);
        }
        else if((strCmpStr(&(token.content), &(rootPtr->content))) < 0){
            rootPtr->rightPtr = BVSSearch(rootPtr->rightPtr, token);
        }
        return rootPtr;
    }
}

void BVSFree(TRoot *SymTable, TNode *rootPtr){
    if(rootPtr != NULL){
        BVSFree(SymTable ,rootPtr->rightPtr);
        BVSFree(SymTable, rootPtr->leftPtr);
        free(rootPtr);
    }
    free(SymTable);
}
//////////////////////////////////////////
///////////////FUNCTION//////////////////

void BVSCreatef(function_save token){
    TNodef *newPtr = malloc(sizeof(struct tnodef));
    if(newPtr == NULL){
        return INT_ERROR; 
    }
    newPtr->leftPtr = NULL;
    newPtr->rightPtr = NULL;
    newPtr->content = token.content;
    newPtr->parameters = token.param_count;
    newPtr->return_type = token.ret_value;
}

void BVSInsertf(TNodef *rootPtr, function_save token){
    if(rootPtr == NULL){
        BVSCreatef(token);
    }
    else{
        if((strCmpStr(&(token.content), &(rootPtr->content))) < 0){
            BVSInsertf(rootPtr->leftPtr, token);
        }
        else if((strCmpStr(&(token.content), &(rootPtr->content))) > 0){
            BVSInsertf(rootPtr->rightPtr, token);
        }
    }
}

TNodef *BVSSearchf(TNodef *rootPtr, token token){
    if(rootPtr == NULL){
        return NULL;
    }
    else{
        if((strCmpStr(&(token.content), &(rootPtr->content))) < 0){
            rootPtr->leftPtr = BVSSearchf(rootPtr->leftPtr, token);
        }
        else if((strCmpStr(&(token.content), &(rootPtr->content))) < 0){
            rootPtr->rightPtr = BVSSearchf(rootPtr->rightPtr, token);
        }
        return rootPtr;
    }
}

void BVSFreef(TRootf *SymTable, TNodef *rootPtr){
    if(rootPtr != NULL){
        BVSFreef(SymTable ,rootPtr->rightPtr);
        BVSFreef(SymTable, rootPtr->leftPtr);
        free(rootPtr);
    }
    free(SymTable);
}
//////////////////////////////////////////