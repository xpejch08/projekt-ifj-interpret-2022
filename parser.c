#include <stdio.h>
#include <stdbool.h>
#include "lexical.h"
#include "str.h"
#include "parser.h"
#include "stack.h"
#include "symtable.h"
#include "code_gen.h"

//todo think about using enum and redefining token structure !!!
//todo function declrlist -> starts a type(checks if something is declared or not etc.) for all types that need it
//todo function statlist that ends functions (checks for right bracket, semicolon, etc.) for all types that need it
//todo generateinstruction(idinstruukce, pointrprev, pointractive, pouinternanext) -> function that inserts active instruction into instruction list
//todo TInst setactiveinstruction(const int *type, void *op1, void *op2, void *op3)

TNode *insideFunction;
TNode *functionNames;
TNode *mainTree;
DLLElementPtr *list;

int paramError;
int tokenId;
token *sToken;

int incId = 1;


//initializing tree
BVSInit(mainTree);
BVSInit(functionNames);
BVSInit(insideFunction);

int stat(); // function declaration;
int statlist();
int parametrs(); // declaration because function is used before definition

//check declare function, checks beginning of token type for example checks if there is a left bracket after while,
// recursively calls itself
int declrList() {

    int result;

    switch (sToken->type) {
        case TYPE_RVINCULUM:
            return SUCCES;
        case TYPE_IDENTIFIER:

            if (strCmpConstStr(sToken->content->str, "write")) {
                if(getNextToken(sToken) == LEX_ERROR){
                    return  LEX_ERROR;
                }
                if (sToken->type == TYPE_LBRACKET) {
                    paramError = parametrs(PARAM_WRITE, 1);
                    if (paramError == SUCCES) {
                        if(getNextToken(sToken) == LEX_ERROR){
                            return  LEX_ERROR;
                        }
                        if (sToken->type != TYPE_SEMICOLON) {
                            //instructionFree()
                            return SYN_ERROR;
                        }
                        generateInstruction(activeInstruction, adr1, NULL, NULL);
                        instructionFree(activeInstruction);
                        //todo instructionFree()
                        getNextToken(sToken);
                        result = statlist();
                        if(result != SUCCES) {
                            return result;
                        }
                        return SUCCES;
                    } else {
                        return paramError;
                    }
                } else {
                    return SYN_ERROR;
                }
            }
            if (strCmpConstStr(sToken->content->str, "reads")) {
                getNextToken(sToken);
                if (sToken->type == TYPE_LBRACKET) {
                    paramError = parametrs(PARAM_READS, 1); 
                    if (paramError == SUCCES) {
                        if (getNextToken(sToken) != TYPE_SEMICOLON) {
                            //instructionFree()
                            return SYN_ERROR;
                        }
                        generateInstruction(activeInstruction, adr1, NULL, NULL);
                        //todo instructionFree()
                        getNextToken(sToken);
                        return SUCCES;
                    } else {
                        return paramError;
                    }
                } else {
                    return SYN_ERROR;
                }
            }
            if (strCmpConstStr(sToken->content->str, "readi")) {
                if(getNextToken(sToken) == LEX_ERROR){
                    return LEX_ERROR;
                }
                if (sToken->type == TYPE_LBRACKET) {
                    paramError = parametrs(PARAM_READI, 1);
                    if (paramError == SUCCES) {
                        if(getNextToken(sToken) == LEX_ERROR){
                            return LEX_ERROR;
                        }
                        if (sToken->type != TYPE_SEMICOLON) {
                            //instructionFree()
                            return SYN_ERROR;
                        }
                        activeInstruction = setActiveInstruction();
                        generateInstruction(activeInstruction, adr1, NULL, NULL);
                        getNextToken(sToken);
                        result = statlist();
                        if(result != SUCCES){
                            return result;
                        }
                        return SUCCES;
                    } else {
                        return paramError;
                    }
                } else {
                    return SYN_ERROR;
                }
            }
            if (strCmpConstStr(sToken->content->str, "readf")) {
                getNextToken(sToken);
                if (sToken->type == TYPE_LBRACKET) {
                    paramError = parametrs(P}tokenType;ARAM_READF, 1);
                    if (paramError == SUCCES) {
                        if (getNextToken(sToken) != TYPE_SEMICOLON) {
                            //instructionFree()
                            return SYN_ERROR;
                        }
                        generateInstruction(activeInstruction, adr1, NULL, NULL);
                        //todo instructionFree()
                        getNextToken(sToken);
                        return SUCCES;
                    } else {
                        return paramError;
                    }
                } else {
                    return SYN_ERROR;
                }
            }
            if (strCmpConstStr(sToken->content->str, "strlen")) {
                getNextToken(sToken);
                if (sToken->type == TYPE_LBRACKET) {
                    paramError == parametrs(PARAM_STRLEN, 1);
                    if (paramError == SUCCES) {
                        if (getNextToken(sToken) != TYPE_SEMICOLON) {
                            //instructionFree()
                            return SYN_ERROR;
                        }
                        generateInstruction(activeInstruction, adr1, NULL, NULL);
                        //todo instructionFree()
                        getNextToken(sToken);
                        return SUCCES;
                    } else {
                        return paramError;
                    }
                } else {
                    return SYN_ERROR;
                }
            }
            if (strCmpConstStr(sToken->content->str, "substring")) {
                getNextToken(sToken);
                if (sToken->type == TYPE_LBRACKET) {
                    paramError = parametrs(PARAM_SUBSTRING, 1); 
                    if (paramError == SUCCES) {
                        if (getNextToken(sToken) != TYPE_SEMICOLON) {
                            //instructionFree()
                            return SYN_ERROR;
                        }
                        generateInstruction(activeInstruction, adr1, NULL, NULL);
                        //todo instructionFree()
                        getNextToken(sToken);
                        return SUCCES;
                    } else {
                        return paramError;
                    }
                } else {
                    return SYN_ERROR;
                }
            }
            if (strCmpConstStr(sToken->content->str, "ord")) {
                getNextToken(sToken);
                if (sToken->type == TYPE_LBRACKET) {
                    paramError = parametrs(PARAM_SUBSTRING, 1); 
                    if (paramError == SUCCES) {
                        if (getNextToken(sToken) != TYPE_SEMICOLON) {
                            //instructionFree()
                            return SYN_ERROR;
                        }
                        generateInstruction(activeInstruction, adr1, NULL, NULL);
                        //todo instructionFree()
                        getNextToken(sToken);
                        return SUCCES;
                    } else {
                        return paramError;
                    }
                } else {
                    return SYN_ERROR;
                }
            }
            if (strCmpConstStr(sToken->content->str, "chr")) {
                getNextToken(sToken);
                if (sToken->type == TYPE_LBRACKET) {
                    paramError = parametrs(PARAM_CHR, 1); 
                    if (paramError == SUCCES) {
                        if (getNextToken(sToken) != TYPE_SEMICOLON) {
                            //instructionFree()
                            return SYN_ERROR;
                        }
                        generateInstruction(activeInstruction, adr1, NULL, NULL);
                        //todo instructionFree()
                        getNextToken(sToken);
                        return SUCCES;
                    } else {
                        return paramError;
                    }
                } else {
                    return SYN_ERROR;
                }
            }

            if (BVSSearch(mainTree, *sToken) == false) {
                BVSInsert(mainTree, *sToken);
            } else {
                return SEM_ERROR;
            }

        case KEYWORD_WHILE:
            getNextToken(sToken);
            if (sToken->type != TYPE_LBRACKET) {
                return SYN_ERROR;
            } else {
                return parametrs(PARAM_IF_WHILE, 1);
            }
        case KEYWORD_VOID:
            getNextToken(sToken);
            if (sToken->type != TYPE_LBRACKET) {
                return SYN_ERROR;
            } else {
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
            if (sToken->type != TYPE_VARIABLE) {
                return SYN_ERROR;
            } else {
                return SUCCES;
            }
        case KEYWORD_IF:
            if(getNextToken(sToken) == LEX_ERROR) {
                return LEX_ERROR;
            }
            if (sToken->type != TYPE_LBRACKET) {
                return SYN_ERROR;
            } else {
                paramError = parametrs(PARAM_IF_WHILE,1);
                if(paramError != SUCCES) {
                     return paramError;
                }
                else {

                    if(getNextToken(sToken) == LEX_ERROR) {
                        return LEX_ERROR;
                    }
                    if(sToken->type != TYPE_LVINCULUM) {
                        return SYN_ERROR;
                    }
                    if(getNextToken(sToken) == LEX_ERROR) {
                        return LEX_ERROR;
                    }
                    if(sToken->type == TYPE_RVINCULUM) {
                        result = statlist();
                        if (result != SUCCES) {
                            return result;
                        }
                        return SUCCES;
                    }
                    else {
                        result = statlist();
                        if (result != SUCCES) {
                            return result;
                        }
                        if(sToken->type != TYPE_RVINCULUM) {
                            return SYN_ERROR;
                        }
                        if(getNextToken(sToken) == LEX_ERROR) {
                            return LEX_ERROR;
                        }
                        result = statlist();
                        if (result != SUCCES) {
                            return result;
                        }
                        return SUCCES;
                    }
                }
            }
        case KEYWORD_ELSE:
            if(getNextToken(sToken) == LEX_ERROR) {
                return LEX_ERROR;
            }
            if(sToken->type != TYPE_LVINCULUM) {
                return SYN_ERROR;
            }
            if(getNextToken(sToken) == LEX_ERROR){
                    return LEX_ERROR;
                }
            if(sToken->type == TYPE_RVINCULUM) {
                if(getNextToken(sToken) == LEX_ERROR) {
                    return LEX_ERROR;
                }
                result = statlist();
                if (result != SUCCES) {
                    return result;
                }
                return SUCCES;
            }else {
                result = statlist();
                if (result != SUCCES) {
                    return result;
                }
                if (sToken->type != TYPE_RVINCULUM) {
                    return SYN_ERROR;
                }
                result = statlist();
                if (result != SUCCES) {
                    return result;
                }
                return SUCCES;
            }
        case KEYWORD_FUNCTION:
            getNextToken(sToken);
            if (sToken->type != TYPE_IDENTIFIER) {
                return SYN_ERROR;
            }
            getNextToken(sToken);
            if (sToken->type != TYPE_LBRACKET) {
                return SYN_ERROR;
            }
            return parametrs(PARAM_FUNCTION, 1);
        case KEYWORD_FLOAT:
            getNextToken(sToken);
            if (sToken->type != TYPE_VARIABLE) {
                return SYN_ERROR;
            } else {
                return SUCCES;
            }
        }
    }
//check after function, checks ending of token type for example checks if there is a right bracket after condition of
// while, recursively calls itself
//also calls main fynction stat
int statList(){
    int result;
    result = getNextToken(sToken);
    if(result == LEX_ERROR){
        return LEX_ERROR;
    }
    switch (sToken->type) {

        case TYPE_COLON:

            getNextToken(sToken);
            if(sToken->type != KEYWORD_VOID    ||
               sToken->type != KEYWORD_INT     ||
               sToken->type != KEYWORD_STRING  ||
               sToken->type != KEYWORD_FLOAT)
                return SYN_ERROR;
            getNextToken(sToken);
            if(sToken->type != TYPE_LVINCULUM)
                return SYN_ERROR;
            getNextToken(sToken);
            return declrList();


                
        case TYPE_RVINCULUM:
            return SUCCES;

        case TYPE_VARIABLE:
            BVSInsert(mainTree, *sToken);
            activeInstruction = setActiveInstruction();
            getNextToken(sToken);
            result = statList();
            if(result != SUCCES){
                return result;
            }
            return SUCCES;

        case TYPE_END_OF_FILE:
        case TYPE_IDENTIFIER:
            result = declrList();
            if(result != SUCCES){
                return result;
            }
            return SUCCES;
        case TYPE_SEMICOLON:
            return SUCCES;
        case TYPE_RBRACKET:
            return SUCCES;
        case TYPE_ASSIGN:
            activeInstruction = setActiveInstruction();
            getNextToken(sToken);
            result = statList();
            if(result != SUCCES){
                return result;
            }
            return SUCCES;
        case KEYWORD_IF:
            result = declrList();
            if(result != SUCCES){
                return result;
            }
            return SUCCES;
        case KEYWORD_ELSE:
            result = declrList();
            if(result != SUCCES){
                return result;
            }
            return SUCCES;
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
i
    
 //todo
}
int parametrs(int option, int repeat){
        switch (option) {
            case 1: // kontrolujeme parametry funkce
                getNextToken(sToken);
                if (sToken->type == TYPE_RBRACKET && repeat == 1) {
                    return SUCCES;
                } else if (sToken->type == KEYWORD_INT ||
                           sToken->type == KEYWORD_FLOAT ||
                           sToken->type == KEYWORD_STRING
                        ) {
                    getNextToken(sToken);
                    if (sToken->type == TYPE_VARIABLE) {
                        generateInstruction();
                        BVSInsert(insideFunction, *sToken);
                        getNextToken(sToken);
                        if (sToken->type == TYPE_COMMA) {
                            repeat++;
                            parametrs(PARAM_FUNCTION, repeat);
                        } else if (sToken->type == TYPE_RBRACKET) {
                            return SUCCES;
                        } else {
                            return SYN_ERROR;
                        }
                    } else {
                        return SYN_ERROR;
                    }

                } else {
                    return SYN_ERROR;
                }
            case 2: // kontrolujeme podminku ve while nebo if

                getNextToken(sToken);
                
            case 3: // write
                if(getNextToken(sToken) == LEX_ERROR){
                    return  LEX_ERROR;
                }
                switch (sToken->type) {
                    case TYPE_VARIABLE:
                        if(getNextToken(sToken) == LEX_ERROR){
                            return  LEX_ERROR;
                        }
                        if(sToken->type == TYPE_RBRACKET){
                            return SUCCES;
                        }
                        else if(sToken->type == TYPE_COMMA){
                            repeat++;
                            parametrs(PARAM_WRITE, repeat);
                        }
                        else{
                            return SYN_ERROR;
                        }
                    case TYPE_STRING:
                        setActiveInstruction(WRITE, *sToken, NULL, NULL, NULL);
                        if(getNextToken(sToken) == LEX_ERROR){
                            return  LEX_ERROR;
                        }
                        if(sToken->type == TYPE_RBRACKET){
                            return SUCCES;
                        }
                        else if(sToken->type == TYPE_COMMA){
                            repeat++;
                            parametrs(PARAM_WRITE, repeat);
                        }
                        else{
                            return SYN_ERROR;
                        }
                    case TYPE_INTEGER_NUMBER:
                        getNextToken(sToken);
                        if(sToken->type == TYPE_RBRACKET){
                            return SUCCES;
                        }
                        else if(sToken->type == TYPE_COMMA){
                            repeat++;
                            parametrs(PARAM_WRITE, repeat);
                        }
                        else{
                            return SYN_ERROR;
                        }
                    case TYPE_DOUBLE_NUMBER:
                        getNextToken(sToken);
                        if(sToken->type == TYPE_RBRACKET){
                            return SUCCES;
                        }
                        else if(sToken->type == TYPE_COMMA){
                            repeat++;
                            parametrs(PARAM_WRITE, repeat);
                        }
                        else{
                            return SYN_ERROR;
                        }
                }
            case 4: // readi
                getNextToken(sToken);
                if(sToken->type == TYPE_RBRACKET){
                    return SUCCES;
                }
                else{
                    return SYN_ERROR;
                }
            case 5: // reads
                getNextToken(sToken);
                if(sToken->type == TYPE_RBRACKET){
                    return SUCCES;
                }
                else{
                    return SYN_ERROR;
                }
            case 6: // readf
                getNextToken(sToken);
                if(sToken->type == TYPE_RBRACKET){
                    return SUCCES;
                }
                else{
                    return SYN_ERROR;
                }
            case 7: // strlen
                getNextToken(sToken);
                if(sToken->type == KEYWORD_STRING){
                    getNextToken(sToken);
                    if(sToken->type == TYPE_VARIABLE){
                        getNextToken(sToken);
                        if(sToken->type == TYPE_RBRACKET){
                            return SUCCES;
                        }
                    }
                }
                return SYN_ERROR;
            case 8: // substring
                getNextToken(sToken);
                if(sToken->type == KEYWORD_STRING){
                    getNextToken(sToken);
                    if(sToken->type == TYPE_VARIABLE){
                        getNextToken(sToken);
                        if(sToken->type == TYPE_COMMA){
                            getNextToken(sToken);
                            if(sToken->type == KEYWORD_INT){
                                getNextToken(sToken);
                                if(sToken->type == TYPE_VARIABLE){
                                    getNextToken(sToken);
                                    if(sToken->type == TYPE_COMMA){
                                        getNextToken(sToken);
                                        if(sToken->type == KEYWORD_INT){
                                            getNextToken(sToken);
                                            if(sToken->type == TYPE_VARIABLE){
                                                getNextToken(sToken);
                                                if(sToken->type == TYPE_RBRACKET){
                                                    return SUCCES;
                                                }
                                            }
                                        }

                                    }
                                }
                            }
                        }
                    }
                }
                return SYN_ERROR;
            case 9: // ord
                getNextToken(sToken);
                if(sToken->type == KEYWORD_STRING){
                    getNextToken(sToken);
                    if(sToken->type == TYPE_VARIABLE){
                        getNextToken(sToken);
                        if(sToken->type == TYPE_RBRACKET);{
                            return SUCCES;
                        }
                    }
                }
                return SYN_ERROR;
            case 10: //chr
                getNextToken(sToken);
                if(sToken->type == KEYWORD_INT){
                    getNextToken(sToken);
                    if(sToken->type == TYPE_VARIABLE){
                        getNextToken(sToken);
                        if(sToken->type == TYPE_RBRACKET){
                            return SUCCES;
                        }
                    }
                }
                return SYN_ERROR;
        }
}

//function that calls function declr list and statlist depending on the type, also checks if there is end of file after
// or not
int program(){

    int result;
    switch (sToken->type) {
        case TYPE_IDENTIFIER:
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


        case TYPE_NOT_EQUAL:
            return SYN_ERROR;
        case TYPE_EXPONENT_NUMBER:
        case TYPE_DOUBLE_NUMBER:
        case TYPE_INTEGER_NUMBER:
        case TYPE_STRING:
        case TYPE_VARIABLE:
        case TYPE_KEYWORD:
            switch (sToken->type) {
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
int parse(DLLElementPtr *iList){

    int result;
    list = iList;
    //todo fix init token function
    initToken(sToken);
    instructionInit(list);
    if((tokenId = getNextToken(sToken)) == LEX_ERROR){
        return LEX_ERROR;
    }
    else{
        result = program();
    }
    //todo freeToken() funtcion
    return result;
}




