//////////////////////////////////////////////////////////////////
//    @@ Implementace překladače imperativního jazyka IFJ22 @@  //
//                                                              //
//       author: xpejch08                                       //
//                                                              //
//                                                              //
//                                                              //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "parser.h"
#include "lexical.h"
#include "stack.h"
#include "errors.h"
#include "str.h"
#include <stdbool.h>

/////////VARIABLE//////////
/**
 * TNode is a structure containing the important information about variable
 */
typedef struct tnode{
    string *content; // variable content
    struct tnode *leftPtr; // pointer for its left child
    struct tnode *rightPtr; // pointer for its right child
    int type; // type of variable
    bool declared; // bool if variable was declared
}TNode;

/**
 * TRoot is symtable for variables
 */
typedef struct troot{
    TNode *rootPtr; // pointer on specific tree
}TRoot;

///////////////////////////
/////////FUNCTION//////////
/**
 * TNodef is a structure containing the important information about function
 */
typedef struct tnodef{
    string *content; // function name
    struct tnodef *leftPtr; // pointer on its left child in BST
    struct tnodef *rightPtr; // pointer on its right pointer in BST
    int parameters; // number of parameters
    int return_type; // what type function should return
}TNodef;
/**
 * TRootf is symtable for functions
 */
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
 * @brief function function initializes new symtable for functions
 * @param SymTable pointer to main root
 */
void BVSInit_function(TRootf *SymTable);
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
 * @brief function dispose every singe node with variable in function
 * @param rootPtr pointer on every variable
 */
void BVSDisposeNode(TNode *rootPtr);



#endif
