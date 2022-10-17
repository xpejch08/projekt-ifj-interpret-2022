#include <stdio.h>
#include <stdbool.h>
#include "lexical.h"
#include "str.h"
#include "parser.h"
#include "stack.h"
#include "symtable.h"

//todo think about using enum and redefining token structure
//todo fucntion stat = actual function of token type
//todo function declrlist -> starts a type(checks if something is declared or not etc.) for all types that need it
//todo function statlist that ends functions (checks for right bracket, semicolon, etc.) for all types that need it

TNode *tree;
tStack *list;
int tokenId;
token *sToken;

int incId = 1;


//initializing tree
BVSInit(tree);

void generateInstruction(){
//todo define instructions in stack.h and write generateInstruction function
}

int stat(); // function declaration;

//check declare function, checks beginning of token type for example checks if there is a left bracket after while,
// recursively calls itself
int declrList(){

    switch (sToken->type) {
        case TYPE_IDENTIFIER:

            if(BVSSearch(tree, *sToken) == false){
                BVSInsert(tree, *sToken);
            }
            else{
                return SEM_ERROR;
            }
        case TYPE_KEYWORD:
            switch (sToken->content.keyword) {
                case KEYWORD_WHILE:

                    if(BVSSearch(tree, *sToken) == false){
                        BVSInsert(tree, *sToken);
                    }
                    else{
                        return SEM_ERROR;
                    }
                    if(tokenId == getNextToken(sToken) == LEX_ERROR){
                        return LEX_ERROR;
                    }
                    if(sToken->type != TYPE_LBRACKET){
                        return SYN_ERROR;
                    }
                    if(tokenId == getNextToken(sToken) == LEX_ERROR){
                        return declrList();
                    }

            }


    }
}
//check after function, checks ending of token type for example checks if there is a right bracket after condition of
// while, recursively calls itself
//also calls main fynction stat
int statList(){
    //todo
}


//stat function checks content of different types, for example condition of while or body of while, calls function
//statlist -> statlist calls stat recursively
int stat(){
    
 //todo
}

//function that calls function declr list and statlist depending on the type, also checks if there is end of file after
// or not
int program(){

    int result;
    switch (sToken->type) {
        case TYPE_IDENTIFIER:
            if((result = declrList()) == SYN_ERROR){
                return SYN_ERROR;
            }
            if((result = statList()) == SYN_ERROR){
                return SYN_ERROR;
            }

            if((sToken->type) != TYPE_END_OF_FILE){
                return SYN_ERROR;
            }

            generateInstruction();

            return SUCCES;


        case TYPE_NOT_EQUAL:
        case TYPE_EXPONENT_NUMBER:
        case TYPE_DOUBLE_NUMBER:
        case TYPE_INTEGER_NUMBER:
        case TYPE_STRING:
        case TYPE_VARIABLE:
        case TYPE_KEYWORD:
            switch (sToken->content.keyword) {
                case KEYWORD_WHILE:
                    if((result = declrList()) == SYN_ERROR){
                        return SYN_ERROR;
                    }
                    if((result = statList()) == SYN_ERROR){
                        return SYN_ERROR;
                    }

                    if((sToken->type) != TYPE_END_OF_FILE){
                        return SYN_ERROR;
                    }

                    generateInstruction();

                    return SUCCES;
                case KEYWORD_VOID:
                case KEYWORD_STRING:
                case KEYWORD_RETURN:
                case KEYWORD_NULL:
                case KEYWORD_INT:
                case KEYWORD_FLOAT:
                case KEYWORD_FUNCTION:
                case KEYWORD_IF:
                case KEYWORD_ELSE:
            }
        case TYPE_INITIAL:
        case TYPE_CONCATENATE:
        case TYPE_EQUAL:
        case TYPE_GREATER_OR_EQUAL:
        case TYPE_GREATER_THAN:
        case TYPE_SMALLER_OR_EQUAL:
        case TYPE_SMALLER_THAN:
        case TYPE_SUBTRACTION:
        case TYPE_DIVIDE:
        case TYPE_MULTIPLY:
        case TYPE_ADDITION:
        case TYPE_ASSIGN:
        case TYPE_END_OF_FILE:
    }
}

//function that initializes tree, list of instructions, token, reads first token and calls program() function
int parse(TNode *BVS, tStack *iList){

    int result;
    tree = BVS;
    list = iList;
    //todo fix init token function
    initToken(sToken);
    if((tokenId = getNextToken(sToken)) == LEX_ERROR){
        return LEX_ERROR;
    }
    else{
        result = program();
    }
    //todo freeToken() funtcion
    return result;
}




