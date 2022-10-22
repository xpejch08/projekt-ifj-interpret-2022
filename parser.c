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
DLLElementPtr *list;
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
                    getNextToken(sToken);
                    if(sToken->type != TYPE_LBRACKET){
                        return SYN_ERROR;
                    }
                    else{
                        return SUCCES;
                    }
                case KEYWORD_VOID:
                    getNextToken(sToken);
                    if(sToken->type != TYPE_LBRACKET){
                        return SYN_ERROR;
                    }
                    else{
                        return SUCCES;
                    }
                //case KEYWORD_STRING:
                //    getNextToken(sToken);
                //    if(sToken->type != TYPE_VARIABLE){
                //        return SYN_ERROR;
                //    }else{
                //        return SUCCES;
                //    }
                //case KEYWORD_RETURN:
                //    getNextToken(sToken);
                //    // TODO check if anything is missing
                //    if(sToken->type != TYPE_VARIABLE || sToken->type != TYPE_STRING || sToken->type != TYPE_INTEGER_NUMBER || sToken){
                //        return SYN_ERROR;
                //    }else{
                //        return SUCCES;
                //    }
                case KEYWORD_INT:
                    getNextToken(sToken);
                    if(sToken->type != TYPE_VARIABLE){
                        return SYN_ERROR;
                    }else{
                        return SUCCES;
                    }
                case KEYWORD_IF:
                    getNextToken(sToken);
                    if(sToken->type != TYPE_LBRACKET){
                        return SYN_ERROR;
                    }else{
                        return SUCCES;
                    }
                case KEYWORD_FUNCTION:
                    getNextToken(sToken);
                    if(sToken->type != TYPE_VARIABLE){
                        return SYN_ERROR;
                    }else{
                        return SUCCES;
                    }
                case KEYWORD_FLOAT:
                    getNextToken(sToken);
                    if(sToken->type != TYPE_VARIABLE){
                        return SYN_ERROR;
                    }else{
                        return SUCCES;
                    }
                case KEYWORD_ELSE:
                    getNextToken(sToken);
                    if(sToken->type != TYPE_LVINCULUM){
                        return SYN_ERROR;
                    }else{
                        return SUCCES;
                    }
            }


    }
}
//check after function, checks ending of token type for example checks if there is a right bracket after condition of
// while, recursively calls itself
//also calls main fynction stat
int statList(){
    int result;
    if((result = getNextToken(sToken)) == LEX_ERROR){
        return LEX_ERROR;
    }
    switch (sToken->type) {
        case TYPE_RVINCULUM:
            return SUCCES;
            break;
        case TYPE_VARIABLE:
        case TYPE_END_OF_FILE:
        case TYPE_IDENTIFIER:
        case TYPE_SEMICOLON:
            return SUCCES;
        case TYPE_RBRACKET:
            return SUCCES;
        case TYPE_ASSIGN:
        case TYPE_ADDITION:
        case TYPE_MULTIPLY:
        case TYPE_DIVIDE:
        case TYPE_SUBTRACTION:
        case TYPE_SMALLER_THAN:
        case TYPE_SMALLER_OR_EQUAL:
        case TYPE_GREATER_THAN:
        case TYPE_GREATER_OR_EQUAL:
        case TYPE_EQUAL:
        case TYPE_CONCATENATE:
        case TYPE_INITIAL:
        case TYPE_STRING:
        case TYPE_INTEGER_NUMBER:
        case TYPE_DOUBLE_NUMBER:
        case TYPE_EXPONENT_NUMBER:
        case TYPE_NOT_EQUAL:
    }
    //todo statList
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
            if((result = statList()) != SUCCES){
                return result;
            }

            if((sToken->type) != TYPE_END_OF_FILE){
                return SYN_ERROR;
            }

            generateInstruction();

            return SUCCES;


        case TYPE_NOT_EQUAL:
            return SYN_ERROR;
        case TYPE_EXPONENT_NUMBER:
        case TYPE_DOUBLE_NUMBER:
        case TYPE_INTEGER_NUMBER:
        case TYPE_STRING:
        case TYPE_VARIABLE:
        case TYPE_KEYWORD:
            switch (sToken->content.keyword) {
                case KEYWORD_WHILE:
                    if((result = declrList()) != SUCCES){
                        return result;
                    }
                    if((result = statList()) != SUCCES){
                        return result;
                    }

                    if((sToken->type) != TYPE_END_OF_FILE){
                        return SYN_ERROR;
                    }

                    generateInstruction();

                    return SUCCES;

                case KEYWORD_VOID:
                    if((result = declrList()) != SUCCES){
                        return result;
                    }
                    if((result = statList()) != SUCCES){
                        return result;
                    }

                    if((sToken->type) != TYPE_END_OF_FILE){
                        return SYN_ERROR;
                    }

                    generateInstruction();

                    return SUCCES;

                case KEYWORD_STRING: // todo budeme delat string a return v declrlist??
                case KEYWORD_RETURN:
                case KEYWORD_NULL:
                case KEYWORD_INT:
                case KEYWORD_FLOAT:
                case KEYWORD_FUNCTION:
                    if((result = declrList()) != SUCCES){
                        return result;
                    }
                    if((result = statList()) != SUCCES){
                        return result;
                    }

                    if((sToken->type) != TYPE_END_OF_FILE){
                        return SYN_ERROR;
                    }

                    generateInstruction();

                    return SUCCES;
                case KEYWORD_IF:
                    if((result = declrList()) != SUCCES){
                        return result;
                    }
                    if((result = statList()) != SUCCES){
                        return result;
                    }

                    if((sToken->type) != TYPE_END_OF_FILE){
                        return SYN_ERROR;
                    }

                    generateInstruction();

                    return SUCCES;
                case KEYWORD_ELSE:
                    if((result = declrList()) != SUCCES){
                        return result;
                    }
                    if((result = statList()) != SUCCES){
                        return result;
                    }

                    if((sToken->type) != TYPE_END_OF_FILE){
                        return SYN_ERROR;
                    }

                    generateInstruction();

                    return SUCCES;
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
            return SYN_ERROR;    
        case TYPE_END_OF_FILE:
        case TYPE_LBRACKET:
        case TYPE_RBRACKET:
            return SYN_ERROR;
        case TYPE_SEMICOLON:
        case TYPE_RVINCULUM:
        case TYPE_LVINCULUM:
    }
}

//function that initializes tree, list of instructions, token, reads first token and calls program() function
int parse(TNode *BVS,DLLElementPtr *iList){

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




