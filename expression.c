// Precedencni syntaktická analýza výrazu

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


//#include "expression.h"
#include "expstack.h"
//#include "lexical.h"
#include "symtable.h"
#include "code_gen.h"
//#include "expstack.c"
#include "str.h"


int result;

//indexy prtable
typedef enum
{
    INDEX_COMP,   // !==,=== :    comp
    INDEX_REL,    // <,>,<=,>= :  rel
    INDEX_ADD_SUB_CON,// +,-, . : asc
    INDEX_MUL_DIV,// *,/ :        md
    INDEX_LEFTBR, // ( :          (
    INDEX_RIGHTBR,// ) :          )
    INDEX_DATA,   // i:           var, int, float, string
    INDEX_DOLLAR,  // $ :          $
    INDEXENUMERROR
} PrtableIndexEnum;

//akce prtable
typedef enum
{
    S, // shift (<)  dej vstup a "<" na zasobnik"
    E, // equal (=)  dej vstup na zasobnik
    R, // reduce (>) hledej na zasobniku "<" a pak pocaď pop a generuj instrukci
    X,  // nic ( )    chyba
    ACTIONSENUMERROR
} PrtableActionsEnum;

//precedencni tabulka, X vodorovne vstup, Y svisle zasobnik
int prtable[8][8] = { // [Y][X]
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


//prevest tokeny na symboly prtable
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
                return DOLLAR;
            }
            else {
                result = SYN_ERROR;
                return SYMBOLSENUMERROR;
            }
        case TYPE_LVINCULUM: ///SEMICOLON -> DOLLAR , EOL -> DOLLAR, default chyba?
            if(iforass == 2){
                return DOLLAR;
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
//prevest datovy typ na token typ def. v lexical.h
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

//prevest symboly na prislusny index prtable
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
            return INDEX_DOLLAR; // DOLLAR 


    }
}


/*PRAVIDLA PRECEDENCNI ANALYZY, z prave strany budeme tvořit levou stranu
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
        else if (op1->symbol == LBRACKET && op2->symbol == NON_TERMINAL && op3->symbol == RBRACKET)
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
    }else if (sToken->type == TYPE_VARIABLE){
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

//struct ExpStack stack;
int countSymbols(Stack *stack) //pocet symbolu ve stacku pred "<" (SHIFT)
{
    if (result != 0){
        return result;
    }
    StackElement *elem = stackGetTopSymbol(stack);
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
//redukuje vyraz, vola se pri akci ">""
DataTypeEnum reduceExpression(Stack *stack, bool in_function){

    bool printon = 0;
    if (result != 0){
        return DATATYPEENUM_ERROR;
    }

    StackElement *op1 = NULL;
    StackElement *op2 = NULL;
    StackElement *op3 = NULL;
    DataTypeEnum resulttype;
    PrtableRulesEnum rule = RULE_ERROR;

    int count = countSymbols(stack);

    if(count == 1)
    {

        op1 = stack->top;
        rule = pickRule(op1, NULL, NULL);
        printon = 0;
    }    
    else if(count == 3)
    {
        op1 = stack->top;
        op2 = op1->nextElement;
        op3 = op2->nextElement;
        rule = pickRule(op3, op2, op1);
        printon = 1;
    }
    else
    {
        result = SEM_COMPABILITY_ERROR;
        return DATATYPEENUM_ERROR;
    }
    if(rule == RULE_ERROR){
        result = SEM_COMPABILITY_ERROR;
        return DATATYPEENUM_ERROR;
    }
    resulttype =  checkTypeForRule(rule, op1, op2, op3);

    
    if(printon == 1) {
        if (rule == RULE_ADDITION) {
            if(!in_function){
                if(op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_INT)
                {
                printf("%s GF@&expTmp int@%s int@%s\n", ADD, op1->codename.str, op3->codename.str);
                }
                if(op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_NONE)
                {
                printf("%s GF@&expTmp int@%s GF@&%s\n", ADD, op1->codename.str, (op3->codename.str)+1);
                }
                if(op1->datatype == DATATYPE_NONE && op3->datatype == DATATYPE_INT)
                {
                printf("%s GF@&expTmp GF@&%s int@%s \n", ADD, (op1->codename.str)+1, op3->codename.str);
                }
            }
            else{
                 if(op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_INT)
                {
                printf("%s LF@&expTmp int@%s int@%s\n", ADD, op1->codename.str, op3->codename.str);
                }
                if(op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_NONE)
                {
                printf("%s LF@&expTmp int@%s GF@&%s\n", ADD, op1->codename.str, (op3->codename.str)+1);
                }
                if(op1->datatype == DATATYPE_NONE && op3->datatype == DATATYPE_INT)
                {
                printf("%s LF@&expTmp GF@&%s int@%s \n", ADD, (op1->codename.str)+1, op3->codename.str);
                }
            }
        }
        else if(rule == RULE_MULTIPLY)
        {
            if(!in_function){
             if(op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_INT)
            {
            printf("%s GF@&expTmp int@%s int@%s\n", MUL, op1->codename.str, op3->codename.str);
            }
            if(op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_NONE)
            {
            printf("%s GF@&expTmp int@%s GF@&%s\n", MUL, op1->codename.str, (op3->codename.str)+1);
            }
            if(op1->datatype == DATATYPE_NONE && op3->datatype == DATATYPE_INT)
            {
            printf("%s GF@&expTmp GF@&%s int@%s \n", MUL, (op1->codename.str)+1, op3->codename.str);
            }
            }
            else{
                  if(op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_INT)
            {
            printf("%s LF@&expTmp int@%s int@%s\n", MUL, op1->codename.str, op3->codename.str);
            }
            if(op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_NONE)
            {
            printf("%s LF@&expTmp int@%s GF@&%s\n", MUL, op1->codename.str, (op3->codename.str)+1);
            }
            if(op1->datatype == DATATYPE_NONE && op3->datatype == DATATYPE_INT)
            {
            printf("%s LF@&expTmp GF@&%s int@%s \n", MUL, (op1->codename.str)+1, op3->codename.str);
            }
            }
        }
        else if(rule == RULE_SUBTRACTION)
        {
            if(!in_function){
             if(op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_INT)
            {
            printf("%s GF@&expTmp int@%s int@%s\n", SUB, op1->codename.str, op3->codename.str);
            }
            if(op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_NONE)
            {
            printf("%s GF@&expTmp int@%s GF@&%s\n", SUB, op1->codename.str, (op3->codename.str)+1);
            }
            if(op1->datatype == DATATYPE_NONE && op3->datatype == DATATYPE_INT)
            {
            printf("%s GF@&expTmp GF@&%s int@%s \n", SUB, (op1->codename.str)+1, op3->codename.str);
            }
            }
            else{
                 if(op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_INT)
            {
            printf("%s LF@&expTmp int@%s int@%s\n", SUB, op1->codename.str, op3->codename.str);
            }
            if(op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_NONE)
            {
            printf("%s LF@&expTmp int@%s GF@&%s\n", SUB, op1->codename.str, (op3->codename.str)+1);
            }
            if(op1->datatype == DATATYPE_NONE && op3->datatype == DATATYPE_INT)
            {
            printf("%s LF@&expTmp GF@&%s int@%s \n", SUB, (op1->codename.str)+1, (op3->codename.str)+1);
            }
            }
        }
        else if(rule == RULE_DIVIDE){
            if(!in_function){
             if(op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_INT)
            {
            printf("%s GF@&expTmp int@%s int@%s\n", DIV, op1->codename.str, op3->codename.str);
            }
            if(op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_NONE)
            {
            printf("%s GF@&expTmp int@%s GF@&%s\n", DIV, op1->codename.str, (op3->codename.str)+1);
            }
            if(op1->datatype == DATATYPE_NONE && op3->datatype == DATATYPE_INT)
            {
            printf("%s GF@&expTmp GF@&%s int@%s \n", DIV, (op1->codename.str)+1, op3->codename.str);
            }
            }
            else{
                 if(op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_INT)
            {
            printf("%s LF@&expTmp int@%s int@%s\n", DIV, op1->codename.str, op3->codename.str);
            }
            if(op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_NONE)
            {
            printf("%s LF@&expTmp int@%s GF@&%s\n", DIV, op1->codename.str, (op3->codename.str)+1);
            }
            if(op1->datatype == DATATYPE_NONE && op3->datatype == DATATYPE_INT)
            {
            printf("%s LF@&expTmp GF@&%s int@%s \n", DIV, (op1->codename.str)+1, op3->codename.str);
            }
            }
        }
        /*
        else if(rule == RULE_I)
        {
            if(!in_function){
             if(op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_INT)
            {
            printf("%s GF@&%s int@%s\n", MOVE, (activeString->str)+1, op3->codename.str);
            }
            }
            else
            {
                 if(op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_INT)
                {
                printf("%s GF@&%s int@%s\n", MOVE, (activeString->str)+1, op3->codename.str);
                }
            }
        }
        */
    }
    if((stackPop(stack, count + 1)) == 1){
        result = INT_ERROR;
        return DATATYPEENUM_ERROR;
    }
    if((stackPush(stack, NON_TERMINAL, resulttype, op1->codename)) == 1){
        result = INT_ERROR;
        return DATATYPEENUM_ERROR;
    }
    return resulttype;
}


//StackElement stacktop;

int precedenceAction(TRoot *someTree, token *sToken, Stack *stack, bool in_function, int iforass){
    if (result != 0){
        return result;
    }
    DataTypeEnum finaltype;

    if(iforass == 1) {
        token tToken = *sToken;
        if ((result = getNextToken(sToken)) != SUCCES) {
            return result;
        }

        if (sToken->type == TYPE_SEMICOLON) {

            if (!in_function) {
                //printf("%s GF@&%s int@%s\n", MOVE, temp, sToken->content.str);
                if (tToken.type > 117) {
                    return SYN_ERROR;
                }
                return tToken.type;
            } else {
                if (tToken.type > 117) {
                    return SYN_ERROR;
                }
                //printf("%s LF@&%s int@%s\n", MOVE, temp, sToken->content.str);
                return tToken.type;
            }
        }
        sToken = &tToken;
    }

    string t;

    bool done = 0;



    stackPush(stack, DOLLAR, DATATYPE_NONE, t);

    
    while (done != 1){
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

        PrtableActionsEnum action = prtable[coordstack][coordinput];
       /* if(inputdatatype == DATATYPE_ENDER){
            action = R;
            done = 1;
        }*/
        switch(action){
            case S:
                stackInsertAfterTopTerminal(stack, SHIFT, DATATYPE_NONE, t);
                stackPush(stack, inputsymbol, inputdatatype, *sToken->content.str);
                if ((result = getNextToken(sToken)) != SUCCES) {
                    return result;
                }
                break;

            case E:
                stackPush(stack, inputsymbol, inputdatatype, *sToken->content.str);
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

    }
    if (stackGetTopSymbol(stack) != NULL) {
       stackDispose(stack);
    }
    return prtableDataTypeToTokenType(finaltype); //vrati TYPE_NECO, def. v lexical.h
}
   



    


    


