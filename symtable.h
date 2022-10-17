#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "lexical.h"
#include "stack.h"
#include "errors.h"
#include "str.h"

typedef struct tnode{
    tokenType type;
    tokenContent content;
    struct tnode *leftPtr;
    struct tnode *rightPtr;
    string key;
}TNode;

typedef struct troot{
    TNode *rootPtr;
}TRoot;

void BVSInit(TRoot *SymTable);
TNode *BVSCreate(token token);
TNode *BVSInsert(TNode *rootPtr, token token);
bool BVSSearch(TNode *rootPtr, token token);
void BVSFree(TRoot *SymTable, TNode *rootPtr);

#endif