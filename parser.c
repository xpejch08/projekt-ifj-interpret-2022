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

string* activeString;

DLList _list;
DLList *list = &_list;


bool in_function = false;
bool canParseEnd = false;
bool returnCount = false;
bool afterAssign = false;

TNodef *call_function_save; //////////////////////////////// new

int tokenId;
//function_save *fun_id;

int uniqueIf = 0;
int uniqueWhile = 0;
int condCounter = 0;
int unique = 0;
int whileCounter = 0;
int incId = 1;

//test
int checkIfBuiltIn(token *sToken){
    if (strCmpConstStr(sToken->content.str, "write") == 0){
        return 1;
    }
    if (strCmpConstStr(sToken->content.str, "reads") == 0){
        return 1;
    }
    if (strCmpConstStr(sToken->content.str, "readi") == 0){
        return 1;
    }
    if (strCmpConstStr(sToken->content.str, "readf") == 0){
        return 1;
    }
    if (strCmpConstStr(sToken->content.str, "strlen") == 0){
        return 1;
    }
    if (strCmpConstStr(sToken->content.str, "substring") == 0){
        return 1;
    }
    if (strCmpConstStr(sToken->content.str, "ord") == 0){
        return 1;
    }
    if (strCmpConstStr(sToken->content.str, "chr") == 0){
        return 1;
    }
    return 0;
}
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
                    printf("%s ", WRITE);
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

                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if (sToken->type == TYPE_LBRACKET) {
                    paramError = parametrs(PARAM_READF, 1, sToken, fun_id);
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
            if (strCmpConstStr(sToken->content.str, "strlen") == 0) {
                canParseEnd = false;

                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if (sToken->type == TYPE_LBRACKET) {
                    paramError = parametrs(PARAM_STRLEN, 1, sToken, fun_id);
                    if (paramError == SUCCES) {
                        if((result = getNextToken(sToken)) != SUCCES){
                            return  result;
                        }
                        if (sToken->type != TYPE_SEMICOLON) {
                            return SYN_ERROR;
                        }
                        canParseEnd = true;
                        
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
                        if((result = getNextToken(sToken)) != SUCCES){
                            return  result;
                        }
                        if (sToken->type != TYPE_SEMICOLON) {
                            return SYN_ERROR;
                        }
                        canParseEnd = true;

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
                printf("%s &%s\n", CALL, sToken->content.str->str);

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
            printf("%s &%send\n", JUMP, fun_id->content->str);
            printf("%s &%s\n", LABEL, fun_id->content->str);
            printf("%s\n", CREATEFRAME);
            printf("%s\n", PUSHFRAME);
            printf("%s LF@&return_val\n", DEFVAR);
            printf("%s LF@&return_val %s\n", MOVE, NIL);
            printf("%s\n", POPFRAME);
            printf("%s\n", RETURN);

            if((result = getNextToken(sToken)) != SUCCES) {
                return result;
            }
            if (sToken->type != TYPE_LBRACKET) {
                return SYN_ERROR;
            }

            else{
                in_function = true;
                paramError = parametrs(PARAM_FUNCTION, 1, sToken, fun_id);
                functionNames->rootPtr = BVSInsert_function(functionNames->rootPtr, *fun_id);
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

                if ((result = getNextToken(sToken)) != SUCCES) {
                    return result;
                }

                result = statlist(sToken, fun_id);
                if(result != SUCCES){
                    return result;
                }
                if(sToken->type != TYPE_RVINCULUM){
                    return SYN_ERROR;
                }
                if(returnCount != true){
                    return SEM_COUNT_ERROR;
                }
                canParseEnd = true;
                returnCount = false;
                in_function = false;
                printf("%s &%send\n", LABEL, fun_id->content->str);
                BVSDispose(insideFunction);
                if((result = getNextToken(sToken)) != SUCCES){
                    return result;
                }

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
            }
            if(in_function == true){
                returnCount = true;
                canParseEnd = true;
                result = statlist(sToken, fun_id);
                if(result != SUCCES){
                    return result;
                }
                return SUCCES;
            } else{
                printf("%s %d\n", EXIT, 0);
                return SUCCES;            
            }


        case KEYWORD_WHILE:
            canParseEnd = false;
            uniqueWhile++;
            whileCounter = uniqueWhile;
            printf("%s &while%d\n", LABEL, whileCounter);
            if((result = getNextToken(sToken)) != SUCCES) {
                return result;
            }
            if (sToken->type != TYPE_LBRACKET) {
                return SYN_ERROR;
            }
            else {
                paramError = parametrs(PARAM_IF_WHILE, 1, sToken, fun_id); // misto parametrs volat precedencku
                printf("%s &while_end%d\n", JUMPIFNEQ, whileCounter);
                if(paramError != SUCCES){
                    return paramError;
                }
                if((result = getNextToken(sToken)) != SUCCES) {
                    return result;
                }
                if(sToken->type != TYPE_LVINCULUM) {
                    return SYN_ERROR;
                }
                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if(sToken->type == TYPE_RVINCULUM) {
                    canParseEnd = true;
                    printf("%s &while%d\n", JUMP, whileCounter);
                    printf("%s &while_end%d\n", LABEL, whileCounter);
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
                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }
                                 
                    result = statlist(sToken, fun_id);
                    printf("%s &while%d\n", JUMP, whileCounter);
                    printf("%s &while_end%d\n", LABEL, whileCounter);                   
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
            if((result = getNextToken(sToken)) != SUCCES){
                return  result;
            }
            if (sToken->type != TYPE_VARIABLE) {
                return SYN_ERROR;
            } else {
                return SUCCES;
            }
        case KEYWORD_IF: // misto parametrs volat precedencku
            uniqueIf+= 1;
            condCounter = uniqueIf;
            canParseEnd = true;
            if((result = getNextToken(sToken)) != SUCCES){
                return  result;
            }
            if (sToken->type != TYPE_LBRACKET) {
                return SYN_ERROR;
            } else {
                paramError = parametrs(PARAM_IF_WHILE,1, sToken, fun_id);
                printf("%s &else%d\n", JUMPIFNEQ, condCounter);
                if(paramError != SUCCES) {
                    return paramError;
                }
                else {

                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }
                    if(sToken->type != TYPE_LVINCULUM) {
                        return SYN_ERROR;
                    }
                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
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
                        printf("%s &else_end%d\n", JUMP, condCounter);
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
            printf("%s &else%d\n", LABEL, condCounter);

            if((result = getNextToken(sToken)) != SUCCES){
                return  result;
            }
            if(sToken->type != TYPE_LVINCULUM) {
                return SYN_ERROR;
            }
            if((result = getNextToken(sToken)) != SUCCES){
                return  result;
            }
            if(sToken->type == TYPE_RVINCULUM) {
                printf("%s &else_end%d\n", LABEL, condCounter);
                
                canParseEnd = true;
                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
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
                printf("%s &else_end%d\n", LABEL, condCounter);
                condCounter--;
                canParseEnd = true;
                result = statlist(sToken, fun_id);
            
                //getNextToken ->statlist?
                printf("%s &else_end%d\n", LABEL, condCounter);
                condCounter--;
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
            return SYN_ERROR;


        case TYPE_RVINCULUM:
            return SUCCES;

        case TYPE_VARIABLE:
            if (afterAssign == false) {
                if(!in_function){
                if (BVSSearch(mainTree->rootPtr, *sToken) == NULL) {
                    printf("%s GF@&%s\n", DEFVAR, (sToken->content.str->str) + 1);
                    mainTree->rootPtr = BVSInsert(mainTree->rootPtr, *sToken);
                }
                }
                else
                {
                    if (BVSSearch(insideFunction->rootPtr, *sToken) == NULL) {
                    insideFunction->rootPtr = BVSInsert(insideFunction->rootPtr, *sToken);
                    printf("%s LF@&%s\n", DEFVAR, (sToken->content.str->str) + 1);
                    }
                }
                strClean(activeString);
                strCpyStr(activeString, sToken->content.str);
                
                if ((result = getNextToken(sToken)) != SUCCES) {
                    return result;
                }
                if (sToken->type == TYPE_SEMICOLON) {
                    canParseEnd = true;
                }
                result = statlist(sToken, fun_id);
                if (result != SUCCES) {
                    return result;
                }
                return SUCCES;
            } else {               
                //todo precedencni
                afterAssign = false;
            }

        case TYPE_END_OF_FILE:
            if (canParseEnd == true) {
                return SUCCES;
            } else {
                return SYN_ERROR;
            }
        case TYPE_IDENTIFIER:
        
            result = declrList(sToken, fun_id);
            if(result != SUCCES){
                return result;
            }
            return SUCCES;

        case TYPE_RBRACKET:
            return SUCCES;

        case TYPE_SEMICOLON:
            if((result = getNextToken(sToken)) != SUCCES){
                return result;
            }
            result = statlist(sToken, fun_id);
            if(result != SUCCES){
                return result;
            }
            return SUCCES;
        case TYPE_ASSIGN:

            if((result = getNextToken(sToken)) != SUCCES){
                return result;
            }
            if (sToken->type != TYPE_VARIABLE) {
                if (sToken->type != TYPE_EXPONENT_NUMBER) {
                    if (sToken->type != TYPE_DOUBLE_NUMBER) {
                        if (sToken->type != TYPE_INTEGER_NUMBER) {
                            afterAssign = false;
                        }
                    }
                }
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
            if ((result = getNextToken(sToken)) != SUCCES) {
                return result;
            }
            if (sToken->type != TYPE_IDENTIFIER) {
                return SYN_ERROR;
            } else if ((checkIfBuiltIn(sToken)) != 0) {
                return 3;
            }
            sToken->type = TYPE_FUNCTIONDECLARE;
            result = declrList(sToken, fun_id);
            if (result != SUCCES) {
                return result;
            }
            return SUCCES;

        case KEYWORD_RETURN:
            if (in_function == true) {
                result = declrList(sToken, fun_id);
                if (result != SUCCES) {
                    return result;
                }
                return SUCCES;
            }
            if (canParseEnd == true) {
                result = parametrs(PARAM_RETURN, 1, sToken, fun_id);
                if (result != SUCCES) {
                    return result;
                }
                return SUCCES;
            }
            return SYN_ERROR;

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
            canParseEnd = false;
            if ((result = getNextToken(sToken)) != SUCCES) {
                return result;
            }
            result = statlist(sToken, fun_id);
            if (result != SUCCES) {
                return result;
            }
            return SUCCES;

        case TYPE_EPILOG:
            return SUCCES;

        case TYPE_STRING:
        case TYPE_INTEGER_NUMBER:
        case TYPE_DOUBLE_NUMBER:
        case TYPE_EXPONENT_NUMBER:
            if (afterAssign == false) {
                if ((result = getNextToken(sToken)) != SUCCES) {
                    return result;
                }
                result = statlist(sToken, fun_id);
                if (result != SUCCES) {
                    return result;
                }
                return SUCCES;
            }
            else{
                //todo precedencni analyza
            }
    }
    return SYN_ERROR;
    //todo statList
}


//stat function checks content of different types, for example condition of while or body of while, calls function
//statlist -> statlist calls stat recursively

//todo
int parametrs(int option, int repeat, token *sToken, function_save *fun_id){
    string* tmpToken;
    string initToken;
    initToken.str = NULL;
    initToken.alloc = 0;
    initToken.length = 0;
    tmpToken = &initToken;

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
                    if(!in_function)
                    {
                        printf("GF@&%s ", (sToken->content.str->str)+1);
                    }
                    else{
                        printf("LF@&%s ", (sToken->content.str->str)+1);
                    }

                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }
                    if(sToken->type == TYPE_RBRACKET){

                        printf("\n");

                        return SUCCES;
                    }
                    else if(sToken->type == TYPE_COMMA){
                        repeat++;
                        return parametrs(PARAM_WRITE, repeat, sToken, fun_id);
                    }
                    else{
                        return SYN_ERROR;
                    }
                case TYPE_EXPONENT_NUMBER:
                case TYPE_STRING:
                case TYPE_INTEGER_NUMBER:
                case TYPE_DOUBLE_NUMBER:
                    printf("%s ", sToken->content.str->str);
                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }
                    if(sToken->type == TYPE_RBRACKET){

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
                printf("%s LF@&%s %s\n", READ,(activeString->str)+1,INT_TYPE);

            }
            else{
                printf("%s GF@&%s %s\n", READ,(activeString->str)+1,INT_TYPE);
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
                printf("%s LF@&%s %s\n", READ,(activeString->str)+1,STRING_TYPE);

            }
            else{
                printf("%s GF@&%s %s\n", READ,(activeString->str)+1,STRING_TYPE);
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
                printf("%s LF@&%s %s\n", READ,(activeString->str)+1,FLOAT_TYPE);
            }
            else{
                printf("%s GF@&%s %s\n", READ,(activeString->str)+1,FLOAT_TYPE);
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
           
            if((result = getNextToken(sToken)) != SUCCES){
                return  result;
            }
            if(sToken->type == TYPE_VARIABLE || sToken->type == TYPE_STRING){
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
                }
            if(in_function){

                printf("%s LF@&%s %s\n", STRLEN,(activeString->str)+1, sToken->content.str->str);
            }
            else{
                printf("%s GF@&%s %s\n", STRLEN,(activeString->str)+1, sToken->content.str->str);
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

            printf("%s\n", CREATEFRAME);
            printf("%s\n", PUSHFRAME);
            printf("%s LF@&cmp\n", DEFVAR);
            printf("%s LF@&cmp bool@false\n", MOVE);
            printf("%s LF@&cmp2\n", DEFVAR);
            printf("%s LF@&cmp2 bool@false\n", MOVE);
            if((result = getNextToken(sToken)) != SUCCES){
                return  result;
            }
            unique++;
            int tmpCounter = unique;
            bool varI = false;
            bool varJ = false;
            
            if(sToken->type == TYPE_VARIABLE || sToken->type == TYPE_STRING){
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
                    printf("%s LF@&tmp%d GF@&%s\n",MOVE, unique, (sToken->content.str->str)+1);
                }
                else{
                    printf("%s LF@&tmp%d\n", DEFVAR, unique);
                    printf("%s LF@&tmp%d string@%s\n",MOVE, unique, sToken->content.str->str);   
                }
                
                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if(sToken->type == TYPE_COMMA){
                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }
                    
                    strCpyStr(tmpToken,sToken->content.str);
                    if(sToken->type == TYPE_VARIABLE || sToken->type == TYPE_INTEGER_NUMBER){
                        if(sToken->type == TYPE_VARIABLE){
                            varI = true;
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
                           
                        printf("%s LF@&%s GF@&%s\n",MOVE, (sToken->content.str->str)+1, (sToken->content.str->str)+1);
                        printf("%s LF@&cmp LF@&%s int@%d\n", LT, (sToken->content.str->str)+1, 0);
                        printf("%s LF@&error LF@&cmp bool@true\n", JUMPIFEQ); 
                        
                        }
                        
                        else{
                             tmpCounter++;
                        
                        printf("%s LF@tmp%d\n", DEFVAR, tmpCounter);
                        printf("%s LF@&res%d\n", DEFVAR, unique);
                        printf("%s LF@&tmp%d int@%s\n",MOVE,tmpCounter, sToken->content.str->str);
                        printf("%s LF@&cmp LF@&tmp%d int@%d\n", LT, tmpCounter, 0);
                        printf("%s LF@&error LF@&cmp bool@true\n", JUMPIFEQ);
                        }
                        
                        if((result = getNextToken(sToken)) != SUCCES){
                            return  result;
                        }

                        if(sToken->type == TYPE_COMMA){
                            if((result = getNextToken(sToken)) != SUCCES){
                                return  result;
                            }
                            
                            if(sToken->type == TYPE_VARIABLE){
                                varJ = true;
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
                                printf("%s LF@&%s GF@&%s\n", MOVE, (sToken->content.str->str)+1,(sToken->content.str->str)+1);
                                printf("%s LF@&cmp LF@&tmp%d LF@&%s\n", GT, tmpCounter,(sToken->content.str->str)+1);
                                printf("%s LF@&error LF@&cmp bool@true\n", JUMPIFEQ);
                                printf("%s LF@&cmp LF@&%s int@%d\n", LT, (sToken->content.str->str)+1, 0);
                                printf("%s LF@&error LF@&cmp bool@true\n", JUMPIFEQ);
                                printf("%s LF@&%s\n", DEFVAR, (activeString->str)+1);
                                printf("%s LF@&%s string@" "\n",MOVE, (activeString->str)+1);
                                printf("%s LF@&substring%d\n", LABEL, unique);
                                printf("%s LF@&tmp%d LF@&tmp%d LF@&%s\n", SUBSTRING, unique,tmpCounter, (sToken->content.str->str)+1);
                                printf("%s LF@&%s LF@&%s LF@&tmp%d\n", CONCAT, (activeString->str)+1, (activeString->str)+1, unique);

                                printf("%s LF@&cmp2 LF@&tmp%d LF@&%s\n", EQ, tmpCounter, (sToken->content.str->str)+1);
                                printf("%s LF@&tmp%d int@%d\n", ADD, tmpCounter, 1);
                                printf("%s LF@&substring%d LF@&cmp2 bool@true\n",JUMPIFNEQ, unique);

                                
                                printf("%s GF@&%s LF@&%s\n", MOVE, (activeString->str)+1, (activeString->str)+1);
                                printf("%s LF@&legit\n", JUMP);
                                
                            }
                            else{
                    
                                if(varI == false && varJ == false){
                                printf("%s LF@&tmp%d\n", DEFVAR, (tmpCounter)+1);
                                printf("%s LF@&tmp%d int@%s\n", MOVE,(tmpCounter)+1, sToken->content.str->str);
                                printf("%s LF@&cmp LF@&tmp%d LF@&tmp%d\n", GT, tmpCounter,(tmpCounter)+1);
                                printf("%s LF@&error LF@&cmp bool@true\n", JUMPIFEQ);
                                printf("%s LF@&cmp LF@&tmp%d int@%d\n", LT, (tmpCounter)+1, 0);
                                printf("%s LF@&error LF@&cmp bool@true\n", JUMPIFEQ);

                                printf("%s LF@&%s\n", DEFVAR, (activeString->str)+1);
                                printf("%s LF@&%s string@\n",MOVE, (activeString->str)+1);

                                printf("%s LF@&substring%d\n", LABEL, unique);
                                printf("%s LF@&res%d LF@&tmp%d LF@&tmp%d\n", SUBSTRING, unique,unique, tmpCounter);
                                
                                printf("%s LF@&%s LF@&%s LF@&res%d\n", CONCAT, (activeString->str)+1,(activeString->str)+1, unique);
                                printf("%s LF@&cmp2 LF@&tmp%d LF@&tmp%d\n", EQ, tmpCounter, (tmpCounter)+1);
                                printf("%s LF@&tmp%d int@%d\n", ADD, tmpCounter, 1);
                                printf("%s LF@&substring%d LF@&cmp2 bool@true",JUMPIFNEQ, unique);
                                printf("%s GF@&%s LF@&%s\n", MOVE, (activeString->str)+1, (activeString->str)+1);
                                printf("%s LF@&legit\n", JUMP);
                                }
                                if(varI == true)
                                {
                                printf("%s LF@&tmp%d\n", DEFVAR, (tmpCounter)+1);
                                printf("%s LF@&tmp%d int@%s\n", MOVE,(tmpCounter)+1, sToken->content.str->str);
                                
                                printf("%s LF@&cmp LF@&%s LF@&tmp%d\n", GT, (tmpToken->str)+1,(tmpCounter)+1);
                                printf("%s LF@&error LF@&cmp bool@true\n", JUMPIFEQ);
                                printf("%s LF@&cmp LF@&tmp%d int@%d\n", LT, (tmpCounter)+1, 0);
                                printf("%s LF@&error LF@&cmp bool@true\n", JUMPIFEQ);
                                
                                printf("%s LF@&%s\n", DEFVAR, (activeString->str)+1);
                                printf("%s LF@&%s string@\n",MOVE, (activeString->str)+1);

                                printf("%s LF@&substring%d\n", LABEL, unique);
                                printf("%s LF@&res%d LF@&%s LF@&tmp%d\n", SUBSTRING, unique,(tmpToken->str)+1, tmpCounter);
                                
                                printf("%s LF@&%s LF@&%s LF@&res%d\n", CONCAT, (activeString->str)+1,(activeString->str)+1, unique);
                                printf("%s LF@&cmp2 LF@&%s LF@&tmp%d\n", EQ, (tmpToken->str)+1, (tmpCounter)+1);
                                printf("%s LF@&%s int@%d\n", ADD, (tmpToken->str)+1, 1);
                                printf("%s LF@&substring%d LF@&cmp2 bool@true",JUMPIFNEQ, unique);
                                
                                printf("%s GF@&%s LF@&%s\n", MOVE, (activeString->str)+1, (activeString->str)+1);
                                printf("%s LF@&legit\n", JUMP);
                                } 
                                
                              } 
                                strClean(tmpToken);
                                if((result = getNextToken(sToken)) != SUCCES){
                                    return  result;
                                }
                                if(sToken->type == TYPE_RBRACKET){
                                    printf("%s LF@&error\n", LABEL);
                                    printf("%s GF@&%s nil@nil\n", MOVE, (activeString->str)+1);
                                    printf("%s LF@&legit\n", LABEL);
                                    printf("%s\n", POPFRAME);
                                    return SUCCES;
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
            if(sToken->type == TYPE_VARIABLE || sToken->type == TYPE_STRING){
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
                }
                printf("%s\n", CREATEFRAME);
                printf("%s\n", PUSHFRAME);
                printf("%s LF@&%s GF@&%s\n", MOVE, (sToken->content.str->str)+1, (sToken->content.str->str)+1);
                printf("%s GF@&%s LF@&%s int@%d\n",STRI2INT, (activeString->str)+1, (sToken->content.str->str)+1, 0);
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
            if(sToken->type == TYPE_VARIABLE || sToken->type == TYPE_INTEGER_NUMBER){
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
                printf("%s %s LF@&%s\n", INT2CHAR,(activeString->str)+1, (sToken->content.str->str)+1);
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
            /////////////////////// IN MAIN PROGRAM ///////////////////////////////////////
            if(!insideFunction){ 
                switch(sToken->type){
                    case KEYWORD_NULL:
                    case TYPE_STRING:
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
                    case TYPE_EXPONENT_NUMBER:
                    case TYPE_INTEGER_NUMBER:
                    case TYPE_DOUBLE_NUMBER:
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
                                return SUCCES;
                            }
                        return SYN_ERROR;
                }    
                    return SYN_ERROR;
            }
            ///////////////// IN FUNCTION /////////////////////////////
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
                case KEYWORD_NULL:
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
                case TYPE_EXPONENT_NUMBER:
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
                        return SEM_RETURN_ERROR;
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
                    return SYN_ERROR;
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
                        if(call_function_save->return_type == KEYWORD_VOID)
                        {
                            printf("%s GF@&%s nil@nil\n", MOVE, (activeString->str)+1);
                        }
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
                    }else{
                        return SYN_ERROR;
                    }
                case TYPE_EXPONENT_NUMBER:
                case TYPE_INTEGER_NUMBER:
                case TYPE_DOUBLE_NUMBER:
                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }
                    if(sToken->type == TYPE_RBRACKET){
                        if(call_function_save->return_type == KEYWORD_VOID)
                        {
                            printf("%s GF@&%s nil@nil\n", MOVE, (activeString->str)+1);
                        }
                        if(repeat == call_function_save->parameters){
                            return SUCCES;
                        }
                        return SEM_COUNT_ERROR;
                    }else if(sToken->type == TYPE_COMMA){
                        repeat++;
                        return parametrs(PARAM_FUNCTION_CALL, repeat, sToken, fun_id);
                    }else if(sToken->type == TYPE_ADDITION    ||
                             sToken->type == TYPE_SUBTRACTION ||
                             sToken->type == TYPE_DIVIDE      ||
                             sToken->type == TYPE_MULTIPLY){
                        return parametrs(PARAM_FUNCTION_CALL, repeat, sToken, fun_id);
                    }else{
                        return SYN_ERROR;
                    }
                case TYPE_STRING:
                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }
                    if(sToken->type == TYPE_RBRACKET){
                        if(call_function_save->return_type == KEYWORD_VOID)
                        {
                            printf("%s GF@&%s nil@nil\n", MOVE, (activeString->str)+1);
                        }
                        if(repeat == call_function_save->parameters){
                            return SUCCES;
                        }
                        return SEM_COUNT_ERROR;
                    }else if(sToken->type == TYPE_COMMA){
                        repeat++;
                        return parametrs(PARAM_FUNCTION_CALL, repeat, sToken, fun_id);
                    }else if(sToken->type == TYPE_CONCATENATE){
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
    string activeStringInit;
    activeStringInit.str = NULL;
    activeStringInit.length = 0;
    activeStringInit.str = 0;


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

    activeString = &activeStringInit;
    TRoot initMain;
    TRoot initInside;
    TRootf initNames;

    strInit(activeString);
    strClean(activeString);

    mainTree = &initMain;
    functionNames = &initNames;
    insideFunction = &initInside;
    //initializing tree
    BVSInit(insideFunction);
    BVSInit(mainTree);
    BVSInit_function(functionNames);

    int result;

    //todo fix init token function

    if((tokenId = getNextToken(sToken)) == LEX_ERROR){
        return LEX_ERROR;
    }
    else{
        if(sToken->type == TYPE_SEMICOLON){
            return SYN_ERROR;
        }
        result = statlist(sToken, fun_id);
        //printf("EXIT %d", result);
    }
    fprintf(stderr, "--%d--\n", result);
    //BVSFree(mainTree);
    //BVSFree(insideFunction);
    //BVSFree_function(functionNames);
    //todo freeToken() funtcion
    return result;
}
