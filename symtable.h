#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "lexical.h"
#include "stack.h"
#include "errors.h"
#include "str.h"

typedef struct tnode{
    tokenContent content;
    struct tnode *leftPtr;
    struct tnode *rightPtr;
    int parameters;
    int return_type;
    int type;
}TNode;

typedef struct troot{
    TNode *rootPtr;
}TRoot;

/**
 * @brief function function initializes new symtable
 * @param SymTable pointer to main root
 */
//void BVSInit(TRoot *SymTable); // TODO declere mistake wtf??
/**
 * @brief function creates new node in SymTable
 * @param token token from which we put data
 * @return returns newly created node
 */
void BVSCreate(token token, int param, int return_value);
/**
 * @brief function inserts node to binary tree based on its ASCII value
 * @param rootPtr pointer on node
 * @param token token from which we put data
 * @return returns 0 if successful
 */
void BVSInsert(TNode *rootPtr, token token, int param, int return_value);
/**
 * @brief function search if we have already declared 
 * @param rootPtr pointer on node
 * @param token token from which we compare with data in node
 * @return returns true if was already declared
 */
TNode *BVSSearch(TNode *rootPtr, token token);
/**
 * @brief function free all nodes in SymTable
 * @param rootPtr pointer on node
 * @param SymTable pointer on SymTable
 */
void BVSFree(TRoot *SymTable, TNode *rootPtr);

#endif