#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include "symtable.h"

void BVSInit(TRoot *SymTable){
    SymTable->rootPtr = NULL;
}

TNode *BVSCreate(token token){
    TNode *newPtr = malloc(sizeof(struct tnode));
    if(newPtr == NULL){
        fprintf(stderr, "Chyba pri alokaci prvku");
        return INT_ERROR; 
    }
    newPtr->leftPtr = NULL;
    newPtr->rightPtr = NULL;
    newPtr->type = token.type;
    newPtr->content = token.content;

    return newPtr;
}

TNode *BVSInsert(TNode *rootPtr, token token){
    if(rootPtr == NULL){
        return BVSCreate(token);
    }
    else{
        if((strCmpStr(&(token.content), &(rootPtr->content))) < 0){
            rootPtr->leftPtr = BVSInsert(rootPtr->leftPtr, token);
        }
        else if((strCmpStr(&(token.content), &(rootPtr->content))) > 0){
            rootPtr->rightPtr = BVSInsert(rootPtr->rightPtr, token);
        }
        return rootPtr;
    }
}

bool BVSSearch(TNode *rootPtr, token token){
    if(rootPtr == NULL){
        return false;
    }
    else{
        if((strCmpStr(&(token.content), &(rootPtr->content))) < 0){
            rootPtr->leftPtr = BVSSearch(rootPtr->leftPtr, token);
        }
        else if((strCmpStr(&(token.content), &(rootPtr->content))) < 0){
            rootPtr->rightPtr = BVSSearch(rootPtr->rightPtr, token);
        }
        return true;
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

