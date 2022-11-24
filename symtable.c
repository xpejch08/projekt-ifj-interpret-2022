#include <stdio.h>
#include <stdlib.h>
//#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include "symtable.h"


////////////VARIABLE///////////////////////////

void BVSInit(TRoot *SymTable){
    SymTable->rootPtr = NULL;
}

void BVSCreate(token token){
    TNode *newPtr = malloc(sizeof(struct tnode));
    if(newPtr == NULL){
        fprintf(stderr, "99");
        return; 
    }
    newPtr->leftPtr = NULL;
    newPtr->rightPtr = NULL;
    newPtr->type = token.type;
    newPtr->content = token.content;
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

void BVSDispose(TRoot *SymTable){
    if(SymTable->rootPtr != NULL){
        BVSDispose(SymTable->rootPtr->leftPtr);
        BVSDispose(SymTable->rootPtr->rightPtr);
        SymTable->rootPtr = NULL;
    }
    SymTable = NULL;
}

void BVSFree(TRoot *SymTable){
    if(SymTable->rootPtr != NULL){
        BVSFree(SymTable->rootPtr->rightPtr);
        BVSFree(SymTable->rootPtr->leftPtr);
        SymTable->rootPtr = NULL;
        free(SymTable->rootPtr);
    }
    free(SymTable);
}
//////////////////////////////////////////
///////////////FUNCTION//////////////////

void BVSInit_function(TRootf *SymTable){
    SymTable->rootPtr = NULL;
}

void BVSCreate_function(function_save token){
    TNodef *newPtr = malloc(sizeof(struct tnodef));
    if(newPtr == NULL){
        fprintf(stderr, "99");
        return; 
    }
    newPtr->leftPtr = NULL;
    newPtr->rightPtr = NULL;
    newPtr->content = token.content;
    newPtr->parameters = token.param_count;
    newPtr->return_type = token.ret_value;
}

void BVSInsert_function(TNodef *rootPtr, function_save token){
    if(rootPtr == NULL){
        BVSCreate_function(token);
    }
    else{
        if((strCmpStr(&(token.content), &(rootPtr->content))) < 0){
            BVSInsert_function(rootPtr->leftPtr, token);
        }
        else if((strCmpStr(&(token.content), &(rootPtr->content))) > 0){
            BVSInsert_function(rootPtr->rightPtr, token);
        }
    }
}

TNodef *BVSSearch_function(TNodef *rootPtr, token token){
    if(rootPtr == NULL){
        return NULL;
    }
    else{
        if((strCmpStr(&(token.content), &(rootPtr->content))) < 0){
            rootPtr->leftPtr = BVSSearch_function(rootPtr->leftPtr, token);
        }
        else if((strCmpStr(&(token.content), &(rootPtr->content))) < 0){
            rootPtr->rightPtr = BVSSearch_function(rootPtr->rightPtr, token);
        }
        return rootPtr;
    }
}

void BVSFree_function(TRootf *SymTable){
    if(SymTable->rootPtr != NULL){
        BVSFree_function(SymTable->rootPtr->rightPtr);
        BVSFree_function(SymTable->rootPtr->leftPtr);
        SymTable->rootPtr = NULL;
        free(SymTable->rootPtr);
    }
    free(SymTable);
}
//////////////////////////////////////////