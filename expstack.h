//////////////////////////////////////////////////////////////////
//    @@ Implementace překladače imperativního jazyka IFJ22 @@  //
//                                                              //
//       author: xpejch08                                       //
//                                                              //
//                                                              //
//                                                              //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef EXPSTACK_H
#define EXPSTACK_H
#include <stdbool.h>
#include "expression.h"
#include "str.h"


//EXPSTACK FUNKCE
typedef struct StackElement {
	
    PrtableSymbolsEnum symbol;
	DataTypeEnum datatype;
    string codename;
    int orig;
	struct StackElement *nextElement;
} StackElement;


typedef struct{
	StackElement *top;
} Stack;


/**
 * @brief Initialise stack: stack->top = NULL
 * @param stack stack to be initialised
 
 */
void stackInit(Stack *stack );

/**
 * @brief Pushes element to stack
 * @param stack stack to be pushed to
 * @param symbol symbol to be pushed
 * @param datatype datatype of symbol
 * @param codename codename of symbol
 * @param orig original value of symbol
 * @return 0 if push was successful, 1 otherwise
 
 */
int stackPush( Stack *stack, PrtableSymbolsEnum symbol, DataTypeEnum datatype, string codename, int orig );

/**
 * @brief Inserts element to stack after the topmost terminal
 * 
 * @param stack stack to be inserted to
 * @param symbol symbol to be pushed
 * @param datatype datatype of symbol
 * @param codename codename of symbol
 * @param orig original value of symbol
 * @return  0 if insert was successful, 1 otherwise
 */
int stackInsertShift( Stack *stack, PrtableSymbolsEnum symbol, DataTypeEnum datatype);

/**
 * @brief Pops element from stack
 * @param stack stack to be popped from
 * @param n how many times to pop
 */
int stackPop( Stack *stack, int n ); 


/**
 * @brief Returns topmost element of stack
 * 
 * @param stack  stack to find topmost element of
 * @return StackElement* - pointer to topmost element of stack
 */
StackElement* stackGetTop( Stack *stack );

/**
 * @brief Returns topmost terminal of stack
 * 
 * @param stack stack to find topmost terminal of
 * @return StackElement* - pointer to topmost terminal of stack
 */
StackElement* stackGetTopTerminal( Stack *stack );

/**
 * @brief Dispose stack, using stackPop
 * @param stack stack to be disposed
 */
void stackDispose( Stack *stack );

/** @brief Count symbols in stack before "<" (SHIFT)
 * 
 * @param stack stack to be checked
 * @return int number of symbols in stack
*/
int countSymbols(Stack *stack);

/**
 * @brief Converts token to PrtableSymbolsEnum
 * 
 * @param sToken token to be converted
 * @param iforass changes ending condition of precedence(DOLLAR) depending if we are in if/while(LVINCULUM) and assign(SEMICOLON)
 * @return returns PrtableSymbolsEnum 
 */
PrtableSymbolsEnum prtableTokenToSymbol(token *sToken, int iforass);

/**
 * @brief Converts DataTypeEnum to int tokentype
 * 
 * @param type  input DataTypeEnum
 * @return int type defined in lexical.h
 */
int prtableDataTypeToTokenType(DataTypeEnum type);

/**
 * @brief Picks rule based on 3 top elements of stack
 * @param op1 top element of stack
 * @param op2 second element of stack
 * @param op3 third element of stack
 * @return Rule to be used for reducing - PrtableRulesEnum
*/
PrtableRulesEnum pickRule(StackElement *op1, StackElement *op2, StackElement *op3);

/**
 * @brief Get the Data Type of token
 * 
 * @param sToken input token
 * @param someTree tree to search for variable(in_function or mainTree)
 * @return datatype of token - DataTypeEnum 
 */
DataTypeEnum getDataType(token *sToken, TRoot *someTree);

/**
 * @brief Check if the rule is correct
 * 
 * @param rule rule to be checked
 * @param op1 operand 1
 * @param op2 operand 2 usually operator
 * @param op3 operand 3
 * @return datatype based on input operands - DataTypeEnum 
 */
DataTypeEnum checkTypeForRule(PrtableRulesEnum rule, StackElement *op1, StackElement *op2, StackElement *op3);


/** @brief Reduces expression in stack, called when ">" action and then prints the expression
 * 
 * @param stack stack with expression to be reduced
 * @param in_function if the expression is in function (LF or GF)
 * @return datatype of the final expression - DataTypeEnum
*/
DataTypeEnum reduceExpression(Stack *stack, bool in_function);

/**
 * @brief MAIN FUNCTION OF PRECEDENCE ANALYSIS
 * 
 * @param someTree which tree to look for variables (mainTree/in_function)
 * @param sToken current token
 * @param stack stack for storing symbols, used for picking actions, reducing and so on
 * @param in_function tells if we are in function or not
 * @param iforass tells if we are in if/while or assignment
 * @param chooseexp which expTmp to choose for defining variables
 * @return int type of final variable/ or error code 0-8
 */
int precedenceAction(TRoot *someTree, token *sToken, Stack *stack, bool in_function,int iforass, bool* chooseexp);

#endif