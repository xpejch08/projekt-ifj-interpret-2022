// Prediktivní syntaktická analýza výrazu

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "expression.h"
#include "lexical.h"
#include "code_gen.h"
#include "expstack.h"
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

//symboly prtable
typedef enum
{
    S, // shift (<)  dej vstup a "<" na zasobnik"
    E, // equal (=)  dej vstup na zasobnik
    R, // reduce (>) hledej na zasobniku "<" a pak pop
    X  // nic ( )    chyba
} PrtableActionsEnum;

//precedencni tabulka, svisle zasobnik, vodorovne vstup
int prtable[8][8] = {
//       comp|rel|asc| md| ( | ) | i | $
/*comp*/ { R , S , S , S , S , R , S , R },
/*rel*/  { R , R , S , S , S , R , S , R },
/*as*/   { R , R , R , S , S , R , S , R },
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
            return INT;
        case TYPE_DOUBLE_NUMBER:
            return FLOAT;
        case TYPE_EXPONENT_NUMBER:
            return FLOAT;
        case TYPE_STRING:
            return STRING;
        case TYPE_IDENTIFIER:
            return IDENTIFIER;
        default:
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
        case INT:
        case FLOAT:
        case STRING:
        case IDENTIFIER:
            return INDEX_DATA;
        default:
            return INDEX_DOLLAR; // DOLLAR 


    }
}

struct ExpStack stack;
int countSymbols()
{
    StackElementPtr elem = stackGetTopSymbol(&stack);
    int count = 0;
    while (elem != NULL)
    {
        if(elem->symbol == STOPPER){
            return count;
            //TODO MAM HLAD AF
        }
        else{
            count++;
            elem = elem->nextElement;
        }
        
    }
    return count;

}


