#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "parser.h"
#include "lexical.h"
#include "stack.h"
#include "errors.h"
#include "str.h"
#include <stdbool.h>

/////////VARIABLE//////////
typedef struct tnode{
    string *content;
    struct tnode *leftPtr;
    struct tnode *rightPtr;
    int type;
    bool declared;
}TNode;

typedef struct troot{
    TNode *rootPtr;
}TRoot;

///////////////////////////
/////////FUNCTION//////////
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
 * @brief function creates new node for variable in SymTable
 * @param token token from which we put data
 * @param rootPtr pointer on node
 * @return returns newly created node
 */
TNode *BVSCreate(TNode *rootPtr, token token);
/**
 * @brief function inserts variables to binary tree based on its ASCII value
 * @param rootPtr pointer on node
 * @param token token from which we put data
 * @return return pointer on newly created ndoe
 */
TNode *BVSInsert(TNode *root, token token);
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
 * @brief function free all nodes with variables in SymTable
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
 * @param rootPtr pointer on node
 * @return returns newly created node
 */
TNodef *BVSCreate_function(TNodef *rootPtr,function_save token);
/**
 * @brief function inserts function to binary tree based on its ASCII value
 * @param rootPtr pointer on node
 * @param token token from which we put data
 * @return returns newly created node or pointer that already exists
 */
TNodef *BVSInsert_function(TNodef *rootPtr, function_save token);
/**
 * @brief function search if we have already declared function
 * @param rootPtr pointer on node
 * @param token token from which we compare with data in node
 * @return returns pointer to node if was already declared
 */
TNodef *BVSSearch_function(TNodef *rootPtr, token token);
/**
 * @brief function free all nodes in SymTable with functions
 * @param SymTable pointer on SymTable
 */
void BVSFree_function(TRootf *SymTable);
/**
 * @brief function dispose every singe node with variable in function
 * @param rootPtr pointer on every variable
 */
void BVSDisposeNode(TNode *rootPtr);
/**
 * @brief function free all nodes 
 * @param rootPtr pointer on node with variable
 */
void BVSFreeNode(TNode *rootPtr);
/**
 * @brief function free all nodes in SymTable
 * @param rootPtr pointer on node with function name
 */
void BVSFreeFunctionNode(TNodef *rootPtr);
/**
 * @brief help function to print tree
 * @param SymTable pointer on SymTable
 */
void postorder(TNode *tree);

#endif
