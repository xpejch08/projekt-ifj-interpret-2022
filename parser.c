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

TRoot *insideFunction = NULL;
TRootf *functionNames = NULL;
TRoot *mainTree = NULL;
DLLElementPtr *list;

bool in_function = false;
bool canParseEnd = false;
bool returnCount = false;

int unique = 0;
int ifCounter = 0;

int tokenId;
token *sToken;
function_save *fun_id;

int incId = 1;




int stat(); // function declaration;
int statlist();
int parametrs(); // declaration because function is used before definition

//check declare function, checks beginning of token type for example checks if there is a left bracket after while,
// recursively calls itself
int declrList() {

    int paramError;
    int result;

    switch (sToken->type) {
        case TYPE_RVINCULUM:
            return SUCCES;
        case TYPE_IDENTIFIER:

            if (strCmpConstStr(sToken->content.str, "write")) {
                canParseEnd = false;

                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
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
                        
                        //todo instructionFree()
                        canParseEnd = true;

                        if((result = getNextToken(sToken)) != SUCCES){
                            return  result;
                        }
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
            if (strCmpConstStr(sToken->content.str, "reads")) {
                canParseEnd = false;

                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if (sToken->type == TYPE_LBRACKET) {
                    paramError = parametrs(PARAM_READS, 1); 
                    if (paramError == SUCCES) {
                        if (getNextToken(sToken) != TYPE_SEMICOLON) {
                            //instructionFree()
                            return SYN_ERROR;
                        }
                       
                        //todo instructionFree()
                        canParseEnd = true;

                        if((result = getNextToken(sToken)) != SUCCES){
                            return  result;
                        }
                        return SUCCES;
                    } else {
                        return paramError;
                    }
                } else {
                    return SYN_ERROR;
                }
            }
            if (strCmpConstStr(sToken->content.str, "readi")) {
                canParseEnd = false;

                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
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

                        canParseEnd = true;

                        if((result = getNextToken(sToken)) != SUCCES){
                            return  result;
                        }
                        result = statlist();
                        if(result != SUCCES){
                            return result;
                        }
                        else {
                            return SUCCES;
                        }
                    } else {
                        return paramError;
                    }
                } else {
                    return SYN_ERROR;
                }
            }
            if (strCmpConstStr(sToken->content.str, "readf")) {
                canParseEnd = false;

                if ((result = getNextToken(sToken)) != SUCCES) {
                    return result;
                }
                if (sToken->type == TYPE_LBRACKET) {
                    paramError = parametrs(PARAM_READF, 1);
                    if (paramError == SUCCES) {
                        if (getNextToken(sToken) != TYPE_SEMICOLON) {
                            //instructionFree()
                            return SYN_ERROR;
                        }
                        
                        //todo instructionFree()
                        canParseEnd = true;

                        if ((result = getNextToken(sToken)) != SUCCES) {
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
            if (strCmpConstStr(sToken->content.str, "strlen")) {
                canParseEnd = false;

                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if (sToken->type == TYPE_LBRACKET) {
                    paramError == parametrs(PARAM_STRLEN, 1);
                    if (paramError == SUCCES) {
                        if (getNextToken(sToken) != TYPE_SEMICOLON) {
                            //instructionFree()
                            return SYN_ERROR;
                        }
                        //todo instructionFree()
                        canParseEnd = true;

                        if((result = getNextToken(sToken)) != SUCCES){
                            return  result;
                        }
                        return SUCCES;
                    } else {
                        return paramError;
                    }
                } else {
                    return SYN_ERROR;
                }
            }
            if (strCmpConstStr(sToken->content.str, "substring")) {
                canParseEnd = false;

                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }

                if (sToken->type == TYPE_LBRACKET) {
                    paramError = parametrs(PARAM_SUBSTRING, 1); 
                    if (paramError == SUCCES) {
                        if (getNextToken(sToken) != TYPE_SEMICOLON) {
                            //instructionFree()
                            return SYN_ERROR;
                        }
                        //todo instructionFree()
                        canParseEnd = true;

                        if((result = getNextToken(sToken)) != SUCCES){
                            return  result;
                        }
                        return SUCCES;
                    } else {
                        return paramError;
                    }
                } else {
                    return SYN_ERROR;
                }
            }
            if (strCmpConstStr(sToken->content.str, "ord")) {
                canParseEnd = false;

                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }

                if (sToken->type == TYPE_LBRACKET) {
                    paramError = parametrs(PARAM_SUBSTRING, 1); 
                    if (paramError == SUCCES) {
                        if (getNextToken(sToken) != TYPE_SEMICOLON) {
                            //instructionFree()
                            return SYN_ERROR;
                        }
                        //todo instructionFree()
                        canParseEnd = true;

                        if((result = getNextToken(sToken)) != SUCCES){
                            return  result;
                        }
                        return SUCCES;
                    } else {
                        return paramError;
                    }
                } else {
                    return SYN_ERROR;
                }
            }
            if (strCmpConstStr(sToken->content.str, "chr")) {
                canParseEnd = false;

                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if (sToken->type == TYPE_LBRACKET) {
                    paramError = parametrs(PARAM_CHR, 1); 
                    if (paramError == SUCCES) {
                        if (getNextToken(sToken) != TYPE_SEMICOLON) {
                            //instructionFree()
                            return SYN_ERROR;
                        }
                        //todo instructionFree()
                        canParseEnd = true;

                        if((result = getNextToken(sToken)) != SUCCES){
                            return  result;
                        }
                        return SUCCES;
                    } else {
                        return paramError;
                    }
                } else {
                    return SYN_ERROR;
                }
            }
            //todo bvssearch nust be true
            if (BVSSearch(functionNames, *sToken) != NULL) {
                canParseEnd = false;
                BVSInsert(mainTree, *sToken);

                paramError = parametrs(PARAM_FUNCTION, 1); //////////////////////// change to new case in parametrs fun
                if(paramError == SUCCES){

                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }
                    if(sToken->type != TYPE_SEMICOLON){
                        return SYN_ERROR;
                    }
                    //todo nevim kam to patří správně
                    result = statlist();
                    if(result != SUCCES){
                        return result;
                    }
                    return SUCCES;
                }

            } else {
                return SEM_ERROR;
            }

        case TYPE_FUNCTIONDECLARE:
            if(BVSSearch_function(functionNames, *sToken) != NULL){
                return SEM_ERROR;
            }
            fun_id->content = sToken->content.str;
            if((result = getNextToken(sToken)) != SUCCES) {
                return result;
            }
            if (sToken->type != TYPE_LBRACKET) {
                return SYN_ERROR;
            }
            else{
                in_function = true;
                paramError = parametrs(PARAM_FUNCTION, 1);
                BVSInsert_function(functionNames, *fun_id);
                if (paramError != SUCCES) {
                    return paramError;
                }
                else{
                    if ((result = getNextToken(sToken)) != SUCCES) {
                        return result;
                    }
                    if (sToken->type != TYPE_LVINCULUM) {
                        return SYN_ERROR;
                    }

                }
                result = statlist();
                if(result != SUCCES){
                    return result;
                }
                if((result = getNextToken(sToken)) != SUCCES) {
                    return result;
                }
                if((sToken->type != TYPE_RVINCULUM) || returnCount != true){
                    return SYN_ERROR;
                }
                returnCount = false;
                in_function = false;
                result = statlist();
                if(result != SUCCES){
                    return result;
                }
                return SUCCES;
            }

        case KEYWORD_RETURN :
            paramError = parametrs(PARAM_RETURN, 1);
            if(paramError != SUCCES){
                return  paramError;
            }else if(in_function == true){
                returnCount = true;
                result = statlist();
                if(result != SUCCES){
                    return result;
                }
                return SUCCES;
            } else{
                return SEM_ERROR;
            }


        case KEYWORD_WHILE:
            if((result = getNextToken(sToken)) != SUCCES) {
                return result;
            }
            if (sToken->type != TYPE_LBRACKET) {
                return SYN_ERROR;
            }
            else {
                    if((result = getNextToken(sToken)) != SUCCES) {
                        return result;
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

        case KEYWORD_WHILE:
            result = declrList();
            if(result != SUCCES){
                return result;
            }
            return SUCCES;

        case KEYWORD_FUNCTION:
            if((result = getNextToken(sToken)) != SUCCES){
                return result;
            }
            sToken->type = TYPE_FUNCTIONDECLARE;
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
            ;
    }
    //todo statList
}


//stat function checks content of different types, for example condition of while or body of while, calls function
//statlist -> statlist calls stat recursively
    
 //todo
int parametrs(int option, int repeat){
        switch (option) {
            case PARAM_FUNCTION: // kontrolujeme parametry funkce
                getNextToken(sToken);
                if (sToken->type == TYPE_RBRACKET && repeat == 1) {
                    fun_id->param_count = 0;
                    getNextToken(sToken);
                    if(sToken->type == TYPE_COLON){
                        getNextToken(sToken);
                        if(sToken->type == KEYWORD_VOID){
                            fun_id->ret_value = KEYWORD_VOID;
                            return SUCCES;
                        }
                        else if(sToken->type == KEYWORD_FLOAT){
                            fun_id->ret_value = KEYWORD_FLOAT;
                            return SUCCES;
                        }
                        else if(sToken->type == KEYWORD_INT){
                            fun_id->ret_value = KEYWORD_INT;
                            return SUCCES;
                        }
                        else if(sToken->type == KEYWORD_STRING){
                            fun_id->ret_value = KEYWORD_STRING;
                            return SUCCES;
                        }
                        else{
                            return SYN_ERROR;
                        }
                    }
                    else{
                        return SYN_ERROR;
                    }
                } else if (sToken->type == KEYWORD_INT ||
                           sToken->type == KEYWORD_FLOAT ||
                           sToken->type == KEYWORD_STRING
                        ) {
                    getNextToken(sToken);
                    if (sToken->type == TYPE_VARIABLE) {
                        
                        BVSInsert(insideFunction, *sToken);
                        getNextToken(sToken);
                        if (sToken->type == TYPE_COMMA) {
                            repeat++;
                            parametrs(PARAM_FUNCTION, repeat);
                        } else if (sToken->type == TYPE_RBRACKET) {
                            fun_id->param_count = repeat;
                            getNextToken(sToken);
                            if(sToken->type == TYPE_COLON){
                                getNextToken(sToken);
                                if(sToken->type == KEYWORD_VOID){
                                    fun_id->ret_value = KEYWORD_VOID;
                                    return SUCCES;
                                }
                                else if(sToken->type == KEYWORD_FLOAT){
                                    fun_id->ret_value = KEYWORD_FLOAT;
                                    return SUCCES;
                                }
                                else if(sToken->type == KEYWORD_INT){
                                    fun_id->ret_value = KEYWORD_INT;
                                    return SUCCES;
                                }
                                else if(sToken->type == KEYWORD_STRING){
                                    fun_id->ret_value = KEYWORD_STRING;
                                    return SUCCES;
                                }
                                else{
                                    return SYN_ERROR;
                                }
                            }
                            else{
                                return SYN_ERROR;
                            }
                        } else {
                            return SYN_ERROR;
                        }
                    } else {
                        return SYN_ERROR;
                        }
                } else {
                    return SYN_ERROR;
                }
            case PARAM_IF_WHILE: // kontrolujeme podminku ve while nebo if
                if(getNextToken(sToken) == LEX_ERROR){
                    return  LEX_ERROR;
                }
                switch(sToken->type){
                    case TYPE_STRING:
                        if(getNextToken(sToken) == LEX_ERROR){
                            return  LEX_ERROR;
                        }
                        if(sToken->type == TYPE_CONCATENATE ||
                           sToken->type == TYPE_EQUAL       ||
                           sToken->type == TYPE_NOT_EQUAL    
                        ){
                            parametrs(PARAM_IF_WHILE, repeat);
                        }
                        else if(sToken->type == TYPE_RBRACKET){
                            repeat--;
                            if(repeat == 0){
                                return SUCCES;
                            }
                            parametrs(PARAM_IF_WHILE, repeat);
                        }
                        else{
                            return SYN_ERROR;
                        } 
                    case TYPE_VARIABLE:
                        if(in_function){
                            if(BVSSearch(insideFunction, *sToken) == NULL){
                                return SEM_UNDEFINED_ERROR;
                            }
                        }    
                        else{
                            if(BVSSearch(mainTree, *sToken) == NULL){
                                return SEM_UNDEFINED_ERROR;
                            }
                        }
                        if(getNextToken(sToken) == LEX_ERROR){
                            return  LEX_ERROR;
                        }
                        if(sToken->type == TYPE_MULTIPLY         ||
                           sToken->type == TYPE_DIVIDE           ||
                           sToken->type == TYPE_ADDITION         ||
                           sToken->type == TYPE_SUBTRACTION      ||
                           sToken->type == TYPE_SMALLER_THAN     ||
                           sToken->type == TYPE_GREATER_THAN     ||
                           sToken->type == TYPE_GREATER_OR_EQUAL ||
                           sToken->type == TYPE_SMALLER_OR_EQUAL
                        ){
                            parametrs(PARAM_IF_WHILE, repeat);    
                        }
                        else if(sToken->type == TYPE_RBRACKET){
                            repeat--;
                            if(repeat == 0){
                                return SUCCES;
                            }
                            parametrs(PARAM_IF_WHILE, repeat);
                        }
                        else{
                            return SYN_ERROR;
                        }
                    case TYPE_RBRACKET:
                        repeat--;
                        if(repeat == 0){
                            return SUCCES;
                        }
                        if(getNextToken(sToken) == LEX_ERROR){
                            return  LEX_ERROR;
                        }
                        if(sToken->type == TYPE_MULTIPLY         ||
                           sToken->type == TYPE_DIVIDE           ||
                           sToken->type == TYPE_ADDITION         ||
                           sToken->type == TYPE_SUBTRACTION      ||
                           sToken->type == TYPE_SMALLER_THAN     ||
                           sToken->type == TYPE_GREATER_THAN     ||
                           sToken->type == TYPE_GREATER_OR_EQUAL ||
                           sToken->type == TYPE_SMALLER_OR_EQUAL
                        ){
                            parametrs(PARAM_IF_WHILE, repeat);    
                        }
                        else if(sToken->type == TYPE_RBRACKET){
                            repeat--;
                            if(repeat == 0){
                                return SUCCES;
                            }
                            parametrs(PARAM_IF_WHILE, repeat);
                        }
                        else{
                            return SYN_ERROR;
                        }
                        
                    case TYPE_LBRACKET:
                        repeat++;
                        parametrs(PARAM_IF_WHILE, repeat);
                }
                return SYN_ERROR;
            case PARAM_WRITE: // write
                if(getNextToken(sToken) == LEX_ERROR){
                    return  LEX_ERROR;
                }
                switch (sToken->type) {
                    case TYPE_VARIABLE:
                        if(in_function){
                            if(BVSSearch(insideFunction, *sToken) == NULL){
                                return SEM_UNDEFINED_ERROR;
                            }
                        }    
                        else{
                            if(BVSSearch(mainTree, *sToken) == NULL){
                                return SEM_UNDEFINED_ERROR;
                            }
                        }
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
            case PARAM_READI: // readi
                getNextToken(sToken);
                if(sToken->type == TYPE_RBRACKET){
                    return SUCCES;
                }
                else{
                    return SYN_ERROR;
                }
            case PARAM_READS: // reads
                getNextToken(sToken);
                if(sToken->type == TYPE_RBRACKET){
                    return SUCCES;
                }
                else{
                    return SYN_ERROR;
                }
            case PARAM_READF: // readf
                getNextToken(sToken);
                if(sToken->type == TYPE_RBRACKET){
                    return SUCCES;
                }
                else{
                    return SYN_ERROR;
                }
            case PARAM_STRLEN: // strlen
                getNextToken(sToken);
                if(sToken->type == TYPE_VARIABLE){
                    if(in_function){
                        if(BVSSearch(insideFunction, *sToken) == NULL){
                                return SEM_UNDEFINED_ERROR;
                        }
                    }    
                    else{
                        if(BVSSearch(mainTree, *sToken) == NULL){
                            return SEM_UNDEFINED_ERROR;
                        }
                    }
                    getNextToken(sToken);
                    if(sToken->type == TYPE_RBRACKET){
                        return SUCCES;
                    }
                }
                return SYN_ERROR;
            case PARAM_SUBSTRING: // substring
                getNextToken(sToken);
                if(sToken->type == TYPE_VARIABLE){
                    if(in_function){
                        if(BVSSearch(insideFunction, *sToken) == NULL){
                            return SEM_UNDEFINED_ERROR;
                        }
                    }    
                    else{
                        if(BVSSearch(mainTree, *sToken) == NULL){
                            return SEM_UNDEFINED_ERROR;
                        }
                    }
                   getNextToken(sToken);
                    if(sToken->type == TYPE_COMMA){
                        getNextToken(sToken);
                         if(sToken->type == TYPE_VARIABLE){
                            if(in_function){
                                if(BVSSearch(insideFunction, *sToken) == NULL){
                                    return SEM_UNDEFINED_ERROR;
                                }
                            }    
                            else{
                                if(BVSSearch(mainTree, *sToken) == NULL){
                                    return SEM_UNDEFINED_ERROR;
                                }
                            }
                            getNextToken(sToken);
                             if(sToken->type == TYPE_COMMA){
                                getNextToken(sToken);
                                if(sToken->type == TYPE_VARIABLE){
                                    if(in_function){
                                        if(BVSSearch(insideFunction, *sToken) == NULL){
                                            return SEM_UNDEFINED_ERROR;
                                        }
                                    }    
                                    else{
                                        if(BVSSearch(mainTree, *sToken) == NULL){
                                            return SEM_UNDEFINED_ERROR;
                                        }
                                    }
                                    getNextToken(sToken);
                                    if(sToken->type == TYPE_RBRACKET){
                                        return SUCCES;
                                    }
                                }
                            }
                        }
                    }
                }
                return SYN_ERROR;
            case PARAM_ORD: // ord
                getNextToken(sToken);
                if(sToken->type == TYPE_VARIABLE){
                    if(in_function){
                        if(BVSSearch(insideFunction, *sToken) == NULL){
                            return SEM_UNDEFINED_ERROR;
                        }
                    }    
                    else{
                        if(BVSSearch(mainTree, *sToken) == NULL){
                            return SEM_UNDEFINED_ERROR;
                        }
                    }
                    getNextToken(sToken);
                    if(sToken->type == TYPE_RBRACKET);{
                        return SUCCES;
                    }
                }
                return SYN_ERROR;
            case PARAM_CHR: //chr
                getNextToken(sToken);
                if(sToken->type == TYPE_VARIABLE){
                    if(in_function){
                        if(BVSSearch(insideFunction, *sToken) == NULL){
                            return SEM_UNDEFINED_ERROR;
                        }
                    }    
                    else{
                        if(BVSSearch(mainTree, *sToken) == NULL){
                            return SEM_UNDEFINED_ERROR;
                        }
                    }
                    getNextToken(sToken);
                    if(sToken->type == TYPE_RBRACKET){
                        return SUCCES;
                    }
                }
                return SYN_ERROR;
            case PARAM_RETURN:
                
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
    //initializing tree
    BVSInit(mainTree);
    BVSInit_function(functionNames);
    BVSInit(insideFunction);

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
       printf("EXIT %d", result);
    }
    //todo freeToken() funtcion
    return result;
}




