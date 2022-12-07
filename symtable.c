//////////////////////////////////////////////////////////////////
//    @@ Implementace překladače imperativního jazyka IFJ22 @@  //
//                                                              //
//       author: xpejch08 (Štěpán Pejchar)                      //
//       author xsocha03 (Marcin Sochacki)                      //
//                                                              //
//                                                              //
//                                                              //
//////////////////////////////////////////////////////////////////

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
    // allocating node and string struct
    TNode *insert = malloc((sizeof (TNode)));
    string *insertStr = malloc(sizeof (string));
    insert->leftPtr = NULL;
    insert->rightPtr = NULL;
    // setting variable name
    insert->content = insertStr;
    strCpyStr(insert->content, token.content.str);
    // variable is not defined yet
    insert->type = KEYWORD_NULL;
    // if rootptr didnt exist before
    if(!tmp){
        rootPtr = insert;
        return rootPtr;
    }
    else {
        // inserting node on right place in BST
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
    // variable not found
    if(rootPtr == NULL){
        return NULL;
    }
    // search based on content ASCII value
    else{
        // variable found   
        if((strCmpStr(token.content.str, rootPtr->content)) == 0){
            return rootPtr;
        }
        // ASCII value is smaller we go to left child
        else if((strCmpStr(token.content.str, rootPtr->content)) < 0){
            return BVSSearch(rootPtr->leftPtr, token);
        }
        // ASCII value is bigger so we go to right child
        else if((strCmpStr(token.content.str, rootPtr->content)) > 0){
            return BVSSearch(rootPtr->rightPtr, token);
        }
        return NULL;
    }
}

void BVSDisposeNode(TNode *rootPtr){
    // disposing recursively evry node
    if(rootPtr != NULL){
        BVSDisposeNode(rootPtr->leftPtr);
        BVSDisposeNode(rootPtr->rightPtr);
        rootPtr = NULL;
    }
}

void BVSDispose(TRoot *SymTable){
    BVSDisposeNode(SymTable->rootPtr);
    SymTable = NULL;
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
//////////////////////////////////////////
