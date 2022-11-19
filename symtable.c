#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include "symtable.h"

void BVSInit(TRoot *SymTable){
    SymTable->rootPtr = NULL;
}

void BVSCreate(token token, int param, int return_value){
    TNode *newPtr = malloc(sizeof(struct tnode));
    if(newPtr == NULL){
        return INT_ERROR; 
    }
    newPtr->leftPtr = NULL;
    newPtr->rightPtr = NULL;
    newPtr->type = token.type;
    newPtr->content = *token.content;
    newPtr->parameters = param;
    newPtr->return_type = return_value;
}

void BVSInsert(TNode *rootPtr, token token, int param, int return_value){
    if(rootPtr == NULL){
        BVSCreate(token, param, return_value);
    }
    else{
        if((strCmpStr(&(token.content), &(rootPtr->content))) < 0){
            BVSInsert(rootPtr->leftPtr, token, param, return_value);
        }
        else if((strCmpStr(&(token.content), &(rootPtr->content))) > 0){
            BVSInsert(rootPtr->rightPtr, token, param, return_value);
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

