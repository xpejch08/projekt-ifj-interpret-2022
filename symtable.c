#include <stdio.h>
#include <stdlib.h>
//#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include "symtable.h"
#include "parser.h"


////////////VARIABLE///////////////////////////

void BVSInit(TRoot *SymTable){
    SymTable->rootPtr = NULL;
}

TNode *BVSInsert(TNode *rootPtr, token token){
    TNode *tmp = rootPtr;
    TNode *insert = malloc((sizeof (TNode)));
    string *insertStr = malloc(sizeof (string));
    insert->leftPtr = NULL;
    insert->rightPtr = NULL;
    insert->content = insertStr;
    strCpyStr(insert->content, token.content.str);
    insert->type = KEYWORD_NULL;
    if(!tmp){
        rootPtr = insert;
        return rootPtr;
    }
    else {
        if ((strCmpStr(token.content.str, rootPtr->content)) < 0) {
            rootPtr->leftPtr =  BVSInsert(rootPtr->leftPtr, token);
            return rootPtr;
        } else if ((strCmpStr(token.content.str,rootPtr->content)) > 0) {
            rootPtr->rightPtr =  BVSInsert(rootPtr->rightPtr, token);
            return rootPtr;
        }
    }
    return rootPtr;
}


TNode *BVSSearch(TNode *rootPtr, token token){
    if(rootPtr == NULL){
        return NULL;
    }
    else{
        if((strCmpStr(token.content.str, rootPtr->content)) == 0){
            return rootPtr;
        }
        else if((strCmpStr(token.content.str, rootPtr->content)) < 0){
            return BVSSearch(rootPtr->leftPtr, token);
        }
        else if((strCmpStr(token.content.str, rootPtr->content)) > 0){
            return BVSSearch(rootPtr->rightPtr, token);
        }
        return NULL;
    }
}

void BVSDisposeNode(TNode *rootPtr){
    if(rootPtr != NULL){
        BVSDisposeNode(rootPtr->leftPtr);
        BVSDisposeNode(rootPtr->rightPtr);
        rootPtr = NULL;
    }
}

void BVSFreeNode(TNode *rootPtr){
    if(rootPtr != NULL){
        BVSFreeNode(rootPtr->leftPtr);
        BVSFreeNode(rootPtr->rightPtr);
        free(rootPtr);
    }
}

void BVSDispose(TRoot *SymTable){
    BVSDisposeNode(SymTable->rootPtr);
    SymTable = NULL;
}

void BVSFree(TRoot *SymTable){
    BVSFreeNode(SymTable->rootPtr);
    free(SymTable);
}
//////////////////////////////////////////
///////////////FUNCTION//////////////////

void BVSInit_function(TRootf *SymTable){
    SymTable->rootPtr = NULL;
}

TNodef *BVSInsert_function(TNodef *rootPtr, function_save token){
    TNodef *tmp = rootPtr;
    TNodef *insert = malloc((sizeof (TNode)));
    string *insertStr = malloc(sizeof (string));
    insert->leftPtr = NULL;
    insert->rightPtr = NULL;
    insert->content = insertStr;
    strCpyStr(insert->content, token.content);
    insert->return_type = token.ret_value;
    insert->parameters = token.param_count;
    if(!tmp){
        rootPtr = insert;
        return rootPtr;
    }
    else {
        if ((strCmpStr(token.content, rootPtr->content)) < 0) {
            rootPtr->leftPtr = BVSInsert_function(rootPtr->leftPtr, token);
        } else if ((strCmpStr(token.content,rootPtr->content)) > 0) {
            rootPtr->rightPtr =  BVSInsert_function(rootPtr->rightPtr, token);
        }
    }
    return rootPtr;
}

TNodef *BVSSearch_function(TNodef *rootPtr, token token) {
    if (rootPtr == NULL) {
        return NULL;
    } else {
        if ((strCmpStr(token.content.str, rootPtr->content)) == 0) {
            return rootPtr;
        }
        else if ((strCmpStr(token.content.str, rootPtr->content)) < 0) {
            return BVSSearch_function(rootPtr->leftPtr, token);
        } else if ((strCmpStr(token.content.str, rootPtr->content)) > 0) {
            return BVSSearch_function(rootPtr->rightPtr, token);
        }
    }
    return NULL;
}

void BVSFreeFunctionNode(TNodef *rootPtr){
    if(rootPtr != NULL){
        BVSFreeFunctionNode(rootPtr->leftPtr);
        BVSFreeFunctionNode(rootPtr->rightPtr);
        free(rootPtr);
    }
}

void BVSFree_function(TRootf *SymTable){
    
    BVSFreeFunctionNode(SymTable->rootPtr);
    free(SymTable);
}
//////////////////////////////////////////
