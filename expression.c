// Precedencni syntaktická analýza výrazu

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>



#include "expstack.h"
//#include "lexical.h"
#include "symtable.h"


#include "str.h"
#include "parser.h"

bool nexttmpexp;
bool exptmpchoose = 0 ;
int result;


double string2doubleExp(string op){
    char *endptr;

    string *str = malloc(sizeof (string));

    strCpyStr(str, &op);
    double d = strtod(str->str, &endptr);
    return d;
}

//indexes for precedence table
/*

    INDEX_COMP,   // !==,=== :    comp
    INDEX_REL,    // <,>,<=,>= :  rel
    INDEX_ADD_SUB_CON,// +,-, . : asc
    INDEX_MUL_DIV,// *,/ :        md
    INDEX_LEFTBR, // ( :          (
    INDEX_RIGHTBR,// ) :          )
    INDEX_DATA,   // i:           var, int, float, string
    INDEX_DOLLAR,  // $ :          $
    INDEXENUMERROR
*/

/*actions of precedence table
    S,  shift (<)  put SHIFT ("<") to stack
    E,  equal (=)  push input to stack
    R,  reduce (>) pop stack until "<" is found, then reduce expression and generate code
    X,   nothing ( )    ERROR
    ACTIONSENUMERROR for sending an error to the caller
 */

/**
 * @brief Precedence Table
 * [Y][X]
 * X INPUT, Y STACKINPUT
 * @return Returns PrtableActionsEnum, depending on the action to be taken
 *
*/
int prtable[8][8] = {
//       comp|rel|asc| md| ( | ) | i | $
/*comp*/ { R , S , S , S , S , R , S , R },
/*rel*/  { R , R , S , S , S , R , S , R },
/*asc*/  { R , R , R , S , S , R , S , R },
/*md*/   { R , R , R , R , S , R , S , R },
/*(*/    { S , S , S , S , S , E , S , X },
/*)*/    { R , R , R , R , X , R , X , R },
/*i*/    { R , R , R , R , X , R , X , R },
/*$*/    { S , S , S , S , S , X , S , X }
};



PrtableSymbolsEnum prtableTokenToSymbol(token *sToken, int iforass)
{
    if (result != 0){
        return SYMBOLSENUMERROR;
    }
    switch (sToken->type)
    {
        case TYPE_EQUAL:
            return EQUAL;
        case TYPE_NOT_EQUAL:
            return NOT_EQUAL;
        case TYPE_SMALLER_THAN:
            return SMALLER_THAN;
        case TYPE_GREATER_THAN:
            return GREATER_THAN;
        case TYPE_SMALLER_OR_EQUAL:
            return SMALLER_OR_EQUAL;
        case TYPE_GREATER_OR_EQUAL:
            return GREATER_OR_EQUAL;
        case TYPE_ADDITION:
            return ADDITION;
        case TYPE_SUBTRACTION:
            return SUBTRACTION;
        case TYPE_CONCATENATE:
            return CONCATENATE;
        case TYPE_MULTIPLY:
            return MULTIPLY;
        case TYPE_DIVIDE:
            return DIVIDE;
        case TYPE_LBRACKET:
            return LBRACKET;
        case TYPE_RBRACKET:
            return RBRACKET;
        case TYPE_INTEGER_NUMBER:
            return SINT;
        case TYPE_DOUBLE_NUMBER:
            return SFLOAT;
        case TYPE_EXPONENT_NUMBER:
            return SFLOAT;
        case TYPE_STRING:
            return SSTRING;
        case TYPE_VARIABLE:
            return VARIABLE;
        case TYPE_SEMICOLON:
            if(iforass == 1){
                return DOLLAR; //THE ENDER of expression
            }
            else {
                result = SYN_ERROR;
                return SYMBOLSENUMERROR;
            }
        case TYPE_LVINCULUM:
            if(iforass == 2){
                return DOLLAR; //THE ENDER of expression
            }
            else {
                result = SYN_ERROR;
                return SYMBOLSENUMERROR;
            }
        default:
            result = SYN_ERROR;
            return SYMBOLSENUMERROR;
    }
}

int prtableDataTypeToTokenType(DataTypeEnum type){

    switch(type)
    {
        case DATATYPE_INT:
            return TYPE_INTEGER_NUMBER;
        case DATATYPE_FLOAT:
            return TYPE_DOUBLE_NUMBER;
        case DATATYPE_STRING:
            return TYPE_STRING;
        default:
            return SEM_UNDEFINED_ERROR;
    }
}


PrtableIndexEnum prtableSymbolToIndex(PrtableSymbolsEnum symb)
{
    if (result != 0){
        return INDEXENUMERROR;
    }
    switch (symb)
    {
        case EQUAL:
        case NOT_EQUAL:
            return INDEX_COMP;
        case SMALLER_THAN:
        case GREATER_THAN:
        case SMALLER_OR_EQUAL:
        case GREATER_OR_EQUAL:
            return INDEX_REL;
        case ADDITION:
        case SUBTRACTION:
        case CONCATENATE:
            return INDEX_ADD_SUB_CON;
        case MULTIPLY:
        case DIVIDE:
            return INDEX_MUL_DIV;
        case LBRACKET:
            return INDEX_LEFTBR;
        case RBRACKET:
            return INDEX_RIGHTBR;
        case SINT:
        case SFLOAT:
        case SSTRING:
        case VARIABLE:
            return INDEX_DATA;
        default:
            return INDEX_DOLLAR; //THE ENDER


    }
}


/* PRECEDENCE ANALYSIS RULES, REDUCING RIGHT SIDE TO LEFT SIDE
 E -> E === E 
 E -> E !== E
 E -> E < E
 E -> E > E
 E -> E <= E
 E -> E >= E
 E -> E + E
 E -> E - E
 E -> E . E
 E -> E * E
 E -> E / E
 E -> (E)
 E -> i
*/


PrtableRulesEnum pickRule(StackElement *op1, StackElement *op2, StackElement *op3)
{
    if (result != 0){
        return RULESENUMERROR;
    }
    if (&(op1->symbol) != NULL && &(op2->symbol) != NULL && &(op3->symbol) != NULL){
        if (op1->symbol == NON_TERMINAL && op2->symbol == EQUAL && op3->symbol == NON_TERMINAL)
            return RULE_EQUAL;
        else if (op1->symbol == NON_TERMINAL && op2->symbol == NOT_EQUAL && op3->symbol == NON_TERMINAL)
            return RULE_NOT_EQUAL;
        else if (op1->symbol == NON_TERMINAL && op2->symbol == SMALLER_THAN && op3->symbol == NON_TERMINAL)
            return RULE_SMALLER_THAN;
        else if (op1->symbol == NON_TERMINAL && op2->symbol == GREATER_THAN && op3->symbol == NON_TERMINAL)
            return RULE_GREATER_THAN;
        else if (op1->symbol == NON_TERMINAL && op2->symbol == SMALLER_OR_EQUAL && op3->symbol == NON_TERMINAL)
            return RULE_SMALLER_OR_EQUAL;
        else if (op1->symbol == NON_TERMINAL && op2->symbol == GREATER_OR_EQUAL && op3->symbol == NON_TERMINAL)
            return RULE_GREATER_OR_EQUAL;
        else if (op1->symbol == NON_TERMINAL && op2->symbol == ADDITION && op3->symbol == NON_TERMINAL)
            return RULE_ADDITION;
        else if (op1->symbol == NON_TERMINAL && op2->symbol == SUBTRACTION && op3->symbol == NON_TERMINAL)
            return RULE_SUBTRACTION;
        else if (op1->symbol == NON_TERMINAL && op2->symbol == CONCATENATE && op3->symbol == NON_TERMINAL)
            return RULE_CONCATENATE;
        else if (op1->symbol == NON_TERMINAL && op2->symbol == MULTIPLY && op3->symbol == NON_TERMINAL)
            return RULE_MULTIPLY;
        else if (op1->symbol == NON_TERMINAL && op2->symbol == DIVIDE && op3->symbol == NON_TERMINAL)
            return RULE_DIVIDE;
        else if (op1->symbol == RBRACKET && op2->symbol == NON_TERMINAL && op3->symbol == LBRACKET)
            return RULE_BRACKETS;
        else
            return RULE_ERROR;
    }
    if (&(op1->symbol) != NULL && &(op2->symbol) == NULL && &(op3->symbol) == NULL){
        if (op1->symbol == VARIABLE || op1->symbol == SINT || op1->symbol == SFLOAT || op1->symbol == SSTRING)
            return RULE_I;
        else
            return RULE_ERROR;

    }
    return RULE_ERROR;
}



DataTypeEnum getDataType(token *sToken, TRoot *someTree){
    if (result != 0){
        return DATATYPEENUM_ERROR;
    }
    if (sToken->type == TYPE_INTEGER_NUMBER){
        return DATATYPE_INT;
    }else if (sToken->type == TYPE_DOUBLE_NUMBER || sToken->type == TYPE_EXPONENT_NUMBER){
        return DATATYPE_FLOAT;
    }else if (sToken->type == TYPE_STRING){
        return DATATYPE_STRING;
    }else if (sToken->type == TYPE_SEMICOLON || sToken->type == TYPE_RVINCULUM){
        return DATATYPE_NONE;
    }else if (sToken->type == TYPE_VARIABLE){ //search for the value of variable in BST
        TNode* data = BVSSearch(someTree->rootPtr, *sToken);
        if (data != NULL){
            if(data->type == TYPE_INTEGER_NUMBER){
                return DATATYPE_INT;
            }
            else if(data->type == TYPE_DOUBLE_NUMBER || data->type == TYPE_EXPONENT_NUMBER){
                return DATATYPE_FLOAT;
            }
            else if(data->type == TYPE_STRING){
                return DATATYPE_STRING;
            }
        }
        return DATATYPE_NONE;
    }
    return DATATYPE_NONE;
}


DataTypeEnum checkTypeForRule(PrtableRulesEnum rule, StackElement *op1, StackElement *op2, StackElement *op3)
{
    if (result != 0){
        return DATATYPEENUM_ERROR;
    }

    if(rule != RULE_I && rule != RULE_BRACKETS)
        if(op1->datatype == DATATYPE_NONE || op3->datatype == DATATYPE_NONE)
            result = SEM_COMPABILITY_ERROR;

    switch(rule)
    {
        case RULE_EQUAL:
        case RULE_NOT_EQUAL:
        case RULE_SMALLER_THAN:
        case RULE_GREATER_THAN:
        case RULE_SMALLER_OR_EQUAL:
        case RULE_GREATER_OR_EQUAL:
            return DATATYPE_INT;
        case CONCATENATE:
            //RETYPE IF op1 or op3 NOT A STRING v codegenu? asi
            return DATATYPE_STRING;
        case RULE_ADDITION:
        case RULE_SUBTRACTION:
        case RULE_MULTIPLY:
            if (op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_INT){
                return DATATYPE_INT;
            }
            if(op1->datatype == DATATYPE_FLOAT && op3->datatype == DATATYPE_FLOAT){
                return DATATYPE_FLOAT;
            }
            if ( op1->datatype == DATATYPE_FLOAT && op3->datatype == DATATYPE_INT){
                //INT2FLOAT op3
                return DATATYPE_FLOAT;
            }
            if ( op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_FLOAT){
                return DATATYPE_FLOAT;
                //INT2FLOAT op1
            }
            result = SEM_COMPABILITY_ERROR; //neco je undefined nebo string
            return DATATYPEENUM_ERROR;

        case RULE_DIVIDE:

            if (op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_INT){
                return DATATYPE_FLOAT;
            }
            if(op1->datatype == DATATYPE_FLOAT && op3->datatype == DATATYPE_FLOAT){
                return DATATYPE_FLOAT;
            }
            if ( op1->datatype == DATATYPE_FLOAT && op3->datatype == DATATYPE_INT){
                return DATATYPE_FLOAT;
            }
            if ( op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_FLOAT){
                return DATATYPE_FLOAT;
            }
            result = SEM_COMPABILITY_ERROR; //neco je undefined nebo string
            return DATATYPEENUM_ERROR;


        case RULE_I:
            if(op1->datatype != DATATYPE_NONE) {
                return op1->datatype;
            }
            else {
                result = SEM_COMPABILITY_ERROR;
                return DATATYPEENUM_ERROR;
            }
        case RULE_BRACKETS:
            if(op2->datatype != DATATYPE_NONE) {
                return op2->datatype;
            }
            else {
                result = SEM_COMPABILITY_ERROR;
                return DATATYPEENUM_ERROR;
            }
        default:
            result = SEM_COMPABILITY_ERROR;
            return DATATYPEENUM_ERROR;

    }
}


int countSymbols(Stack *stack)
{
    if (result != 0){
        return result;
    }
    StackElement *elem = stackGetTop(stack);
    int count = 0;
    while (elem != NULL)
    {
        if(elem->symbol == SHIFT){
            elem = elem->nextElement;
            return count;

        }
        else{
            elem = elem->nextElement;
            count++;
        }

    }
    return -1;

}


DataTypeEnum reduceExpression(Stack *stack, bool in_function){
    string *expTmp2 = malloc(sizeof (string));
    string *expTmp1 = malloc(sizeof (string));

    strInit(expTmp1);
    strClean(expTmp1);
    strInit(expTmp2);
    strClean(expTmp2);

    char expTmpOne[]= "expTmp1";//strings too be stored in expTmp1 and expTmp2
    char expTmpTwo[]= "expTmp2";

    strcpy(expTmp1->str, expTmpOne);
    strcpy(expTmp2->str, expTmpTwo);

    bool printon = 0;
    if (result != 0){
        return DATATYPEENUM_ERROR;
    }
    int isfirstreduction = 0; //for checking if its the first iteration of a 3 operator reduction
    StackElement *op1 = NULL;
    StackElement *op2 = NULL;
    StackElement *op3 = NULL;
    DataTypeEnum resulttype;
    PrtableRulesEnum rule = RULE_ERROR;

    int count = countSymbols(stack);

    if(count == 1)
    {

        op1 = stack->top;
        rule = pickRule(op1, NULL, NULL); //should pick RULE_I, since it's just one operator
        printon = 0;
    }
    else if(count == 3) {
        op3 = stack->top;
        op2 = op3->nextElement;
        op1 = op2->nextElement;
        rule = pickRule(op3, op2, op1);
        isfirstreduction++;


        printon = 1;
    }else{
        result = SEM_COMPABILITY_ERROR;
        return DATATYPEENUM_ERROR;
    }
    if(rule == RULE_ERROR){
        result = SEM_COMPABILITY_ERROR;
        return DATATYPEENUM_ERROR;
    }
    resulttype =  checkTypeForRule(rule, op1, op2, op3); //final type for expression


    if(printon == 1) { //printing the expression, lots of prints(checks rules, if in function, types of operands, which expTmp to use, etc.)
        if (rule == RULE_ADDITION) {
           if(!in_function){
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", ADD, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s GF@&expTmp1 int@%s int@%s\n", ADD, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", ADD, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s int@%s\n", ADD, op1->codename.str, op3->codename.str);
                            }                            
                    }else{
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", ADD, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s GF@&expTmp2 int@%s int@%s\n", ADD, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", ADD, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s GF@&expTmp2 GF@&%s int@%s\n", ADD, op1->codename.str, op3->codename.str);
                            }                    
                        }
                }

                if(op1->orig == TYPE_DOUBLE_NUMBER && op3->orig == TYPE_DOUBLE_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", ADD, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {   double f = string2doubleExp(op1); 
                                double g = string2doubleExp(op3);
                                printf("%s GF@&expTmp1 float@%a float@%a\n", ADD,f, g);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {     
                                double f = string2doubleExp(op1);                
                                printf("%s GF@&expTmp1 float@%a GF@&%s\n", ADD, f, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                double f = string2doubleExp(op3);
                                printf("%s GF@&expTmp1 GF@&%s float@%a\n", ADD, op1->codename.str,f);
                            }                            
                    }else{
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", ADD, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s GF@&expTmp2 int@%s int@%s\n", ADD, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", ADD, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s GF@&expTmp2 GF@&%s int@%s\n", ADD, op1->codename.str, op3->codename.str);
                            }                    
                        }
                }

                if(op1->orig == TYPE_DOUBLE_NUMBER && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s GF@&%s GF@&%s\n",INT2FLOAT,op3->codename.str,op3->codename.str);
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", ADD, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                
                                printf("%s GF@&expTmp1 int@%s int@%s\n", ADD, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", ADD, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s int@%s\n", ADD, op1->codename.str, op3->codename.str);
                            }                            
                    }else{
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", ADD, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s GF@&expTmp2 int@%s int@%s\n", ADD, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", ADD, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s GF@&expTmp2 GF@&%s int@%s\n", ADD, op1->codename.str, op3->codename.str);
                            }                    
                        }
                }

                
                
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", ADD, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", ADD, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0)
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", ADD, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", ADD, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                    printf("%s GF@&expTmp1 GF@&%s GF@&%s \n", ADD, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                    else{
                    printf("%s GF@&expTmp2 GF@&%s GF@&%s \n", ADD, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", ADD, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", ADD, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0)
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", ADD, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", ADD, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                }
                 if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_DOUBLE_NUMBER)
                {

                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", ADD, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s GF@&expTmp1 float@%s GF@&%s\n", ADD, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0)
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", ADD, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", ADD, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                }

            }
                else{
                      if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", ADD, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s LF@&expTmp1 int@%s int@%s\n", ADD, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", ADD, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s LF@&expTmp1 GF@&%s int@%s\n", ADD, op1->codename.str, op3->codename.str);
                            }                            
                    }else{
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", ADD, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s LF@&expTmp2 int@%s int@%s\n", ADD, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", ADD, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s LF@&expTmp2 LF@&%s int@%s\n", ADD, op1->codename.str, op3->codename.str);
                            }                    
                        }
                }
                
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", ADD, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", ADD, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0)
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", ADD, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", ADD, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s \n", ADD, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                    else{
                    printf("%s LF@&expTmp2 LF@&%s LF@&%s \n", ADD, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", ADD, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", ADD, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0)
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", ADD, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", ADD, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                }
            }   
        }
        else if(rule == RULE_MULTIPLY)
        {
            if(!in_function){
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", MUL, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s GF@&expTmp1 int@%s int@%s\n", MUL, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", MUL, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s int@%s\n", MUL, op1->codename.str, op3->codename.str);
                            }                            
                    }else{
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", MUL, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s GF@&expTmp2 int@%s int@%s\n", MUL, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", MUL, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s GF@&expTmp2 GF@&%s int@%s\n", MUL, op1->codename.str, op3->codename.str);
                            }                    
                        }
                }
                
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", MUL, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", MUL, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0)
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", MUL, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", MUL, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        printf("%s GF@&expTmp1 GF@&%s GF@&%s \n", MUL, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                    else{
                        printf("%s GF@&expTmp2 GF@&%s GF@&%s \n", MUL, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", MUL, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", MUL, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0)
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", MUL, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", MUL, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                }
                
            }
                else{
                      if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", MUL, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s LF@&expTmp1 int@%s int@%s\n", MUL, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", MUL, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s LF@&expTmp1 GF@&%s int@%s\n", MUL, op1->codename.str, op3->codename.str);
                            }                            
                    }else{
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", MUL, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s LF@&expTmp2 int@%s int@%s\n", MUL, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", MUL, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s LF@&expTmp2 LF@&%s int@%s\n", MUL, op1->codename.str, op3->codename.str);
                            }                    
                        }
                }
                
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", MUL, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", MUL, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0)
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", MUL, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", MUL, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        printf("%s LF@&expTmp1 LF@&%s LF@&%s \n", MUL, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                    else{
                        printf("%s LF@&expTmp2 LF@&%s LF@&%s \n", MUL, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", MUL, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", MUL, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0)
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", MUL, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", MUL, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                }
            }     
        
        }
        else if(rule == RULE_GREATER_THAN)
        {
            if(!in_function){
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", GT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s GF@&expTmp1 int@%s int@%s\n", GT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", GT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s int@%s\n", GT, op1->codename.str, op3->codename.str);
                            }                            
                    }else{
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", GT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s GF@&expTmp2 int@%s int@%s\n", GT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", GT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s GF@&expTmp2 GF@&%s int@%s\n", GT, op1->codename.str, op3->codename.str);
                            }                    
                        }
                }
                
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", GT, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", GT, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0)
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", GT, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", GT, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        printf("%s GF@&expTmp1 GF@&%s GF@&%s \n", GT, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                    else{
                        printf("%s GF@&expTmp2 GF@&%s GF@&%s \n", GT, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", GT, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", GT, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0)
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", GT, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", GT, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                }
                
            }
                else{
                      if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", GT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s LF@&expTmp1 int@%s int@%s\n", GT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", GT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s LF@&expTmp1 GF@&%s int@%s\n", GT, op1->codename.str, op3->codename.str);
                            }                            
                    }else{
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", GT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s LF@&expTmp2 int@%s int@%s\n", GT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", GT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s LF@&expTmp2 LF@&%s int@%s\n", GT, op1->codename.str, op3->codename.str);
                            }                    
                        }
                }
                
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", GT, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", GT, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0)
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", GT, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", GT, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        printf("%s LF@&expTmp1 LF@&%s LF@&%s \n", GT, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                    else{
                        printf("%s LF@&expTmp2 LF@&%s LF@&%s \n", GT, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", GT, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", GT, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0)
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", GT, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", GT, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                }
                
        }
        }

        else if(rule == RULE_DIVIDE)
        {
            if(!in_function){
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", DIV, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s GF@&expTmp1 int@%s int@%s\n", DIV, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", DIV, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s int@%s\n", DIV, op1->codename.str, op3->codename.str);
                            }
                    }else{
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", DIV, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s GF@&expTmp2 int@%s int@%s\n", DIV, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", DIV, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s GF@&expTmp2 GF@&%s int@%s\n", DIV, op1->codename.str, op3->codename.str);
                            }                    
                        }
                }
                
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", DIV, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", DIV, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0)
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", DIV, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", DIV, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        printf("%s GF@&expTmp1 GF@&%s GF@&%s \n", DIV, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                    else{
                        printf("%s GF@&expTmp2 GF@&%s GF@&%s \n", DIV, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", DIV, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", DIV, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0)
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", DIV, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", DIV, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                }
                
            }
                else{
                      if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", DIV, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s LF@&expTmp1 int@%s int@%s\n", DIV, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", DIV, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s LF@&expTmp1 GF@&%s int@%s\n", DIV, op1->codename.str, op3->codename.str);
                            }                            
                    }else{
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", DIV, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s LF@&expTmp2 int@%s int@%s\n", DIV, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", DIV, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s LF@&expTmp2 LF@&%s int@%s\n", DIV, op1->codename.str, op3->codename.str);
                            }                    
                        }
                }
                
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", DIV, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", DIV, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0)
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", DIV, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", DIV, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        printf("%s LF@&expTmp1 LF@&%s LF@&%s \n", DIV, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                    else{
                        printf("%s LF@&expTmp2 LF@&%s LF@&%s \n", DIV, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", DIV, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", DIV, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0)
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", DIV, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", DIV, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                }
            }
        }
        else if (rule == RULE_SMALLER_THAN)
        {
            if(!in_function){
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", LT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s GF@&expTmp1 int@%s int@%s\n", LT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", LT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s int@%s\n", LT, op1->codename.str, op3->codename.str);
                            }                            
                    }else{
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", LT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s GF@&expTmp2 int@%s int@%s\n", LT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", LT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s GF@&expTmp2 GF@&%s int@%s\n", LT, op1->codename.str, op3->codename.str);
                            }                    
                        }
                }
                
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", LT, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", LT, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0)
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", LT, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", LT, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        printf("%s GF@&expTmp1 GF@&%s GF@&%s \n", LT, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                    else{
                        printf("%s GF@&expTmp2 GF@&%s GF@&%s \n", LT, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", LT, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", LT, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0)
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", LT, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", LT, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                }
                
            }
                else{
                      if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", LT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s LF@&expTmp1 int@%s int@%s\n", LT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", LT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s LF@&expTmp1 GF@&%s int@%s\n", LT, op1->codename.str, op3->codename.str);
                            }                            
                    }else{
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", LT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s LF@&expTmp2 int@%s int@%s\n", LT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", LT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s LF@&expTmp2 LF@&%s int@%s\n", LT, op1->codename.str, op3->codename.str);
                            }                    
                        }
                }
                
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", LT, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", LT, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0)
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", LT, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", LT, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        printf("%s LF@&expTmp1 LF@&%s LF@&%s \n", LT, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                    else{
                        printf("%s LF@&expTmp2 LF@&%s LF@&%s \n", LT, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", LT, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", LT, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0)
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", LT, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", LT, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                }
                
        }

        }
         else if(rule == RULE_SMALLER_OR_EQUAL)
        {
            if(!in_function){
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", EQ, op1->codename.str, op3->codename.str);
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", LT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s GF@&expTmp1 int@%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                                printf("%s GF@&expTmp1 int@%s int@%s\n", LT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {    
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", EQ, op1->codename.str, op3->codename.str);                 
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", LT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                                printf("%s GF@&expTmp1 GF@&%s int@%s\n", LT, op1->codename.str, op3->codename.str);
                            }                            
                    }else{
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", EQ, op1->codename.str, op3->codename.str);
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", LT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s GF@&expTmp2 int@%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                                printf("%s GF@&expTmp2 int@%s int@%s\n", LT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {   
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", EQ, op1->codename.str, op3->codename.str);                  
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", LT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s GF@&expTmp2 GF@&%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                                printf("%s GF@&expTmp2 GF@&%s int@%s\n", LT, op1->codename.str, op3->codename.str);
                            }                    
                        }
                }
                
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", LT, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", LT, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0)
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", LT, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", LT, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        printf("%s GF@&expTmp1 GF@&%s GF@&%s \n", EQ, (op1->codename.str)+1, (op3->codename.str)+1);
                        printf("%s GF@&expTmp1 GF@&%s GF@&%s \n", LT, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                    else{
                        printf("%s GF@&expTmp2 GF@&%s GF@&%s \n", EQ, (op1->codename.str)+1, (op3->codename.str)+1);
                        printf("%s GF@&expTmp2 GF@&%s GF@&%s \n", LT, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", LT, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", LT, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0)
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", LT, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", LT, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                }
                
            }
                else{
                      if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", EQ, op1->codename.str, op3->codename.str);
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", LT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {   
                                printf("%s LF@&expTmp1 int@%s int@%s\n", EQ, op1->codename.str, op3->codename.str); 
                                printf("%s LF@&expTmp1 int@%s int@%s\n", LT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {       
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", EQ, op1->codename.str, op3->codename.str);              
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", LT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s LF@&expTmp1 GF@&%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                                printf("%s LF@&expTmp1 GF@&%s int@%s\n", LT, op1->codename.str, op3->codename.str);
                            }                            
                    }else{
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", EQ, op1->codename.str, op3->codename.str);
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", LT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s LF@&expTmp2 int@%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                                printf("%s LF@&expTmp2 int@%s int@%s\n", LT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {    
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", EQ, op1->codename.str, op3->codename.str);                 
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", LT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s LF@&expTmp2 LF@&%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                                printf("%s LF@&expTmp2 LF@&%s int@%s\n", LT, op1->codename.str, op3->codename.str);
                            }                    
                        }
                }
                
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", LT, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", LT, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0)
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", LT, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", LT, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        printf("%s LF@&expTmp1 LF@&%s LF@&%s \n", EQ, (op1->codename.str)+1, (op3->codename.str)+1);
                        printf("%s LF@&expTmp1 LF@&%s LF@&%s \n", LT, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                    else{
                        printf("%s LF@&expTmp2 LF@&%s LF@&%s \n", EQ, (op1->codename.str)+1, (op3->codename.str)+1);
                        printf("%s LF@&expTmp2 LF@&%s LF@&%s \n", LT, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", LT, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", LT, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0)
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", LT, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", LT, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                }
                
        }
        }

        else if(rule == RULE_GREATER_OR_EQUAL)
        {
            if(!in_function){
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", EQ, op1->codename.str, op3->codename.str);
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", GT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s GF@&expTmp1 int@%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                                printf("%s GF@&expTmp1 int@%s int@%s\n", GT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {    
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", EQ, op1->codename.str, op3->codename.str);                 
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", GT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                                printf("%s GF@&expTmp1 GF@&%s int@%s\n", GT, op1->codename.str, op3->codename.str);
                            }                            
                    }else{
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", EQ, op1->codename.str, op3->codename.str);
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", GT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s GF@&expTmp2 int@%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                                printf("%s GF@&expTmp2 int@%s int@%s\n", GT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {   
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", EQ, op1->codename.str, op3->codename.str);                  
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", GT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s GF@&expTmp2 GF@&%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                                printf("%s GF@&expTmp2 GF@&%s int@%s\n", GT, op1->codename.str, op3->codename.str);
                            }                    
                        }
                }
                
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", GT, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", GT, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0)
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", GT, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", GT, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        printf("%s GF@&expTmp1 GF@&%s GF@&%s \n", EQ, (op1->codename.str)+1, (op3->codename.str)+1);
                        printf("%s GF@&expTmp1 GF@&%s GF@&%s \n", GT, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                    else{
                        printf("%s GF@&expTmp2 GF@&%s GF@&%s \n", EQ, (op1->codename.str)+1, (op3->codename.str)+1);
                        printf("%s GF@&expTmp2 GF@&%s GF@&%s \n", GT, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", GT, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", GT, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0)
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", GT, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", GT, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                }
                
            }
                else{
                      if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", EQ, op1->codename.str, op3->codename.str);
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", GT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {   
                                printf("%s LF@&expTmp1 int@%s int@%s\n", EQ, op1->codename.str, op3->codename.str); 
                                printf("%s LF@&expTmp1 int@%s int@%s\n", GT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {       
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", EQ, op1->codename.str, op3->codename.str);              
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", GT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s LF@&expTmp1 GF@&%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                                printf("%s LF@&expTmp1 GF@&%s int@%s\n", GT, op1->codename.str, op3->codename.str);
                            }                            
                    }else{
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", EQ, op1->codename.str, op3->codename.str);
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", GT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s LF@&expTmp2 int@%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                                printf("%s LF@&expTmp2 int@%s int@%s\n", GT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {    
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", EQ, op1->codename.str, op3->codename.str);                 
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", GT, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s LF@&expTmp2 LF@&%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                                printf("%s LF@&expTmp2 LF@&%s int@%s\n", GT, op1->codename.str, op3->codename.str);
                            }                    
                        }
                }
                
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", GT, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", GT, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0)
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", GT, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", GT, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        printf("%s LF@&expTmp1 LF@&%s LF@&%s \n", EQ, (op1->codename.str)+1, (op3->codename.str)+1);
                        printf("%s LF@&expTmp1 LF@&%s LF@&%s \n", GT, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                    else{
                        printf("%s LF@&expTmp2 LF@&%s LF@&%s \n", EQ, (op1->codename.str)+1, (op3->codename.str)+1);
                        printf("%s LF@&expTmp2 LF@&%s LF@&%s \n", GT, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", GT, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", GT, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0)
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", GT, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", GT, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                }
                
        }
        }

        else if(rule == RULE_EQUAL){
            if(!in_function){
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", EQ, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s GF@&expTmp1 int@%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", EQ, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                            }                            
                    }else{
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", EQ, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s GF@&expTmp2 int@%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", EQ, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s GF@&expTmp2 GF@&%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                            }                    
                        }
                }
                
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0)
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        printf("%s GF@&expTmp1 GF@&%s GF@&%s \n", EQ, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                    else{
                        printf("%s GF@&expTmp2 GF@&%s GF@&%s \n", EQ, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0)
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                }
                 if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_STRING)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s GF@&expTmp1 GF@&%s string@%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                        }
                    else{
                         if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s GF@&expTmp2 GF@&%s string@%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }

                    }   
                if(op1->orig == TYPE_STRING && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s GF@&expTmp1 string@%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                        }
                    else{
                         if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s GF@&expTmp2 string@%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                    }               
                }
                
            }
            }
            else{
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_INTEGER_NUMBER)
                    {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", EQ, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s LF@&expTmp1 int@%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", EQ, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s LF@&expTmp1 GF@&%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                            }                            
                    }else{
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", EQ, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s LF@&expTmp2 int@%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", EQ, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s LF@&expTmp2 LF@&%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                            }                    
                        }
                }
                
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0)
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        printf("%s LF@&expTmp1 LF@&%s LF@&%s \n", EQ, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                    else{
                        printf("%s LF@&expTmp2 LF@&%s LF@&%s \n", EQ, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0)
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                }
                  if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_STRING)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s LF@&expTmp1 LF@&%s string@%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                        }
                    else{
                         if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s LF@&expTmp2 LF@&%s string@%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }

                    }   
                if(op1->orig == TYPE_STRING && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp1 GF@&%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s LF@&expTmp1 string@%s LF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                        }
                    else{
                         if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s LF@&expTmp2 string@%s LF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                        }               
                    }
                }   
            }

        }

        else if(rule == RULE_CONCATENATE)
        {
            if(op1->orig == TYPE_STRING && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        if(strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0)
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", CONCAT, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s GF@&expTmp1 string@%s GF@&%s\n", CONCAT, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0)
                            {
                                printf("%s GF@&expTmp12 GF@&%s GF@&%s\n", CONCAT, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s GF@&expTmp12 string@%s GF@&%s\n", CONCAT, op1->codename.str, (op3->codename.str)+1);
                            } 
                       }
                }
        }     

        else if(rule == RULE_NOT_EQUAL)
        {
            if(!in_function){
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", EQ, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s GF@&expTmp1 int@%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", EQ, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                            }                            
                    }else{
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", EQ, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s GF@&expTmp2 int@%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", EQ, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s GF@&expTmp2 GF@&%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                            }                    
                        }
                }
                
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0)
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        printf("%s GF@&expTmp1 GF@&%s GF@&%s \n", EQ, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                    else{
                        printf("%s GF@&expTmp2 GF@&%s GF@&%s \n", EQ, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s GF@&expTmp1 int@%s GF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0)
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s GF@&expTmp2 int@%s GF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                }
                 if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_STRING)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s GF@&expTmp1 GF@&%s string@%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                        }
                    else{
                         if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s GF@&expTmp2 GF@&%s string@%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }

                    }   
                if(op1->orig == TYPE_STRING && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp1 GF@&%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s GF@&expTmp1 string@%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                        }
                    else{
                         if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s GF@&expTmp2 GF@&%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s GF@&expTmp2 string@%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                    }               
                }
                
            }
            }
            else{
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_INTEGER_NUMBER)
                    {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", EQ, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s LF@&expTmp1 int@%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", EQ, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s LF@&expTmp1 GF@&%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                            }                            
                    }else{
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))     
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", EQ, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {    
                                printf("%s LF@&expTmp2 int@%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op1->codename), "expTmp2") != 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {                     
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", EQ, op1->codename.str, op3->codename.str);
                            }
                        else if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0) &&
                            (strCmpConstStr((&op3->codename), "expTmp1") != 0 &&
                            strCmpConstStr((&op3->codename), "expTmp2") != 0))
                            {
                                printf("%s LF@&expTmp2 LF@&%s int@%s\n", EQ, op1->codename.str, op3->codename.str);
                            }                    
                        }
                }
                
                if(op1->orig == TYPE_INTEGER_NUMBER && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0)
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        printf("%s LF@&expTmp1 LF@&%s LF@&%s \n", EQ, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                    else{
                        printf("%s LF@&expTmp2 LF@&%s LF@&%s \n", EQ, (op1->codename.str)+1, (op3->codename.str)+1);
                    }
                }
                if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_INTEGER_NUMBER)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s LF@&expTmp1 int@%s LF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                    else{
                        if(strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0)
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s LF@&expTmp2 int@%s LF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                    }
                }
                  if(op1->orig == TYPE_VARIABLE && op3->orig == TYPE_STRING)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp1 LF@&%s LF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s LF@&expTmp1 LF@&%s string@%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                        }
                    else{
                         if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }
                            else{
                                printf("%s LF@&expTmp2 LF@&%s string@%s\n", EQ, (op1->codename.str)+1, op3->codename.str);
                            }

                    }   
                if(op1->orig == TYPE_STRING && op3->orig == TYPE_VARIABLE)
                {
                    if(exptmpchoose == 0){
                        if((strCmpConstStr((&op1->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op1->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp1 LF@&%s GF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s LF@&expTmp1 string@%s LF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                        }
                    else{
                         if((strCmpConstStr((&op3->codename), "expTmp1") == 0 ||
                            strCmpConstStr((&op3->codename), "expTmp2") == 0))
                            {
                                printf("%s LF@&expTmp2 LF@&%s LF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                            else{
                                printf("%s LF@&expTmp2 string@%s LF@&%s\n", EQ, op1->codename.str, (op3->codename.str)+1);
                            }
                        }               
                    }
                }   
            }
            if(!in_function){
                if(exptmpchoose == 0){
                    printf("%s GF@&expTmp1 GF@&expTmp1\n", NOT);
                }
                else{
                    printf("%s GF@&expTmp2 GF@&expTmp2\n", NOT);
                }
            }
            else{
                if(exptmpchoose == 0){
                    printf("%s LF@&expTmp1 LF@&expTmp1\n", NOT);
                }
                else{
                    printf("%s LF@&expTmp2 LF@&expTmp2\n", NOT);
                }
            }
        }
    }
            
    

    
   if((stackPop(stack, count + 1)) == 1){ //pop the operands of expression
        result = INT_ERROR;
        return DATATYPEENUM_ERROR;
    }

    if(isfirstreduction > 0){ //if it is not the first 3op reduction we push expTmp1 or expTmp2 to stack
        if(exptmpchoose == 0){
            if((stackPush(stack, NON_TERMINAL, resulttype, *expTmp1, op1->orig)) == 1){
                result = INT_ERROR;
                return DATATYPEENUM_ERROR;
            }
        }else{
            if((stackPush(stack, NON_TERMINAL, resulttype, *expTmp2, op1->orig)) == 1){
                result = INT_ERROR;
                return DATATYPEENUM_ERROR;
            }
        }
    }else {//if it is the first 3op reduction, we need to push the original value of the expression to stack
        if ((stackPush(stack, NON_TERMINAL, resulttype, op1->codename, op1->orig)) == 1) {
            result = INT_ERROR;
            return DATATYPEENUM_ERROR;
        }
    }
    if (strCmpConstStr((&op1->codename), "expTmp1") != 0 ||
        strCmpConstStr((&op3->codename), "expTmp1") != 0 ||
        strCmpConstStr((&op1->codename), "expTmp2") != 0 ||
        strCmpConstStr((&op3->codename), "expTmp2") != 0)
    {
        if (nexttmpexp == 0) {
            exptmpchoose = 0;
            nexttmpexp = 1;
        } else {
            exptmpchoose = 1;
            nexttmpexp = 0;
        }
    }
    return resulttype;

}


//StackElement stacktop;

int precedenceAction(TRoot *someTree, token *sToken, Stack *stack, bool in_function, int iforass, bool* chooseexp){
    if (result != 0){
        return result;
    }
    exptmpchoose = false;
    nexttmpexp = true;
    DataTypeEnum finaltype;


    string t;
    int d = 0;
    bool done = 0;
    token tToken = *sToken;
    int secondgothrough = 0;


    stackPush(stack, DOLLAR, DATATYPE_NONE, t, d);

    while (done != 1){
        if(iforass == 1) {
            if(secondgothrough == 0) {//saves the actual token to use later if we need to assign a simple value;

                tToken.content.str = malloc(sizeof(string));
                strCpyStr(tToken.content.str, sToken->content.str);
            }
            if(secondgothrough == 1) { //if we are in a second go through of while and token is semicolon, assign previous token to expTmp
                double f;
                if(sToken->type == TYPE_SEMICOLON) {
                    if (!in_function) {
                        switch (tToken.type) {
                            case TYPE_INTEGER_NUMBER:
                            if(nexttmpexp==0){
                                printf("%s GF@&expTmp1 int@%s\n", MOVE, tToken.content.str->str);
                            }
                            else
                            {
                                printf("%s GF@&expTmp2 int@%s\n", MOVE, tToken.content.str->str);
                            }
                                break;
                            case TYPE_STRING:
                            if(nexttmpexp==0){
                                printf("%s GF@&expTmp1 string@%s\n", MOVE, tToken.content.str->str);
                            }
                            else{
                                printf("%s GF@&expTmp2 string@%s\n", MOVE, tToken.content.str->str);
                            }
                                break;
                            case TYPE_DOUBLE_NUMBER:
                                f = string2double(&tToken);
                                if(nexttmpexp==0){
                                    
                                printf("%s GF@&expTmp1 float@%a\n", MOVE, f);
                                }
                                else{
                                printf("%s GF@&expTmp2 float@%a\n", MOVE, f);
                                }
                        }
                        free(tToken.content.str);
                        if (tToken.type > 117 || tToken.type < 113) {
                            return SYN_ERROR;
                        }
                        return tToken.type;

                    } else {
                        switch (tToken.type) {
                            case TYPE_INTEGER_NUMBER:
                            if(nexttmpexp==0){
                                printf("%s LF@&expTmp1 int@%s\n", MOVE, tToken.content.str->str);
                            }
                            else{
                                printf("%s LF@&expTmp2 int@%s\n", MOVE, tToken.content.str->str);
                            }
                                break;
                            case TYPE_STRING:
                            if(nexttmpexp==0){
                                printf("%s LF@&expTmp1 string@%s\n", MOVE, tToken.content.str->str);
                            }
                            else{
                                printf("%s LF@&expTmp2 string@%s\n", MOVE, tToken.content.str->str);
                            }
                                break;
                            case TYPE_DOUBLE_NUMBER:
                                f = string2double(&tToken);
                                if(nexttmpexp==0){
                                    printf("%s LF@&expTmp1 float@%a\n", MOVE, f);
                                }
                                else{
                                    printf("%s LF@&expTmp2 float@%a\n", MOVE, f);
                                } 
                        }//CASE TYPE VARIABLE -> tTokentype = BVSSearchVariable
                        free(tToken.content.str);
                        if (tToken.type > 117 || tToken.type < 113) {
                            return SYN_ERROR;
                        }
                        return tToken.type;
                    }
                }
                free(tToken.content.str);
            }
        }

        PrtableSymbolsEnum inputsymbol;
        if ((inputsymbol = prtableTokenToSymbol(sToken, iforass)) == SYMBOLSENUMERROR){
            return result;
        }
        DataTypeEnum inputdatatype;
        if((inputdatatype = getDataType(sToken, someTree)) == DATATYPEENUM_ERROR){
            return result;
        }

        StackElement *stacktopterminal;
        if((stacktopterminal = stackGetTopTerminal(stack)) == NULL ){
            return INT_ERROR;
        }

        PrtableIndexEnum coordinput;
        if((coordinput = prtableSymbolToIndex(inputsymbol)) == INDEXENUMERROR){
            return result;
        }
        PrtableIndexEnum coordstack;
        if((coordstack = prtableSymbolToIndex(stacktopterminal->symbol)) == INDEXENUMERROR){
            return result;
        }
//112 vraci!!!!!!!! checknout rano


        // picks the action to be taken based off of cooridinates in prtable
        PrtableActionsEnum action = prtable[coordstack][coordinput];

        switch(action){
            case S:
                stackInsertAfterTopTerminal(stack, SHIFT, DATATYPE_NONE, t,d);
                stackPush(stack, inputsymbol, inputdatatype, *sToken->content.str, sToken->type);
                if ((result = getNextToken(sToken)) != SUCCES) {
                    return result;
                }
                break;

            case E:
                stackPush(stack, inputsymbol, inputdatatype, *sToken->content.str, sToken->type);
                if ((result = getNextToken(sToken)) != SUCCES) {
                    return result;
                }
                break;

            case R:

                finaltype = reduceExpression(stack, in_function);
                break;

            case X:
                if (coordinput == INDEX_DOLLAR && coordstack == INDEX_DOLLAR) {

                    stackDispose(stack);
                    done = 1;
                    break;
                }
                else {
                    stackDispose(stack);
                    result = SYN_ERROR;
                    return result;
                }
            case ACTIONSENUMERROR:
                return result;

        }

        secondgothrough++;
    }
    if (stackGetTop(stack) != NULL) {
        stackDispose(stack);
    }
    *chooseexp = nexttmpexp; //switches between expTmp and expTmp2
    return prtableDataTypeToTokenType(finaltype);
}
   







