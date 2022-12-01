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



//indexy prtable
typedef enum
{
    INDEX_COMP,   // !==,=== :    comp
    INDEX_REL,    // <,>,<=,>= :  rel
    INDEX_ADD_SUB_CON,// +,-, . : asc
    INDEX_MUL_DIV,// *,/ :        md
    INDEX_LEFTBR, // ( :          (
    INDEX_RIGHTBR,// ) :          )
    INDEX_DATA,   // i:           id, int, float, string
    INDEX_DOLLAR  // $ :          $
} PrtableIndexEnum;

//akce prtable
typedef enum
{
    S, // shift (<)  dej vstup a "<" na zasobnik"
    E, // equal (=)  dej vstup na zasobnik
    R, // reduce (>) hledej na zasobniku "<" a pak pocaď pop a generuj instrukci
    X  // nic ( )    chyba
} PrtableActionsEnum;

//precedencni tabulka, X vodorovne vstup, Y svisle zasobnik
int prtable[8][8] = {
//       comp|rel|asc| md| ( | ) | i | $
/*comp*/ { R , S , S , S , S , R , S , R },
/*rel*/  { R , R , S , S , S , R , S , R },
/*asc*/  { R , R , R , S , S , R , S , R },
/*md*/   { R , R , R , R , S , R , S , R },
/*(*/    { S , S , S , S , S , E , X , X },
/*)*/    { R , R , R , R , X , R , S , R },
/*i*/    { R , R , R , R , X , R , X , X },
/*$*/    { S , S , S , S , S , X , S , X }
};


//prevest tokeny na symboly prtable

static PrtableSymbolsEnum prtableTokenToSymbol(token *sToken)
{
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
        default: ///SEMICOLON -> DOLLAR , EOL -> DOLLAR, default chyba?
            return DOLLAR;
    }
}

//prevest symboly na prislusny index prtable
static PrtableIndexEnum prtableSymbolToIndex(PrtableSymbolsEnum symb)
{
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
* E -> E === E 
* E -> E !== E
* E -> E < E
* E -> E > E
* E -> E <= E
* E -> E >= E
* E -> E + E
* E -> E - E
* E -> E . E
* E -> E * E
* E -> E / E
* E -> (E)
* E -> i
*/

PrtableRulesEnum pickRule(StackElementPtr op1, StackElementPtr op2, StackElementPtr op3)
{
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


//TNode* data;
//TRoot *mainTree; //nejak zaridit aby to byl STEJNY treee jako se pouziva v parser.c
DataTypeEnum getDataType(token *sToken, TRoot *mainTree){
    if (sToken->type == TYPE_INTEGER_NUMBER){
        return DATATYPE_INT;
    }else if (sToken->type == TYPE_DOUBLE_NUMBER || sToken->type == TYPE_EXPONENT_NUMBER){
        return DATATYPE_FLOAT;
    }else if (sToken->type == TYPE_STRING){
        return DATATYPE_STRING;
    }else if (sToken->type == TYPE_VARIABLE){
        TNode* data = BVSSearch(mainTree->rootPtr, *sToken);
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
       /* if(data->content == NULL && data->content == NULL && data->content == NULL)
            return DATATYPE_NONE;
        else if(data->contentegerNumber != NULL && data->content == NULL && data->content == NULL)
            return DATATYPE_INT;
        else if(data->content != NULL && data->contentegerNumber == NULL && data->content == NULL)
            return DATATYPE_FLOAT;
        else if(data->content != NULL && data->contentegerNumber == NULL && data->content == NULL)
            return DATATYPE_STRING;*/


    }
    return DATATYPE_NONE;
        
        
        
    
}   

int checkTypeForRule(PrtableRulesEnum rule, StackElementPtr op1, StackElementPtr op2, StackElementPtr op3, DataTypeEnum* resulttype)
{
    if(rule != RULE_I && rule != RULE_BRACKETS)
        if(op1->datatype == DATATYPE_NONE || op3->datatype == DATATYPE_NONE)
            return SEM_COMPABILITY_ERROR;

    switch(rule)
    {
        case RULE_EQUAL:
        case RULE_NOT_EQUAL:
        case RULE_SMALLER_THAN:
        case RULE_GREATER_THAN:
        case RULE_SMALLER_OR_EQUAL:
        case RULE_GREATER_OR_EQUAL:
            *resulttype = DATATYPE_INT;
                return 0;
        case CONCATENATE:
                //RETYPE IF op1 or op3 NOT A STRING v codegenu? asi
                *resulttype = DATATYPE_STRING;
                return 0;

            return 0;
        case RULE_ADDITION:
        case RULE_SUBTRACTION:
        case RULE_MULTIPLY:
            if (op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_INT){
                *resulttype = DATATYPE_INT;
                return 0;
            }
            if(op1->datatype == DATATYPE_FLOAT && op3->datatype == DATATYPE_FLOAT){
                *resulttype = DATATYPE_FLOAT;
                return 0;
            }
            if ( op1->datatype == DATATYPE_FLOAT && op3->datatype == DATATYPE_INT){
                *resulttype = DATATYPE_FLOAT;
                return 0;
            }
            if ( op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_FLOAT){
                *resulttype = DATATYPE_FLOAT;
                return 0;
            }
            return SEM_COMPABILITY_ERROR; //neco je undefined nebo string
        case RULE_DIVIDE:
            *resulttype = DATATYPE_FLOAT;
            if (op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_INT){
                *resulttype = DATATYPE_FLOAT;
                return 0;
            }
            if(op1->datatype == DATATYPE_FLOAT && op3->datatype == DATATYPE_FLOAT){
                *resulttype = DATATYPE_FLOAT;
                return 0;
            }
            if ( op1->datatype == DATATYPE_FLOAT && op3->datatype == DATATYPE_INT){
                *resulttype = DATATYPE_FLOAT;
                return 0;
            }
            if ( op1->datatype == DATATYPE_INT && op3->datatype == DATATYPE_FLOAT){
                *resulttype = DATATYPE_FLOAT;
                return 0;
            }
            return SEM_COMPABILITY_ERROR; //neco je undefined nebo string
        

        case RULE_I:
            if(op1->datatype != DATATYPE_NONE)
                *resulttype = op1->datatype;
            else
                return SEM_COMPABILITY_ERROR;
        case RULE_BRACKETS:
            if(op2->datatype != DATATYPE_NONE)
                *resulttype = op2->datatype;
            else
                return SEM_COMPABILITY_ERROR;
        default:
            return SEM_COMPABILITY_ERROR;
        
    }
}

//struct ExpStack stack;
int countSymbols(Stack stack) //pocet symbolu ve stacku nez prijde "<"
{
    StackElementPtr elem = stackGetTopSymbol(&stack);
    int count = 0;
    while (elem != NULL)
    {
        if(elem->symbol == STOPPER){
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
int reduceExpression(Stack stack){ 
    
   
    StackElementPtr op1 = NULL;
    StackElementPtr op2 = NULL;
    StackElementPtr op3 = NULL;
    DataTypeEnum resulttype = DATATYPE_NONE;
    PrtableRulesEnum rule = RULE_ERROR;



    if(countSymbols(stack) == 1)
    {
        op1 = stack.top;
        rule = pickRule(op1, NULL, NULL);
    }    
    else if(countSymbols(stack) == 3)
    {
        op1 = stack.top;
        op2 = op1->nextElement;
        op3 = op2->nextElement;
        rule = pickRule(op3, op2, op1);
    }
    else
    {
        return SEM_COMPABILITY_ERROR;
    }
    if(rule == RULE_ERROR)
        return 1;
    
    if(checkTypeForRule(rule, op1, op2, op3, &resulttype) != 0 )
        return checkTypeForRule(rule, op1, op2, op3, &resulttype);
    
    //generate operaci (pomoci rule)

    stackPop(&stack, countSymbols(stack) + 1);
    stackPush(&stack, NON_TERMINAL, resulttype);
    return 0;
}


//StackElementPtr stacktop;

int precedenceAction(TRoot *someTree, token *sToken, Stack stack){ 
    
    int result;
    bool done = 0;

    stackInit(&stack);
    stackPush(&stack, DOLLAR, DATATYPE_NONE);

    
    while (done != 1){
        PrtableSymbolsEnum inputsymbol = prtableTokenToSymbol(sToken);
        DataTypeEnum inputdatatype = getDataType(sToken, someTree);

        PrtableSymbolsEnum stacktopterminal = stackGetTopTerminal(&stack);

        PrtableIndexEnum coordinput = prtableSymbolToIndex(inputsymbol);
        PrtableIndexEnum coordstack = prtableSymbolToIndex(stacktopterminal);

        PrtableActionsEnum action = prtable[coordinput][coordstack];
        switch(action){
            case S:
                stackInsertAfterTop(&stack, STOPPER, DATATYPE_NONE);
                stackPush(&stack, inputsymbol, inputdatatype);
                if ((result = getNextToken(sToken)) != SUCCES) {
                    return result;
                }
                break;

            case E:
                stackPush(&stack, inputsymbol, inputdatatype);
                if ((result = getNextToken(sToken)) != SUCCES) {
                    return result;
                }
                break;

            case R:
                reduceExpression(stack);
                break;

            case X:
                if (coordinput == INDEX_DOLLAR && coordstack == INDEX_DOLLAR) {
                    stackDispose(&stack);
                    done = 1;
                    break;
                }
                else {
                    stackDispose(&stack);
                    return SEM_COMPABILITY_ERROR;
                }
        }

    }

}   



    


    


