#include "lexical.h"
#include "stack.h"

typedef struct tnode{
    tokenType type;
    struct tnode *leftPtr;
    struct tnode *rightPtr;
}TNode;

typedef struct troot{
    TNode *rootPtr;
}TRoot;

void BVSInit(TRoot *SymTable);
TNode *BVSInsert(TNode *rootPtr, token token);
TNode *BVSSearch(TNode *rootPtr, token token);
void BVSFree(TRoot *SymTable, TNode *rootPtr);