#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include "symtable.h"
#include "parser.h"


////////////VARIABLE///////////////////////////

void BVSInit(TRoot *SymTable){
    SymTable->rootPtr = NULL;
}

void BVSCreate(TNode *rootPtr, token token){
    TNode *newPtr = malloc(sizeof(struct tnode));
    if(newPtr == NULL){
        fprintf(stderr, "99");
        return;
    }
    newPtr->leftPtr = NULL;
    newPtr->rightPtr = NULL;
    newPtr->type = token.type;
    newPtr->name = token.content.str;
    newPtr->name->str = token.content.str->str;
    rootPtr = newPtr;
}

void BVSInsert(TRoot *root, token token){
    if(root->rootPtr == NULL){
        TNode *newPtr = (TNode *) malloc(sizeof(struct tnode));
        if(newPtr == NULL){
            fprintf(stderr, "99");
            return;
        }
        newPtr->leftPtr = NULL;
        newPtr->rightPtr = NULL;
        newPtr->type = token.type;
        newPtr->content = token.content;
        root->rootPtr = newPtr;
    }
    else{
        if((strCmpStr(token.content.str, root->rootPtr->content.str)) < 0){
            BVSInsert(root, token);
        }
        else if((strCmpStr(token.content.str, root->rootPtr->content.str)) > 0){
            BVSInsert(root, token);
        }
    }
}


TNode *BVSSearch(TNode *rootPtr, token token){
    if(rootPtr == NULL){
        return NULL;
    }
    else{
        if((strCmpStr(token.content.str, rootPtr->name)) < 0){
            rootPtr->leftPtr = BVSSearch(rootPtr->leftPtr, token);
        }
        else if((strCmpStr(token.content.str, rootPtr->name)) < 0){
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

void BVSCreate_function(function_save token){
    TNodef *newPtr = malloc(sizeof(struct tnodef));
    if(newPtr == NULL){
        fprintf(stderr, "99");
        return;
    }
    newPtr->leftPtr = NULL;
    newPtr->rightPtr = NULL;
    newPtr->content = token.content;
    newPtr->content->str = token.content->str;
    newPtr->parameters = token.param_count;
    newPtr->return_type = token.ret_value;
}

void BVSInsert_function(TNodef *rootPtr, function_save token){
    if(rootPtr == NULL){
        BVSCreate_function(token);
    }
    else{
        if((strCmpStr(token.content, rootPtr->content)) < 0){
            BVSInsert_function(rootPtr->leftPtr, token);
        }
        else if((strCmpStr(token.content, rootPtr->content)) > 0){
            BVSInsert_function(rootPtr->rightPtr, token);
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
void postorder(TRoot *tree) {
  post(tree->rootPtr);
}

void post(TNode *tree){
    if (tree != NULL){
        post(tree->leftPtr);
        post(tree->rightPtr);
        fprintf(stderr,"CONTENT: %s      TYPE: %d\n", tree->name->str, tree->type);
    }else{
        fprintf(stderr, "SADDDDDDDDDDDDD");
    }
}