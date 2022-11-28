#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "lexical.h"
#include "str.h"
#include "parser.h"
#include "symtable.h"

//todo think about using enum and redefining token structure !!!
//todo function declrlist -> starts a type(checks if something is declared or not etc.) for all types that need it
//todo function statlist that ends functions (checks for right bracket, semicolon, etc.) for all types that need it
//todo generateinstruction(idinstruukce, pointrprev, pointractive, pouinternanext) -> function that inserts active instruction into instruction list
//todo TInst setactiveinstruction(const int *type, void *op1, void *op2, void *op3)

TRoot *insideFunction;
TRootf *functionNames;
TRoot *mainTree;

DLList *list;

bool in_function = false;
bool canParseEnd = false;
bool returnCount = false;

TNodef *call_function_save; //////////////////////////////// new
 
int unique = 0;
int condCounter = 0;
int tokenId;
//function_save *fun_id;

int incId = 1;



// declaration because function is used before definition

//check declare function, checks beginning of token type for example checks if there is a left bracket after while,
// recursively calls itself
int declrList(token *sToken, function_save *fun_id) {
    int paramError;
    int result;

    switch (sToken->type) {
        case TYPE_RVINCULUM:
            return SUCCES;
        case TYPE_IDENTIFIER:

            if (strCmpConstStr(sToken->content.str, "write") == 0) {
                canParseEnd = false;

                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if (sToken->type == TYPE_LBRACKET) {
                    paramError = parametrs(PARAM_WRITE, 1,  sToken, fun_id);
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
                        result = statlist(sToken, fun_id);
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
            if (strCmpConstStr(sToken->content.str, "reads") == 0) {
                canParseEnd = false;

                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if (sToken->type == TYPE_LBRACKET) {
                    paramError = parametrs(PARAM_READS, 1, sToken, fun_id);
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
            if (strCmpConstStr(sToken->content.str, "readi") == 0) {
                canParseEnd = false;

                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if (sToken->type == TYPE_LBRACKET) {
                    paramError = parametrs(PARAM_READI, 1, sToken, fun_id);
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
                        result = statlist(sToken, fun_id);
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
            if (strCmpConstStr(sToken->content.str, "readf") == 0) {
                canParseEnd = false;

                if ((result = getNextToken(sToken)) != SUCCES) {
                    return result;
                }
                if (sToken->type == TYPE_LBRACKET) {
                    paramError = parametrs(PARAM_READF, 1, sToken, fun_id);
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
            if (strCmpConstStr(sToken->content.str, "strlen") == 0) {
                canParseEnd = false;

                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if (sToken->type == TYPE_LBRACKET) {
                    paramError = parametrs(PARAM_STRLEN, 1, sToken, fun_id);
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
            if (strCmpConstStr(sToken->content.str, "substring") == 0) {
                canParseEnd = false;

                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }

                if (sToken->type == TYPE_LBRACKET) {
                    paramError = parametrs(PARAM_SUBSTRING, 1, sToken, fun_id);
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
            if (strCmpConstStr(sToken->content.str, "ord") == 0) {
                canParseEnd = false;

                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }

                if (sToken->type == TYPE_LBRACKET) {
                    paramError = parametrs(PARAM_SUBSTRING, 1, sToken, fun_id);
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
            if (strCmpConstStr(sToken->content.str, "chr") == 0) {
                canParseEnd = false;

                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if (sToken->type == TYPE_LBRACKET) {
                    paramError = parametrs(PARAM_CHR, 1, sToken, fun_id);
                    if (paramError == SUCCES) {
                        if (getNextToken(sToken) != TYPE_SEMICOLON) {
                            //instructionFree()
                            return SYN_ERROR;
                        }
                        //todo instructionFree()
                        canParseEnd = true;

                        if((result = getNextToken(sToken)) != SUCCES){
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
            //todo bvssearch nust be true
            if (BVSSearch_function(functionNames->rootPtr, *sToken) != NULL) {
                canParseEnd = false;
                call_function_save = BVSSearch_function(functionNames->rootPtr, *sToken);
                printf("%s &function%s\n", JUMP, sToken->content.str->str);
                printf("%s &return%s\n", LABEL, sToken->content.str->str);
                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if(sToken->type != TYPE_LBRACKET){
                    return SYN_ERROR;
                }    
                
                paramError = parametrs(PARAM_FUNCTION_CALL, 1, sToken, fun_id);
                if(paramError == SUCCES){

                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }
                   
                    //todo nevim kam to patří správně
                    result = statlist(sToken, fun_id);
                    if(result != SUCCES){
                        return result;
                    }
                    return SUCCES;
                }

            } else {
                return SEM_ERROR;
            }

        case TYPE_FUNCTIONDECLARE:
            if(in_function){
                return SYN_ERROR;
            }
            if(BVSSearch_function(functionNames->rootPtr, *sToken) != NULL){
                return SEM_DEFINE_ERROR;
            }
            fun_id->content->str = sToken->content.str->str;
            printf("%s &function%send\n", JUMP, fun_id->content->str);
            printf("%s &function%s\n", LABEL, fun_id->content->str);
            printf("%s\n", CREATEFRAME);
            printf("%s\n", PUSHFRAME);
            printf("%s LF@&return_val\n", DEFVAR);
            printf("%s LF@&return_val %s\n", MOVE, NIL);
            
            if((result = getNextToken(sToken)) != SUCCES) {
                return result;
            }
            if (sToken->type != TYPE_LBRACKET) {
                return SYN_ERROR;
            }
            else{
                in_function = true;
                paramError = parametrs(PARAM_FUNCTION, 1, sToken, fun_id);
                BVSInsert_function(functionNames->rootPtr, *fun_id);
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
                result = statlist(sToken, fun_id);
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
                printf("%s &function%send\n", LABEL, fun_id->content->str);
                BVSDispose(insideFunction);
                result = statlist(sToken, fun_id);
                if(result != SUCCES){
                    return result;
                }
                return SUCCES;
            }

        case KEYWORD_RETURN :
            paramError = parametrs(PARAM_RETURN, 1, sToken, fun_id);
            if(paramError != SUCCES){
                return  paramError;
            }else if(in_function == true){
                returnCount = true;
                result = statlist(sToken, fun_id);
                if(result != SUCCES){
                    return result;
                }
                return SUCCES;
            } else{
                return SEM_ERROR;
            }


        case KEYWORD_WHILE:
            unique++;
            condCounter = unique;
            printf("%s &while%d\n", LABEL, condCounter);
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
                    result = statlist(sToken, fun_id);
                    if (result != SUCCES) {
                        return result;
                    }
                    return SUCCES;
                }
                else {
                    result = statlist(sToken, fun_id);
                    if (result != SUCCES) {
                        return result;
                    }
                    if(sToken->type != TYPE_RVINCULUM) {
                        return SYN_ERROR;
                    }
                    if(getNextToken(sToken) == LEX_ERROR) {
                        return LEX_ERROR;
                    }
                    result = statlist(sToken, fun_id);
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
            unique+= 1;
            condCounter = unique;
            if(getNextToken(sToken) == LEX_ERROR) {
                return LEX_ERROR;
            }
            if (sToken->type != TYPE_LBRACKET) {
                return SYN_ERROR;
            } else {
                paramError = parametrs(PARAM_IF_WHILE,1, sToken, fun_id);
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
                        result = statlist(sToken, fun_id);
                        if (result != SUCCES) {
                            return result;
                        }
                        return SUCCES;
                    }
                    else {
                        result = statlist(sToken, fun_id);
                        if (result != SUCCES) {
                            return result;
                        }
                        if(sToken->type != TYPE_RVINCULUM) {
                            return SYN_ERROR;
                        }
                        if(getNextToken(sToken) == LEX_ERROR) {
                            return LEX_ERROR;
                        }
                        result = statlist(sToken, fun_id);
                        if (result != SUCCES) {
                            return result;
                        }
                        return SUCCES;
                    }
                }
            }
        case KEYWORD_ELSE:
            printf("%s &else%d\n", LABEL, condCounter);
            condCounter--;
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
                result = statlist(sToken, fun_id);
                if (result != SUCCES) {
                    return result;
                }
                return SUCCES;
            }else {
                result = statlist(sToken, fun_id);
                if (result != SUCCES) {
                    return result;
                }
                if (sToken->type != TYPE_RVINCULUM) {
                    return SYN_ERROR;
                }
                result = statlist(sToken, fun_id);
                if (result != SUCCES) {
                    return result;
                }
                return SUCCES;
            }
    }
    return SYN_ERROR;
}
//check after function, checks ending of token type for example checks if there is a right bracket after condition of
// while, recursively calls itself
//also calls main fynction stat
int statlist(token *sToken, function_save *fun_id){

    int result;

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
            return declrList(sToken, fun_id);



        case TYPE_RVINCULUM:
            return SUCCES;

        case TYPE_VARIABLE:
            BVSInsert(mainTree, *sToken);
            getNextToken(sToken);
            result = statlist(sToken, fun_id);
            if(result != SUCCES){
                return result;
            }
            return SUCCES;

        case TYPE_END_OF_FILE:
            if(canParseEnd == true){
                return SUCCES;
            }
            else{
                return SYN_ERROR;
            }
        case TYPE_IDENTIFIER:
            result = declrList(sToken, fun_id);
            if(result != SUCCES){
                return result;
            }
            return SUCCES;

        case TYPE_SEMICOLON:
        case TYPE_RBRACKET:
            return SUCCES;

        case TYPE_ASSIGN:

            getNextToken(sToken);
            result = statlist(sToken, fun_id);
            if(result != SUCCES){
                return result;
            }
            return SUCCES;

        case KEYWORD_IF:
        case KEYWORD_ELSE:
        case KEYWORD_WHILE:
            result = declrList(sToken, fun_id);
            if(result != SUCCES){
                return result;
            }
            return SUCCES;

        case KEYWORD_FUNCTION:
            if((result = getNextToken(sToken)) != SUCCES){
                return result;
            }
            sToken->type = TYPE_FUNCTIONDECLARE;
            result = declrList(sToken, fun_id);
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
            return SYN_ERROR;

    }
    return SYN_ERROR;
    //todo statList
}


//stat function checks content of different types, for example condition of while or body of while, calls function
//statlist -> statlist calls stat recursively

//todo
int parametrs(int option, int repeat, token *sToken, function_save *fun_id){

    int result;
    switch (option) {
        case PARAM_FUNCTION: // kontrolujeme parametry funkce
            if((result = getNextToken(sToken)) != SUCCES){
                return  result;
            }
            if (sToken->type == TYPE_RBRACKET && repeat == 1) {
                fun_id->param_count = 0;
                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if(sToken->type == TYPE_COLON){
                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }
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
                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if (sToken->type == TYPE_VARIABLE) {
                    BVSInsert(mainTree, *sToken);
                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }
                    if (sToken->type == TYPE_COMMA) {
                        repeat++;
                        return parametrs(PARAM_FUNCTION, repeat, sToken, fun_id);
                    } else if (sToken->type == TYPE_RBRACKET) {
                        fun_id->param_count = repeat;
                        if((result = getNextToken(sToken)) != SUCCES){
                            return  result;
                        }
                        if(sToken->type == TYPE_COLON){
                            if((result = getNextToken(sToken)) != SUCCES){
                                return  result;
                            }
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
            if((result = getNextToken(sToken)) != SUCCES){
                return  result;
            }
            switch(sToken->type){
                case TYPE_STRING:
                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }
                    if(sToken->type == TYPE_CONCATENATE ||
                       sToken->type == TYPE_EQUAL       ||
                       sToken->type == TYPE_NOT_EQUAL
                            ){
                        return parametrs(PARAM_IF_WHILE, repeat, sToken, fun_id);
                    }
                    else if(sToken->type == TYPE_RBRACKET){
                        repeat--;
                        if(repeat == 0){
                            printf("%s &else%d", JUMPIFNEQ, condCounter);
                            return SUCCES;
                        }
                        return parametrs(PARAM_IF_WHILE, repeat, sToken, fun_id);
                    }
                    else{
                        return SYN_ERROR;
                    }
                case TYPE_VARIABLE:
                    if(in_function){
                        if(BVSSearch(insideFunction->rootPtr, *sToken) == NULL){
                            return SEM_UNDEFINED_ERROR;
                        }
                    }
                    else{
                        if(BVSSearch(mainTree->rootPtr, *sToken) == NULL){
                            return SEM_UNDEFINED_ERROR;
                        }
                    }
                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }
                    if(sToken->type == TYPE_MULTIPLY         ||
                       sToken->type == TYPE_DIVIDE           ||
                       sToken->type == TYPE_ADDITION         ||
                       sToken->type == TYPE_SUBTRACTION      ||
                       sToken->type == TYPE_SMALLER_THAN     ||
                       sToken->type == TYPE_GREATER_THAN     ||
                       sToken->type == TYPE_GREATER_OR_EQUAL ||
                       sToken->type == TYPE_SMALLER_OR_EQUAL ||
                       sToken->type == TYPE_EQUAL            ||
                       sToken->type == TYPE_NOT_EQUAL
                            ){
                        return parametrs(PARAM_IF_WHILE, repeat, sToken, fun_id);
                    }
                    else if(sToken->type == TYPE_RBRACKET){
                        repeat--;
                        if(repeat == 0){
                            return SUCCES;
                        }
                        return parametrs(PARAM_IF_WHILE, repeat, sToken, fun_id);
                    }
                    else{
                        return SYN_ERROR;
                    }
                case TYPE_RBRACKET:
                    repeat--;
                    if(repeat == 0){
                        return SUCCES;
                    }
                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
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
                        return parametrs(PARAM_IF_WHILE, repeat, sToken, fun_id);
                    }
                    else if(sToken->type == TYPE_RBRACKET){
                        repeat--;
                        if(repeat == 0){
                            return SUCCES;
                        }
                        return parametrs(PARAM_IF_WHILE, repeat, sToken, fun_id);
                    }
                    else{
                        return SYN_ERROR;
                    }

                case TYPE_LBRACKET:
                    repeat++;
                    return parametrs(PARAM_IF_WHILE, repeat, sToken, fun_id);
                case KEYWORD_NULL:
                    
                case TYPE_INTEGER_NUMBER:
                    
                case TYPE_DOUBLE_NUMBER:
                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }
                    if(sToken->type == TYPE_MULTIPLY         ||
                       sToken->type == TYPE_DIVIDE           ||
                       sToken->type == TYPE_ADDITION         ||
                       sToken->type == TYPE_SUBTRACTION      ||
                       sToken->type == TYPE_SMALLER_THAN     ||
                       sToken->type == TYPE_GREATER_THAN     ||
                       sToken->type == TYPE_GREATER_OR_EQUAL ||
                       sToken->type == TYPE_SMALLER_OR_EQUAL ||
                       sToken->type == TYPE_EQUAL            ||
                       sToken->type == TYPE_NOT_EQUAL
                            ){
                        return parametrs(PARAM_IF_WHILE, repeat, sToken, fun_id);
                    }
                    else if(sToken->type == TYPE_RBRACKET){
                        repeat--;
                        if(repeat == 0){
                            return SUCCES;
                        }
                        return parametrs(PARAM_IF_WHILE, repeat, sToken, fun_id);
                    }
                    else{
                        return SYN_ERROR;
                    }
            }
            return SYN_ERROR;
        case PARAM_WRITE: // write
            if((result = getNextToken(sToken)) != SUCCES){
                return  result;
            }
            printf("%s ", WRITE);
            switch (sToken->type) {
                case TYPE_VARIABLE:
                    if(in_function){
                        if(BVSSearch(insideFunction->rootPtr, *sToken) == NULL){
                            return SEM_UNDEFINED_ERROR;
                        }
                    }
                    else{
                        if(BVSSearch(mainTree->rootPtr, *sToken) == NULL){
                            return SEM_UNDEFINED_ERROR;
                        }
                    }
                    DLL_InsertLast(list, *sToken);
                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }
                    if(sToken->type == TYPE_RBRACKET){
                        DLL_Print(list);
                        printf("\n");
                        DLL_Free(list);
                        return SUCCES;
                    }
                    else if(sToken->type == TYPE_COMMA){
                        repeat++;
                        return parametrs(PARAM_WRITE, repeat, sToken, fun_id);
                    }
                    else{
                        return SYN_ERROR;
                    }
                case TYPE_STRING:
                case TYPE_INTEGER_NUMBER:
                case TYPE_DOUBLE_NUMBER:
                    DLL_InsertLast(list, *sToken);
                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }
                    if(sToken->type == TYPE_RBRACKET){
                        DLL_Print(list);
                        printf("\n");
                        DLL_Free(list);
                        return SUCCES;
                    }
                    else if(sToken->type == TYPE_COMMA){
                        repeat++;
                        return parametrs(PARAM_WRITE, repeat, sToken, fun_id);
                    }
                    else{
                        return SYN_ERROR;
                    }
            }
            return SYN_ERROR;
        case PARAM_READI: // readi
        case PARAM_READS: // reads
        case PARAM_READF: // readf
            if((result = getNextToken(sToken)) != SUCCES){
                return  result;
            }
            if(sToken->type == TYPE_RBRACKET){
                return SUCCES;
            }
            else{
                return SYN_ERROR;
            }
        case PARAM_STRLEN: // strlen
            if((result = getNextToken(sToken)) != SUCCES){
                return  result;
            }
            if(sToken->type == TYPE_VARIABLE){
                if(in_function){
                    if(BVSSearch(insideFunction->rootPtr, *sToken) == NULL){
                        return SEM_UNDEFINED_ERROR;
                    }
                }
                else{
                    if(BVSSearch(mainTree->rootPtr, *sToken) == NULL){
                        return SEM_UNDEFINED_ERROR;
                    }
                }
                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if(sToken->type == TYPE_RBRACKET){
                    return SUCCES;
                }
            }
            return SYN_ERROR;
        case PARAM_SUBSTRING: // substring
            if((result = getNextToken(sToken)) != SUCCES){
                return  result;
            }
            if(sToken->type == TYPE_VARIABLE){
                if(in_function){
                    if(BVSSearch(insideFunction->rootPtr, *sToken) == NULL){
                        return SEM_UNDEFINED_ERROR;
                    }
                }
                else{
                    if(BVSSearch(mainTree->rootPtr, *sToken) == NULL){
                        return SEM_UNDEFINED_ERROR;
                    }
                }
                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if(sToken->type == TYPE_COMMA){
                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }
                    if(sToken->type == TYPE_VARIABLE){
                        if(in_function){
                            if(BVSSearch(insideFunction->rootPtr, *sToken) == NULL){
                                return SEM_UNDEFINED_ERROR;
                            }
                        }
                        else{
                            if(BVSSearch(mainTree->rootPtr, *sToken) == NULL){
                                return SEM_UNDEFINED_ERROR;
                            }
                        }
                        if((result = getNextToken(sToken)) != SUCCES){
                            return  result;
                        }
                        if(sToken->type == TYPE_COMMA){
                            if((result = getNextToken(sToken)) != SUCCES){
                                return  result;
                            }
                            if(sToken->type == TYPE_VARIABLE){
                                if(in_function){
                                    if(BVSSearch(insideFunction->rootPtr, *sToken) == NULL){
                                        return SEM_UNDEFINED_ERROR;
                                    }
                                }
                                else{
                                    if(BVSSearch(mainTree->rootPtr, *sToken) == NULL){
                                        return SEM_UNDEFINED_ERROR;
                                    }
                                }
                                if((result = getNextToken(sToken)) != SUCCES){
                                    return  result;
                                }
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
        case PARAM_CHR: //chr
            if((result = getNextToken(sToken)) != SUCCES){
                return  result;
            }
            if(sToken->type == TYPE_VARIABLE){
                if(in_function){
                    if(BVSSearch(insideFunction->rootPtr, *sToken) == NULL){
                        return SEM_UNDEFINED_ERROR;
                    }
                }
                else{
                    if(BVSSearch(mainTree->rootPtr, *sToken) == NULL){
                        return SEM_UNDEFINED_ERROR;
                    }
                }
                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if(sToken->type == TYPE_RBRACKET){
                    return SUCCES;
                }
            }
            return SYN_ERROR;
        case PARAM_RETURN: // function return value
            if((result = getNextToken(sToken)) != SUCCES){
                return  result;
            }
            switch(sToken->type){
                case TYPE_IDENTIFIER:
                    if(BVSSearch_function(functionNames->rootPtr, *sToken) == NULL){
                        return SEM_DEFINE_ERROR;
                    }
                    TNodef *tmp = BVSSearch_function(functionNames->rootPtr, *sToken);
                    if(tmp->return_type != fun_id->ret_value){
                        return SEM_COUNT_ERROR;
                    }
                    call_function_save = BVSSearch_function(functionNames->rootPtr, *sToken);
                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }   
                    if(sToken->type != TYPE_LBRACKET){
                        return SYN_ERROR;
                    }
                    result = parametrs(PARAM_FUNCTION_CALL, 1, sToken, fun_id);
                    if(result != SUCCES){
                        return result;
                    }
                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }   
                    if(sToken->type == TYPE_SEMICOLON){
                        return SUCCES;
                    }else if(sToken->type == TYPE_ADDITION || 
                       sToken->type == TYPE_DIVIDE   || 
                       sToken->type == TYPE_MULTIPLY || 
                       sToken->type == TYPE_SUBTRACTION
                    ){
                        return parametrs(PARAM_RETURN, repeat, sToken, fun_id);
                    }else{
                        return SYN_ERROR;
                    }
                case TYPE_STRING:
                    if(fun_id->ret_value != KEYWORD_STRING){
                        return SEM_COUNT_ERROR;
                    }
                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }
                    if(sToken->type == TYPE_CONCATENATE){
                        repeat++;
                        return parametrs(PARAM_RETURN, repeat, sToken, fun_id);
                    }else if(sToken->type == TYPE_SEMICOLON){
                        return SUCCES;
                    }
                    return SYN_ERROR;
                case TYPE_INTEGER_NUMBER:
                    if(fun_id->ret_value != KEYWORD_INT){
                        return SEM_COUNT_ERROR;
                    }
                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }
                    if(sToken->type == TYPE_ADDITION || 
                       sToken->type == TYPE_DIVIDE   || 
                       sToken->type == TYPE_MULTIPLY || 
                       sToken->type == TYPE_SUBTRACTION
                    ){
                        repeat++;
                        return parametrs(PARAM_RETURN, repeat, sToken, fun_id);
                    }else if(sToken->type == TYPE_SEMICOLON){
                        return SUCCES;
                    }
                    return SYN_ERROR;
                case TYPE_DOUBLE_NUMBER:
                    if(fun_id->ret_value != KEYWORD_FLOAT){
                        return SEM_COUNT_ERROR;
                    }
                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }
                    if(sToken->type == TYPE_ADDITION || 
                       sToken->type == TYPE_DIVIDE   || 
                       sToken->type == TYPE_MULTIPLY || 
                       sToken->type == TYPE_SUBTRACTION
                    ){
                        repeat++;
                        return parametrs(PARAM_RETURN, repeat, sToken, fun_id);
                    }else if(sToken->type == TYPE_SEMICOLON){
                        return SUCCES;
                    }
                    return SYN_ERROR;
                case TYPE_SEMICOLON:
                    if(repeat == 1){
                        if(fun_id->ret_value == KEYWORD_VOID){
                            return SUCCES;
                        }
                        return SEM_COUNT_ERROR;
                    }
                    return SYN_ERROR;
            }
            return SYN_ERROR;
        case PARAM_FUNCTION_CALL: // calling function
            if((result = getNextToken(sToken)) != SUCCES){
                return  result;
            }
            switch (sToken->type)
            {
                case TYPE_RBRACKET:
                    if(repeat == 1){
                        if(call_function_save->parameters == 0){
                            return SUCCES;
                        }else{
                            return SEM_COUNT_ERROR;
                        }
                    } 
                case TYPE_VARIABLE:
                    if(in_function){
                        if(BVSSearch(insideFunction->rootPtr, *sToken) == NULL){
                            return SEM_UNDEFINED_ERROR;
                        }
                    }
                    else{
                        if(BVSSearch(mainTree->rootPtr, *sToken) == NULL){
                            return SEM_UNDEFINED_ERROR;
                        }
                    }
                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }
                    if(sToken->type == TYPE_RBRACKET){
                        if(repeat == call_function_save->parameters){
                            return SUCCES;
                        }
                        return SEM_COUNT_ERROR;
                    }else if(sToken->type == TYPE_COMMA){
                        repeat++;
                        return parametrs(PARAM_FUNCTION_CALL, repeat, sToken, fun_id);
                    }else if(sToken->type == TYPE_ADDITION || 
                             sToken->type == TYPE_DIVIDE   || 
                             sToken->type == TYPE_MULTIPLY || 
                             sToken->type == TYPE_SUBTRACTION
                             ){
                        return parametrs(PARAM_FUNCTION_CALL, repeat, sToken, fun_id);
                    }
                case TYPE_INTEGER_NUMBER:
                case TYPE_DOUBLE_NUMBER:
                case TYPE_STRING:
                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }
                    if(sToken->type == TYPE_RBRACKET){
                        if(repeat == call_function_save->parameters){
                            return SUCCES;
                        }
                        return SEM_COUNT_ERROR;
                    }else if(sToken->type == TYPE_COMMA){
                        repeat++;
                        return parametrs(PARAM_FUNCTION_CALL, repeat, sToken, fun_id);
                    }else{
                        return SYN_ERROR;
                    }
            }
            return SYN_ERROR;
    }
    return SYN_ERROR;
}


//function that initializes tree, list of instructions, token, reads first token and calls program() function
int parse(void){
    canParseEnd = true;
    setSourceFile(stdin);
    token init;
    string initStr;
    function_save initek;

    initStr.str = NULL;
    initStr.length = 0;
    initStr.alloc = 0;
    init.type = 110;
    init.content.str = &initStr;
    initek.content->str = &initStr;
    initek.param_count = 0;
    initek.ret_value = 0;
    token *sToken;
    sToken = &init;
    function_save *fun_id;
    fun_id = &initek;

    TRoot initMain;
    TRoot initInside;
    TRootf initNames;

    mainTree = &initMain;
    functionNames = &initNames;
    insideFunction = &initInside;
    //initializing tree
    BVSInit(insideFunction);
    BVSInit(mainTree);
    BVSInit_function(functionNames);
    
    int result;
    
    DLL_Init(list);
    //todo fix init token function

    if((tokenId = getNextToken(sToken)) == LEX_ERROR){
        return LEX_ERROR;
    }
    else{
        result = statlist(sToken, fun_id);
        //printf("EXIT %d", result);
    }
    fprintf(stderr, "--%d--", result);
    BVSFree(mainTree);
    BVSFree(insideFunction);
    BVSFree_function(functionNames);
    //todo freeToken() funtcion
    return result;
}
