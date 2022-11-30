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

TNode *BVSCreate(TNode *rootPtr, token token){
    TNode *newPtr = malloc(sizeof(struct tnode));
    if(newPtr == NULL){
        fprintf(stderr, "99");
        return NULL;
    }
    //TNode init;
    string initString;
    initString.str = NULL;
    initString.length = 0;
    initString.alloc = 0;
    newPtr->content = &initString;
    strCpyStr(newPtr->content, (&token)->content.str);
    newPtr->type = (&token)->type;
    newPtr->leftPtr = NULL;  
    newPtr->rightPtr = NULL;
    rootPtr = newPtr;
    return newPtr;
}

TNode *BVSInsert(TNode *rootPtr, token token){
    if(rootPtr == NULL){
        return BVSCreate(rootPtr, token);
    }
    else {
        if ((strCmpStr(token.content.str, rootPtr->content)) < 0) {
            return BVSInsert(rootPtr->leftPtr, token);
        } else if ((strCmpStr(token.content.str,rootPtr->content)) > 0) {
            return BVSInsert(rootPtr->rightPtr, token);
        }
    }
    return rootPtr;
}


TNode *BVSSearch(TNode *rootPtr, token token){
    if(rootPtr == NULL){
        return NULL;
    }
    else{
        if((strCmpStr(token.content.str, rootPtr->content)) < 0){
            rootPtr->leftPtr = BVSSearch(rootPtr->leftPtr, token);
        }
        else if((strCmpStr(token.content.str, rootPtr->content)) < 0){
            rootPtr->rightPtr = BVSSearch(rootPtr->rightPtr, token);
        }
        return rootPtr;
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

void BVSCreate_function(TNodef *rootPtr,function_save token){
    TNodef *newPtr = malloc(sizeof(struct tnodef));
    if(newPtr == NULL){
        fprintf(stderr, "99");
        return;
    }
    //TNodef init;
    string initStr;
    initStr.str = NULL;
    initStr.length = 0;
    initStr.alloc = 0;
    //newPtr = &init;
    newPtr->content = &initStr;
    newPtr->leftPtr = NULL;
    newPtr->rightPtr = NULL;
    newPtr->return_type = token.ret_value;
    newPtr->parameters = token.param_count;
    strCpyStr(newPtr->content, token.content);
    rootPtr = newPtr;
}

void BVSInsert_function(TNodef *rootPtr, function_save token){
    if(rootPtr == NULL){
        BVSCreate_function(rootPtr ,token);
    }
    else{
        if((strCmpStr(token.content, rootPtr->content)) < 0){
            BVSCreate_function(rootPtr->leftPtr, token);
        }
        else if((strCmpStr(token.content, rootPtr->content)) > 0){
            BVSCreate_function(rootPtr->rightPtr, token);
        }
    }
}

TNodef *BVSSearch_function(TNodef *rootPtr, token token){
    if(rootPtr == NULL){
        return NULL;
    }
    else{
        if((strCmpStr(token.content.str, rootPtr->content)) < 0){
            rootPtr->leftPtr = BVSSearch_function(rootPtr->leftPtr, token);
        }
        else if((strCmpStr(token.content.str, rootPtr->content)) < 0){
            rootPtr->rightPtr = BVSSearch_function(rootPtr->rightPtr, token);
        }
        return rootPtr;
    }
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
void postorder(TNode *tree){ 
    if (tree != NULL){
        fprintf(stderr,"CONTENT: %s      TYPE: %d\n", tree->content->str, tree->type);
    }
    if(tree == NULL){
        fprintf(stderr, "IS EMPTY\n");
    }
}

//TNode *newPtr = &init;
//newPtr->leftPtr = NULL;
//newPtr->rightPtr = NULL;
//newPtr->type = token.type;
//strCpyStr(newPtr->content, token.content.str);
//rootPtr = newPtr;
//fprintf(stderr, "%s\n", newPtr->content->str);
//fprintf(stderr, "%d\n", newPtr->type);
//newPtr->content = (&token)->content.str;