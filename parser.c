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

DLList _list;
DLList *list = &_list;
token *tmpToken;
token *tmp2Token;

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
                            return SYN_ERROR;
                        }

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
                            return SYN_ERROR;
                        }

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
                            return SYN_ERROR;
                        }

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
                            return SYN_ERROR;
                        }
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
            /**
             * @brief case for declaring a function
             */
            if (BVSSearch_function(functionNames->rootPtr, *sToken) != NULL) {
                canParseEnd = false;
                call_function_save = BVSSearch_function(functionNames->rootPtr, *sToken);
                printf("%s $%s\n", JUMP, sToken->content.str->str);
                printf("%s $return%s\n", LABEL, sToken->content.str->str);
                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if(sToken->type != TYPE_LBRACKET){
                    return SYN_ERROR;
                }    
                
                paramError = parametrs(PARAM_FUNCTION_CALL, 1, sToken, fun_id);
                if(paramError == SUCCES){
                    canParseEnd = true;
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
                return SEM_DEFINE_ERROR;
            }

        case TYPE_FUNCTIONDECLARE:
            canParseEnd = false;
            if(in_function){
                return SYN_ERROR;
            }
            if(BVSSearch_function(functionNames->rootPtr, *sToken) != NULL){
                return SEM_DEFINE_ERROR;
            }
            fun_id->content->str = sToken->content.str->str;
            printf("%s $%send\n", JUMP, fun_id->content->str);
            printf("%s $%s\n", LABEL, fun_id->content->str);
            printf("%s\n", CREATEFRAME);
            printf("%s\n", PUSHFRAME);
            printf("%s LF@$return_val\n", DEFVAR);
            printf("%s LF@$return_val %s\n", MOVE, NIL);
            
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
                canParseEnd = true;
                returnCount = false;
                in_function = false;
                printf("%s $%send\n", LABEL, fun_id->content->str);
                BVSDispose(insideFunction);
                result = statlist(sToken, fun_id);
                if(result != SUCCES){
                    return result;
                }
                return SUCCES;
            }

        case KEYWORD_RETURN :
            canParseEnd = false;
            paramError = parametrs(PARAM_RETURN, 1, sToken, fun_id);
            if(paramError != SUCCES){
                return  paramError;
            }else if(in_function == true){
                returnCount = true;
                canParseEnd = true;
                result = statlist(sToken, fun_id);
                if(result != SUCCES){
                    return result;
                }
                return SUCCES;
            } else{
                return SEM_ERROR;
            }


        case KEYWORD_WHILE:
            canParseEnd = false;
            unique++;
            condCounter = unique;
            printf("%s $while%d\n", LABEL, condCounter);
            if((result = getNextToken(sToken)) != SUCCES) {
                return result;
            }
            if (sToken->type != TYPE_LBRACKET) {
                return SYN_ERROR;
            }
            else {
                paramError = parametrs(PARAM_IF_WHILE, 1, sToken, fun_id);
                if(paramError != SUCCES){
                    return paramError;
                }
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
                    canParseEnd = true;
                    printf("%s $while%d\n", JUMP, condCounter);
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
                    canParseEnd = true;
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
            canParseEnd = true;
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
                        canParseEnd = true;
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
                        canParseEnd = true;
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
            canParseEnd = false;
            printf("%s $else%d\n", LABEL, condCounter);
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
                canParseEnd = true;
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
                canParseEnd = true;
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
        case TYPE_LVINCULUM:
            return SYN_ERROR;
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
            BVSInsert(mainTree->rootPtr, *sToken);
            printf("%s GF@&%s\n", DEFVAR, (sToken->content.str->str)+1);
            tmpToken = sToken;
            if((result = getNextToken(sToken)) != SUCCES){
                return result;
            }
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

            if((result = getNextToken(sToken)) != SUCCES){
                return result;
            }
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
        case TYPE_NOT_EQUAL:
        case TYPE_CONCATENATE:
            if((result = getNextToken(sToken)) != SUCCES){
                return result;
            }
            result = statlist(sToken, fun_id);
            if(result != SUCCES){
                return result;
            }
            return SUCCES;

        case TYPE_STRING:
        case TYPE_INTEGER_NUMBER:
        case TYPE_DOUBLE_NUMBER:
        case TYPE_EXPONENT_NUMBER:
        break;
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
                    BVSInsert(mainTree->rootPtr, *sToken);
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
                            printf("%s $else%d\n", JUMPIFNEQ, condCounter);
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
                            printf("%s $else%d\n", JUMPIFNEQ, condCounter);
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
                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }
                    if(sToken->type == TYPE_RBRACKET){
                        return SUCCES;
                    }else if(sToken->type == TYPE_EQUAL || sToken->type == TYPE_NOT_EQUAL){
                        return parametrs(PARAM_IF_WHILE, repeat, sToken, fun_id);
                    }
                    return SYN_ERROR;
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
                            printf("%s $else%d\n", JUMPIFNEQ, condCounter);
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
                        if(!in_function){
                            DLL_PrintGlobal(list);
                        }
                        else{
                            DLL_PrintLocal(list);
                        }
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
                        if(!in_function)
                        {
                            DLL_PrintGlobal(list);
                        }
                        else{
                            DLL_PrintLocal(list);
                        }
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
            if(in_function){
            printf("%s LF@$%s %s\n", READ,tmpToken->content.str->str,INT_TYPE);
                //BVSSearch(insideFunction, *tmpToken)->content.integerNumber = INT_TYPE;
                
            }
            else{
            printf("%s GF@$%s %s\n", READ,tmpToken->content.str->str,INT_TYPE);
            }
            if((result = getNextToken(sToken)) != SUCCES){
                return  result;
            }
            if(sToken->type == TYPE_RBRACKET){
                return SUCCES;
            }
            else{
                return SYN_ERROR;
            }
        case PARAM_READS: // reads
         if(in_function){
            printf("%s LF@$%s %s\n", READ,tmpToken->content.str->str,STRING_TYPE);
                //BVSSearch(insideFunction, *tmpToken)->content.integerNumber = INT_TYPE;
                
            }
            else{
            printf("%s GF@$%s %s\n", READ,tmpToken->content.str->str,STRING_TYPE);
            }
            if((result = getNextToken(sToken)) != SUCCES){
                return  result;
            }
            if(sToken->type == TYPE_RBRACKET){
                return SUCCES;
            }
            else{
                return SYN_ERROR;
            }
        case PARAM_READF: // readf
            if(in_function){
            printf("%s LF@$%s %s\n", READ,tmpToken->content.str->str,FLOAT_TYPE);
                //BVSSearch(insideFunction, *tmpToken)->content.integerNumber = INT_TYPE; 
            }
            else{
            printf("%s GF@$%s %s\n", READ,tmpToken->content.str->str,FLOAT_TYPE);
            }

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
        printf("%s LF@&%s GF@&%s\n", MOVE, (sToken->content.str->str)+1, sToken->content.str->str);
         if(in_function){
            
            printf("%s LF@&%s\n", STRLEN,(tmpToken->content.str->str)+1);
                //BVSSearch(insideFunction, *tmpToken)->content.integerNumber = INT_TYPE; 
            }
            else{
            printf("%s GF@&%s\n", STRLEN,(tmpToken->content.str->str)+1);
            }
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
                        unique++;
                        printf("%s LF@&%s GF@&%s\n",MOVE, (sToken->content.str->str)+1, (sToken->content.str->str)+1);
                        printf("%s LF@&substring%d\n", LABEL, unique);
                        printf("%s LF@&tmp%d\n", DEFVAR, unique);
                        printf("%s LF@&tmp%d %s ", SUBSTRING, unique, (sToken->content.str->str)+1);
                        
                        tmp2Token = sToken;
                        tmp2Token->content = sToken->content;
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
                                
                                printf("%s\n",sToken->content.str->str);
                                printf("%s LF@&%s int@%d\n", ADD, tmp2Token->content.str->str, 1);
                                
                            
                                printf("%s LF@&%s LF@&%s LF@&tmp%d\n", CONCAT, (tmpToken->content.str->str)+1, (tmpToken->content.str->str)+1, unique);
                                printf("%s LF@&substring%d LF@&%s GF@&%s\n",JUMPIFNEQ, unique, (tmpToken->content.str->str)+1, (sToken->content.str->str)+1);
                                printf("%s GF@&%s LF@&%s\n", MOVE, (sToken->content.str->str)+1, (sToken->content.str->str)+1);
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
                printf("%s\n", CREATEFRAME);
                printf("%s\n", PUSHFRAME);
                printf("%s LF@&%s GF@&%s\n", MOVE, (sToken->content.str->str)+1, (sToken->content.str->str)+1);
                printf("%s GF@&%s LF@&%s int@%d\n",STRI2INT, (tmpToken->content.str->str)+1, (sToken->content.str->str)+1, 0);
                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if(sToken->type == TYPE_RBRACKET){
                    return SUCCES;
                }
            }
            return SYN_ERROR;

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
                unique++;
                printf("%s\n", CREATEFRAME);
                printf("%s\n", PUSHFRAME);
                printf("%s LF@&%s GF@&%s\n", MOVE, (sToken->content.str->str)+1, (sToken->content.str->str)+1);
                printf("%s LF@&tmp%d\n", DEFVAR, unique);
                printf("%s LF@&tmp%d LF@&%s int@%d\n", LT,unique, sToken->content.str->str, 0);
                printf("%s LF@&chr%d bool@true LF@&tmp%d\n",JUMPIFEQ, unique, unique);
                printf("%s LF@&tmp%d LF@&%s int@%d\n", GT,unique, sToken->content.str->str, 255);
                printf("%s LF@&chr%d bool@true LF@&tmp%d\n",JUMPIFEQ, unique, unique);
                printf("%s %s LF@&%s\n", INT2CHAR,(tmpToken->content.str->str)+1, (sToken->content.str->str)+1);
                printf("%s LF@&chr%d\n", LABEL, unique);
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
    string fun_init;

    initStr.str = NULL;
    initStr.length = 0;
    initStr.alloc = 0;
    fun_init.alloc = 0;
    fun_init.length = 0;
    fun_init.str = NULL;
    init.type = 110;
    init.content.str = &initStr;
    initek.content = &fun_init;
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
    fprintf(stderr, "--%d--\n", result);
    BVSFree(mainTree);
    BVSFree(insideFunction);
    BVSFree_function(functionNames);
    //todo freeToken() funtcion
    return result;
}
