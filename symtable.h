#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "parser.h"
#include "lexical.h"
#include "stack.h"
#include "errors.h"
#include "str.h"
#include "symtable.h"
/////////VARIABLE//////////
typedef struct tnode{
    tokenContent content;
    struct tnode *leftPtr;
    struct tnode *rightPtr;
    int type;
}TNode;
///////////////////////////
/////////FUNCTION//////////
typedef struct troot{
    TNode *rootPtr;
}TRoot;

typedef struct tnodef{
    string *content;
    struct tnodef *leftPtr;
    struct tnodef *rightPtr;
    int parameters;
    int return_type;
}TNodef;

typedef struct trootf{
    TNodef *rootPtr;
}TRootf;

typedef struct{
    string *content;
    int param_count;
    int ret_value;
}function_save;
//////////////////////////////
/**
 * @brief function function initializes new symtable for variables
 * @param SymTable pointer to main root
 */
void BVSInit(TRoot *SymTable); // TODO declere mistake wtf??
/**
 * @brief function creates new node in SymTable
 * @param token token from which we put data
 * @return returns newly created node
 */
void BVSCreate(token token);
/**
 * @brief function inserts variables to binary tree based on its ASCII value
 * @param rootPtr pointer on node
 * @param token token from which we put data
 */
void BVSInsert(TNode *rootPtr, token token);
/**
 * @brief function search if we have already declared variable
 * @param rootPtr pointer on node
 * @param token token from which we compare with data in node
 * @return returns pointer to found variable or NULL
 */
TNode *BVSSearch(TNode *rootPtr, token token);
/**
 * @brief function dispose whole symtable
 * @param rootPtr pointer on node
 * @param SymTable pointer on SymTable
 */
void BVSDispose(Troot *Symtable, TNode *rootPtr);
/**
 * @brief function free all nodes in SymTable
 * @param rootPtr pointer on node
 * @param SymTable pointer on SymTable
 */
void BVSFree(TRoot *SymTable, TNode *rootPtr);
/**
 * @brief function function initializes new symtable for functions
 * @param SymTable pointer to main root
 */
void BVSInit_function(TRootf *SymTable); // 
/**
 * @brief function creates new function in SymTable
 * @param token token from which we put data
 */
void BVSCreate_function(function_save token);
/**
 * @brief function inserts function to binary tree based on its ASCII value
 * @param rootPtr pointer on node
 * @param token token from which we put data
 */
void BVSInsert_function(TNodef *rootPtr, function_save token);
/**
 * @brief function search if we have already declared function
 * @param rootPtr pointer on node
 * @param token token from which we compare with data in node
 * @return returns pointer to node if was already declared
 */
TNodef *BVSSearch_function(TNodef *rootPtr, token token);
/**
 * @brief function free all nodes in SymTable
 * @param rootPtr pointer on node
 * @param SymTable pointer on SymTable
 */
void BVSFree_function(TRootf *SymTable, TNodef *rootPtr);


#endif