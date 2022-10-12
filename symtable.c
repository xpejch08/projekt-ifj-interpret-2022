#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "symtable.h"

void BVSInit(TRoot *SymTable){
    SymTable->rootPtr = NULL;
}

TNode *BVSInsert(TNode *rootPtr, token token){
    TNode *newPtr = malloc(sizeof(struct tnode));
    rootPtr->leftPtr = NULL;
    rootPtr->rightPtr = NULL;
    rootPtr->type = token.type;

    return newPtr;
}
// TODO musime se domluvit podle ceho budeme vyhledavat
/*
TNode *BVSSearch(TNode *rootPtr, token token){
    
}
*/
void BVSFree(TRoot *SymTable, TNode *rootPtr){
    if(rootPtr != NULL){
        BVSFree(SymTable ,rootPtr->rightPtr);
        BVSFree(SymTable, rootPtr->leftPtr);
        free(rootPtr);
    }
    free(SymTable);
}

