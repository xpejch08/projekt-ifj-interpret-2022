#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "parser.h"
#include "lexical.h"
#include "stack.h"
#include "errors.h"
#include "str.h"

/////////VARIABLE//////////
typedef struct tnode{
    string *content;
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
void BVSCreate(TNode *rootPtr, token token);
/**
 * @brief function inserts variables to binary tree based on its ASCII value
 * @param rootPtr pointer on node
 * @param token token from which we put data
 */
void BVSInsert(TNode *root, token token);
/**
 * @brief function search if we have already declared variable
 * @param rootPtr pointer on node
 * @param token token from which we compare with data in node
 * @return returns pointer to found variable or NULL
 */
TNode *BVSSearch(TNode *rootPtr, token token);
/**
 * @brief function dispose whole symtable
  * @param SymTable pointer on SymTable
 */
void BVSDispose(TRoot *Symtable);
/**
 * @brief function free all nodes in SymTable
 * @param SymTable pointer on SymTable
 */
void BVSFree(TRoot *SymTable);
/**
 * @brief function function initializes new symtable for functions
 * @param SymTable pointer to main root
 */
void BVSInit_function(TRootf *SymTable); 
/**
 * @brief function creates new function in SymTable
 * @param token token from which we put data
 */
void BVSCreate_function(TNodef *rootPtr,function_save token);
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
 * @param SymTable pointer on SymTable
 */
void BVSFree_function(TRootf *SymTable);

void BVSDisposeNode(TNode *rootPtr);

void BVSFreeNode(TNode *rootPtr);

void BVSFreeFunctionNode(TNodef *rootPtr);

void postorder(TNode *tree);

#endif
