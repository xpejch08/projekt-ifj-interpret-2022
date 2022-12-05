#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "lexical.h"
#include "str.h"
#include "parser.h"
#include "symtable.h"
#include "expstack.h"
#include "expression.h"


//global variables for trees that store variable names and function names
//tree used for variables inside a function
TRoot *insideFunction;

//tree that stores all function names
TRootf *functionNames;

//tree that stores all variable names that aren't inside a function (global variables)
TRoot *mainTree;


//global variable used for storing the value of the active variable, needed because of precedence analysis
string* activeString;

//stack used in precedence analysis
Stack *stack;

DLList _list;
DLList *list = &_list;

//bool global variable used for checking if we are inside a function or not
bool in_function = false;

//bool global variable used for checking if function parse can end after EOF or if it is a syntax error
bool canParseEnd = false;

//bool global variable used for checking if a function has a return or not
bool returnCount = false;

//bool global variable used for checking if the next token arrives after an assign token or not
//if true calls precedenceAction -> main function of precedence analysis
bool afterAssign = false;

TNodef *call_function_save; 


int uniqueIf = 0;
int uniqueWhile = 0;
int condCounter = 0;
int unique = 0;
int whileCounter = 0;
int incId = 1;

/**
 * @brief function that checks if the content of a token is a built in function or not
 * @param sToken active token passed around through all functions that need to work with it
 * @return returns 1 if content is an inbuilt function else returns 0
 */
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


int declrList(token *sToken, function_save *fun_id) {

    //variable used for checking if function parametrs returns and error or success
    int paramError;

    //variable used for storing the value of next recursive call of statlist or declrlist
    int result;
   
    switch (sToken->type) {
        //if token is rvinculum we always return succes and jump out of current declrlist
        case TYPE_RVINCULUM:
            return SUCCES;

            /**
             * case that checks all identifiers
             * identifier can either be a built in function, function declaration or a function call
             * all built in fucntions call prametrs, if successfully call next token and call function statlist
             * all built in function cases are the same but they call parametrs with different option
             */
        case TYPE_IDENTIFIER:

            if (strCmpConstStr(sToken->content.str, "write") == 0) {

                //setting canParseEnd to false -> if we get EOF we return SYNTAX ERROR
                canParseEnd = false;


                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if (sToken->type == TYPE_LBRACKET) {
                    printf("%s ", WRITE);
                    //calling function parametrs with PARAM_WRITE
                    paramError = parametrs(PARAM_WRITE, 1,  sToken, fun_id);
                    if (paramError == SUCCES) {
                        if(getNextToken(sToken) == LEX_ERROR){
                            return  LEX_ERROR;
                        }
                        //next token has to be semicolon else SYNTAX ERROR
                        if (sToken->type != TYPE_SEMICOLON) {
                            return SYN_ERROR;
                        }

                        //setting canParseEnd to false -> if we get EOF we return SUCCESS
                        canParseEnd = true;

                        //calling next token after correct handle of inbuilt function
                        if((result = getNextToken(sToken)) != SUCCES){
                            return  result;
                        }
                        //calling function statlist one layer deeper
                        result = statlist(sToken, fun_id);
                        if(result != SUCCES) {
                            return result;
                        }
                        //if result is SUCCESS we can return here
                        return SUCCES;

                    }
                    //if function parametrs didn't return succes we return its error from errors.h
                    else {
                        return paramError;
                    }
                }
                // if type isn't left bracket returns syntax error
                else {
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
            if (strCmpConstStr(sToken->content.str, "chr") == 0) {
                canParseEnd = false;

                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if (sToken->type == TYPE_LBRACKET) {
                    paramError = parametrs(PARAM_CHR, 1, sToken, fun_id);
                    if (paramError == SUCCES) {
                        if ((result = getNextToken(sToken)) != SUCCES) {
                            return result;
                        }

                        if(sToken->type != TYPE_SEMICOLON)
                        {
                            return SYN_ERROR;
                        }
                        //todo instructionFree()
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
            /**
             * if identifier isn't a built in function we know it's a function call
             */

            //checking if a function was already defined otherwise we are
            // calling an undefined function which returns a SEM_UNDEFINED_ERROR

            if (BVSSearch_function(functionNames->rootPtr, *sToken) != NULL) {

                //setting canParseEnd to false, if we get EOF token now we return SYNTAX ERROR
                canParseEnd = false;
                call_function_save = BVSSearch_function(functionNames->rootPtr, *sToken);
                
                //tmpToken for call
                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if(sToken->type != TYPE_LBRACKET){
                    return SYN_ERROR;
                }
                printf("%s\n", CREATEFRAME);
                paramError = parametrs(PARAM_FUNCTION_CALL, 1, sToken, fun_id);
                printf("%s &%s\n", CALL, call_function_save->content->str);

                //if parametrs returns SUCCESS we call next token and jump into another layer of function statlist
                if(paramError == SUCCES){
                    //setting canParseEnd to true, if we get EOF we return SUCCESS
                    canParseEnd = true;
                    if((result = getNextToken(sToken)) != SUCCES){
                        return  result;
                    }

                    result = statlist(sToken, fun_id);
                    if(result != SUCCES){
                        return result;
                    }
                    return SUCCES;
                }

            }
            //we called an undefined function
            else {
                return SEM_DEFINE_ERROR;
            }


            /**
             * case that checks a function declaration
             */
        case TYPE_FUNCTIONDECLARE:
            //if we get EOF returns error
            canParseEnd = false;

            //if we are inside a function we can't declare a function
            if(in_function){
                return SYN_ERROR;
            }

            //if function already exists return SEM_DEFINE_ERROR because of function redeclaration
            if(BVSSearch_function(functionNames->rootPtr, *sToken) != NULL){
                return SEM_DEFINE_ERROR;
            }
            fun_id->content->str = sToken->content.str->str;
            printf("%s &%send\n", JUMP, fun_id->content->str);
            printf("%s &%s\n", LABEL, fun_id->content->str);
            printf("%s\n", PUSHFRAME);
            printf("%s LF@&return_val\n", DEFVAR);
            printf("%s LF@&return_val %s\n", MOVE, NIL);
            

            if((result = getNextToken(sToken)) != SUCCES) {
                return result;
            }
            //next token after function identifier has to be left bracket
            if (sToken->type != TYPE_LBRACKET) {
                return SYN_ERROR;
            }

            else{
                //from now on we are inside a function
                in_function = true;

                //calling function parametrs with option PARAM_FUNCTION to handle function parametrs
                paramError = parametrs(PARAM_FUNCTION, 1, sToken, fun_id);
                functionNames->rootPtr = BVSInsert_function(functionNames->rootPtr, *fun_id);
                if (paramError != SUCCES) {
                    return paramError;
                }
                else{

                    //next token has to be left vinculum
                    if ((result = getNextToken(sToken)) != SUCCES) {
                        return result;
                    }
                    if (sToken->type != TYPE_LVINCULUM) {
                        return SYN_ERROR;
                    }

                }

                //call next token and jump into another layer of statlist
                if ((result = getNextToken(sToken)) != SUCCES) {
                    return result;
                }

                result = statlist(sToken, fun_id);
                if(result != SUCCES){
                    return result;
                }
                //here we jumped out of function declaration next token has to be right vinculum
                if(sToken->type != TYPE_RVINCULUM){
                    return SYN_ERROR;
                }
                if(returnCount != true){
                    return SEM_COUNT_ERROR;
                }
                printf("%s\n", POPFRAME);
                printf("%s\n", RETURN);

                //parse function can end, we are no longer inside a function
                canParseEnd = true;
                returnCount = false;
                in_function = false;
                printf("%s &%send\n", LABEL, fun_id->content->str);

                //freeing inside function tree
                BVSDispose(insideFunction);


                if((result = getNextToken(sToken)) != SUCCES){
                    return result;
                }

                //calling next layer of statlist
                result = statlist(sToken, fun_id);
                if(result != SUCCES){
                    return result;
                }
                return SUCCES;
            }

            /**
             * case return handles parametrs of return
             */
        case KEYWORD_RETURN :
            canParseEnd = false;
            paramError = parametrs(PARAM_RETURN, 1, sToken, fun_id);
            if(paramError != SUCCES){
                return  paramError;
            }

            returnCount = true;
            canParseEnd = true;
            result = statlist(sToken, fun_id);
            if(result != SUCCES){
                return result;
            }
            return SUCCES;



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
                     printf("%s &while%d\n", JUMP, whileCounter);
                    printf("%s &while_end%d\n", LABEL, whileCounter);           
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
                //printf("%s &else_end%d\n", LABEL, condCounter);
                //condCounter--;
                canParseEnd = true;
                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
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
                if(in_function == false) {
                    result = precedenceAction(mainTree, sToken, stack, in_function);
                    printf("%s GF@&%s GF@&expTmp\n", MOVE, (activeString->str)+1);
                    if (result < 113 || result > 117) {
                        return result;
                    }
                    strClean(sToken->content.str);
                    strCpyStr(sToken->content.str, activeString);
                    TNode *active = BVSSearch(mainTree->rootPtr, *sToken);
                    active->type = result;
                    if ((result = getNextToken(sToken)) != SUCCES) {
                        return result;
                    }
                    afterAssign = false;
                    result = statlist(sToken, fun_id);
                    if (result != SUCCES) {
                        return result;
                    }
                    return SUCCES;
                }
                else{
                    result = precedenceAction(insideFunction, sToken, stack, in_function);
                    printf("%s LF@&%s LF@&expTmp\n", MOVE, (activeString->str)+1);
                    if (result < 113 || result > 117) {
                        return result;
                    }
                    strClean(sToken->content.str);
                    strCpyStr(sToken->content.str, activeString);
                    TNode *active = BVSSearch(insideFunction->rootPtr, *sToken);
                    active->type = result;
                    if ((result = getNextToken(sToken)) != SUCCES) {
                        return result;
                    }
                    afterAssign = false;
                    result = statlist(sToken, fun_id);
                    if (result != SUCCES) {
                        return result;
                    }
                    return SUCCES;
                }
            }

        case TYPE_END_OF_FILE:
            if (canParseEnd == true) {
                printf("%s int@%d\n", EXIT, 0);
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
            afterAssign = true;
            if((result = getNextToken(sToken)) != SUCCES){
                return result;
            }
            if (sToken->type != TYPE_VARIABLE) {
                if (sToken->type != TYPE_EXPONENT_NUMBER) {
                    if (sToken->type != TYPE_DOUBLE_NUMBER) {
                        if (sToken->type != TYPE_INTEGER_NUMBER) {
                            if(sToken->type != TYPE_STRING){
                                if(sToken->type != TYPE_LBRACKET) { // $a = (a);
                                    afterAssign = false;
                                }
                            }
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
                printf("%s int@%d\n", EXIT, 0);
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
            if ((result = getNextToken(sToken)) != SUCCES) {
                return result;
            }
            if(sToken->type == EOF){
                return SUCCES;
            }
            else {
                return SYN_ERROR;
            }
        case TYPE_LBRACKET:
            if (afterAssign == false) {

                return SYN_ERROR;
            }
            else {
                if(in_function == false) {
                    result = precedenceAction(mainTree, sToken, stack, in_function);
                    printf("%s GF@&%s GF@&expTmp\n", MOVE, (activeString->str)+1);
                    result = precedenceAction(mainTree, sToken, stack, in_function, 1);
                    if (result < 113 || result > 117) {
                        return result;
                    }
                    strClean(sToken->content.str);
                    strCpyStr(sToken->content.str, activeString);
                    TNode *active = BVSSearch(mainTree->rootPtr, *sToken);
                    active->type = result;
                    if ((result = getNextToken(sToken)) != SUCCES) {
                        return result;
                    }
                    afterAssign = false;
                    result = statlist(sToken, fun_id);
                    if (result != SUCCES) {
                        return result;
                    }
                    return SUCCES;
                }
                else{
                    result = precedenceAction(insideFunction, sToken, stack, in_function,1);
                    if (result < 113 || result > 117) {
                        return result;
                    }
                    strClean(sToken->content.str);
                    strCpyStr(sToken->content.str, activeString);
                    TNode *active = BVSSearch(insideFunction->rootPtr, *sToken);
                    active->type = result;
                    if ((result = getNextToken(sToken)) != SUCCES) {
                        return result;
                    }
                    afterAssign = false;
                    result = statlist(sToken, fun_id);
                    if (result != SUCCES) {
                        return result;
                    }
                    return SUCCES;
                }
            }
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
            else {
                if(in_function == false) {
                    result = precedenceAction(mainTree, sToken, stack, in_function, 1);
                    if (result < 113 || result > 117) {
                        return result;
                    }
                    strClean(sToken->content.str);
                    strCpyStr(sToken->content.str, activeString);
                    TNode *active = BVSSearch(mainTree->rootPtr, *sToken);
                    active->type = result;
                    if ((result = getNextToken(sToken)) != SUCCES) {
                        return result;
                    }
                    afterAssign = false;
                    result = statlist(sToken, fun_id);
                    if (result != SUCCES) {
                        return result;
                    }
                    return SUCCES;
                }
                else{
                    result = precedenceAction(insideFunction, sToken, stack, in_function,1);
                    if (result < 113 || result > 117) {
                        return result;
                    }
                    strClean(sToken->content.str);
                    strCpyStr(sToken->content.str, activeString);
                    TNode *active = BVSSearch(insideFunction->rootPtr, *sToken);
                    active->type = result;
                    if ((result = getNextToken(sToken)) != SUCCES) {
                        return result;
                    }
                    afterAssign = false;
                    result = statlist(sToken, fun_id);
                    if (result != SUCCES) {
                        return result;
                    }
                    return SUCCES;
                }
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
        case PARAM_FUNCTION: 
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
                    printf("%s LF@&param%d\n", DEFVAR, repeat);
                    printf("%s LF@&param%d LF@&fun_param%d\n", MOVE, repeat, repeat);
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
        case PARAM_IF_WHILE: 
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
        case PARAM_WRITE: 
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
                    if(sToken->type == TYPE_STRING){
                    printf("string@%s ", sToken->content.str->str);
                    }
                    if(sToken->type == TYPE_INTEGER_NUMBER){
                    printf("int@%s ", sToken->content.str->str);
                    }

                    if(sToken->type == TYPE_DOUBLE_NUMBER){
                    printf("float@%s ", sToken->content.str->str);
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
            }
            return SYN_ERROR;
        case PARAM_READI: 
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
        case PARAM_READS: 
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
        case PARAM_READF: 
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
        case PARAM_STRLEN: 

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

                    printf("%s LF@&%s string@%s\n", STRLEN,(activeString->str)+1, sToken->content.str->str);
                }
                else{
                    printf("%s GF@&%s string@%s\n", STRLEN,(activeString->str)+1, sToken->content.str->str);
                }
                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if(sToken->type == TYPE_RBRACKET){
                    return SUCCES;
                }
            }
            return SYN_ERROR;
        case PARAM_SUBSTRING: 

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
                            printf("%s &error LF@&cmp bool@true\n", JUMPIFEQ);

                        }

                        else{
                            tmpCounter++;

                            printf("%s LF@&tmp%d\n", DEFVAR, tmpCounter);
                            printf("%s LF@&res%d\n", DEFVAR, unique);
                            printf("%s LF@&tmp%d int@%s\n",MOVE,tmpCounter, sToken->content.str->str);
                            printf("%s LF@&cmp LF@&tmp%d int@%d\n", LT, tmpCounter, 0);
                            printf("%s &error LF@&cmp bool@true\n", JUMPIFEQ);
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
                                printf("%s &error LF@&cmp bool@true\n", JUMPIFEQ);
                                printf("%s LF@&cmp LF@&%s int@%d\n", LT, (sToken->content.str->str)+1, 0);
                                printf("%s &error LF@&cmp bool@true\n", JUMPIFEQ);
                                printf("%s LF@&%s\n", DEFVAR, (activeString->str)+1);
                                printf("%s LF@&%s string@" "\n",MOVE, (activeString->str)+1);
                                printf("%s &substring%d\n", LABEL, unique);
                                printf("%s LF@&tmp%d LF@&tmp%d LF@&%s\n", SUBSTRING, unique,tmpCounter, (sToken->content.str->str)+1);
                                printf("%s LF@&%s LF@&%s LF@&tmp%d\n", CONCAT, (activeString->str)+1, (activeString->str)+1, unique);

                                printf("%s LF@&cmp2 LF@&tmp%d LF@&%s\n", EQ, tmpCounter, (sToken->content.str->str)+1);
                                printf("%s LF@&tmp%d LF@&tmp%d int@%d\n", ADD, tmpCounter,tmpCounter, 1);
                                printf("%s &substring%d LF@&cmp2 bool@true\n",JUMPIFNEQ, unique);


                                printf("%s GF@&%s LF@&%s\n", MOVE, (activeString->str)+1, (activeString->str)+1);
                                printf("%s &legit\n", JUMP);

                            }
                            else{

                                if(varI == false && varJ == false){
                                    printf("%s LF@&tmp%d\n", DEFVAR, (tmpCounter)+1);
                                    printf("%s LF@&tmp%d int@%s\n", MOVE,(tmpCounter)+1, sToken->content.str->str);
                                    printf("%s LF@&cmp LF@&tmp%d LF@&tmp%d\n", GT, tmpCounter,(tmpCounter)+1);
                                    printf("%s &error LF@&cmp bool@true\n", JUMPIFEQ);
                                    printf("%s LF@&cmp LF@&tmp%d int@%d\n", LT, (tmpCounter)+1, 0);
                                    printf("%s &error LF@&cmp bool@true\n", JUMPIFEQ);

                                    printf("%s LF@&%s\n", DEFVAR, (activeString->str)+1);
                                    printf("%s LF@&%s string@\n",MOVE, (activeString->str)+1);

                                    printf("%s &substring%d\n", LABEL, unique);
                                    printf("%s LF@&res%d LF@&tmp%d LF@&tmp%d\n", SUBSTRING, unique,unique, tmpCounter);

                                    printf("%s LF@&%s LF@&%s LF@&res%d\n", CONCAT, (activeString->str)+1,(activeString->str)+1, unique);
                                    printf("%s LF@&cmp2 LF@&tmp%d LF@&tmp%d\n", EQ, tmpCounter, (tmpCounter)+1);
                                    printf("%s LF@&tmp%d LF@&tmp%d int@%d\n", ADD, tmpCounter,tmpCounter, 1);
                                    printf("%s &substring%d LF@&cmp2 bool@true\n",JUMPIFNEQ, unique);
                                    printf("%s GF@&%s LF@&%s\n", MOVE, (activeString->str)+1, (activeString->str)+1);
                                    printf("%s &legit\n", JUMP);
                                }
                                if(varI == true)
                                {
                                    printf("%s LF@&tmp%d\n", DEFVAR, (tmpCounter)+1);
                                    printf("%s LF@&tmp%d int@%s\n", MOVE,(tmpCounter)+1, sToken->content.str->str);

                                    printf("%s LF@&cmp LF@&%s LF@&tmp%d\n", GT, (tmpToken->str)+1,(tmpCounter)+1);
                                    printf("%s &error LF@&cmp bool@true\n", JUMPIFEQ);
                                    printf("%s LF@&cmp LF@&tmp%d int@%d\n", LT, (tmpCounter)+1, 0);
                                    printf("%s &error LF@&cmp bool@true\n", JUMPIFEQ);

                                    printf("%s LF@&%s\n", DEFVAR, (activeString->str)+1);
                                    printf("%s LF@&%s string@\n",MOVE, (activeString->str)+1);

                                    printf("%s &substring%d\n", LABEL, unique);
                                    printf("%s LF@&res%d LF@&%s LF@&tmp%d\n", SUBSTRING, unique,(tmpToken->str)+1, tmpCounter);

                                    printf("%s LF@&%s LF@&%s LF@&res%d\n", CONCAT, (activeString->str)+1,(activeString->str)+1, unique);
                                    printf("%s LF@&cmp2 LF@&%s LF@&tmp%d\n", EQ, (tmpToken->str)+1, (tmpCounter)+1);
                                    printf("%s LF@&%s LF@&%s int@%d\n", ADD, (tmpToken->str)+1,(tmpToken->str)+1, 1);
                                    printf("%s &substring%d LF@&cmp2 bool@true",JUMPIFNEQ, unique);

                                    printf("%s GF@&%s LF@&%s\n", MOVE, (activeString->str)+1, (activeString->str)+1);
                                    printf("%s &legit\n", JUMP);
                                }

                            }
                            strClean(tmpToken);
                            if((result = getNextToken(sToken)) != SUCCES){
                                return  result;
                            }
                            if(sToken->type == TYPE_RBRACKET){
                                printf("%s &error\n", LABEL);
                                if(!in_function)
                                {
                                    printf("%s GF@&%s nil@nil\n", MOVE, (activeString->str)+1);
                                }
                                else
                                {
                                    printf("%s LF@&%s nil@nil\n", MOVE, (activeString->str)+1);
                                }
                                printf("%s &legit\n", LABEL);
                                printf("%s\n", POPFRAME);
                                return SUCCES;
                            }

                        }
                    }
                }
            }
            return SYN_ERROR;
        case PARAM_ORD: 
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
                printf("%s\n",POPFRAME);
                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if(sToken->type == TYPE_RBRACKET){
                    return SUCCES;
                }
            }
            return SYN_ERROR;

        case PARAM_CHR: 
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
                printf("%s LF@&tmp%d\n", DEFVAR, unique);
                printf("%s LF@&tmp%d bool@false\n", MOVE, unique);
                printf("%s LF@&tmp%d int@%s int@%d\n", LT,unique, sToken->content.str->str, 0);
                printf("%s &chr%d bool@true LF@&tmp%d\n",JUMPIFEQ, unique, unique);
                printf("%s LF@&tmp%d int@%s int@%d\n", GT,unique, sToken->content.str->str, 255);
                printf("%s &chr%d bool@true LF@&tmp%d\n",JUMPIFEQ, unique, unique);
                printf("%s GF@&%s int@%s\n", INT2CHAR,(activeString->str)+1, sToken->content.str->str);
                printf("%s &chr_legit\n", JUMP);
                printf("%s &chr%d\n", LABEL, unique);
                printf("%s int@%d\n", EXIT, SEM_ERROR);
                printf("%s &chr_legit\n", LABEL);
                printf("%s\n", POPFRAME);
                if((result = getNextToken(sToken)) != SUCCES){
                    return  result;
                }
                if(sToken->type == TYPE_RBRACKET){
                    return SUCCES;
                }
            }
            return SYN_ERROR;
        case PARAM_RETURN: 
            if((result = getNextToken(sToken)) != SUCCES){
                return  result;
            }
            /////////////////////// IN MAIN PROGRAM ///////////////////////////////////////
            if(!in_function){
                switch(sToken->type){
                    case TYPE_IDENTIFIER:
                        if(BVSSearch_function(functionNames->rootPtr, *sToken) == NULL){
                            return SEM_DEFINE_ERROR;
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
                    if(tmp->return_type != KEYWORD_VOID && fun_id->ret_value == KEYWORD_VOID){
                        return SEM_RETURN_ERROR;
                    }
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
                    if(fun_id->ret_value == KEYWORD_VOID){
                        return SEM_RETURN_ERROR;
                    }
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
                    if(fun_id->ret_value == KEYWORD_VOID){
                        return SEM_RETURN_ERROR;
                    }
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
                    if(fun_id->ret_value == KEYWORD_VOID){
                        return SEM_RETURN_ERROR;
                    }
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
        case PARAM_FUNCTION_CALL: 
        printf("%s\n", CREATEFRAME);
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
                    printf("%s TF@&fun_param%d\n", DEFVAR, repeat);
                    printf("%s TF@&fun_param%d LF@&%s\n", MOVE, repeat, (sToken->content.str->str)+1);

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
                    printf("%s TF@&fun_param%d\n", DEFVAR, repeat);
                    printf("%s TF@&fun_param%d int@%s\n", MOVE, repeat, sToken->content.str->str);
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
    Stack initStack;
    activeStringInit.str = NULL;
    activeStringInit.length = 0;
    activeStringInit.str = 0;

    stack = &initStack;

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

    //initializing trees
    BVSInit(insideFunction);
    BVSInit(mainTree);
    BVSInit_function(functionNames);
    stackInit(stack);
    int result;

    //calling first token after successfull prolog
    if((result = getNextToken(sToken)) == LEX_ERROR){
        return LEX_ERROR;
    }
    else{
        //if the first token is a semicolon returns a syntax serror
        if(sToken->type == TYPE_SEMICOLON){
            return SYN_ERROR;
        }
        //.IFJcode22 has to be on top of every ifjcode22 code
        printf(".IFJcode22\n");

        //calling first statlist
        result = statlist(sToken, fun_id);
        if(result != 0)
        {
        printf("EXIT int@%d\n", result);
        }    
    }
    
    //BVSFree(mainTree);
    //BVSFree(insideFunction);
    //BVSFree_function(functionNames);
    //todo freeToken() funtcion
    return result;
}
