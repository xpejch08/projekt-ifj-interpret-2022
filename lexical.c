//////////////////////////////////////////////////////////////////
//    @@ Implementace překladače imperativního jazyka IFJ22 @@  //
//                                                              //
//       author: xpejch08                                       //
//                                                              //
//                                                              //
//                                                              //
//                                                              //
//////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "lexical.h"
#include "str.h"

//todo hexa escape octa escape

#define typeIdentifierState             299
#define basicState                      300 //beginning state waiting for the first character
#define possibleCommentState            301 //state that found the '/' symbol and checks if comment will be one-line block, or won't be at all
#define oneLineCommentState             302 //one line comment state waiting for '\n' to end else returns LEX_ERROR
#define blockCommentState               303 //block comment state waiting for a '*' symbol that will jump to the next state waitForBlockCommentEnd
#define waitForBlockCommentEnd          304 //waitForBlockCommentEnd checks if the '/' symbol followed after the '*' symbol then ends comment otherwise jumps back to blockCommentState
#define keywordOrIdentifierStateBegin   305 //the $ symbol tells us the next token will be a variable
#define keywordOrIdentifierState        306 //reading the variable
#define identifierOrKeywordState        307 //the next token will be an identifier or a keyword
#define numberState                     308 //the next token will be a number, we don't know the specific data type yet
#define smallerThanOrEqualState         309 //the next token can be an assign
#define greaterThanOrEqualState         310 //the next token can be an assign
#define assignOrEqualState              311 //next token can be either a = or a space
#define equalState                      312 //waiting for last =
#define stringStartState                313 //token was ", beginning of string state
#define waitForStringEnd                314 //waiting for " to end string
#define variableRead                    315 //token was $ => next has to be variable identifier
#define backslashState                  316 //escape sequence in a string
#define escapeHexaState                 317 //token is x after backslash, waiting for 2 numbers
#define endHexaState                    318 //ending escape hexadecimal sequence
#define escapeOctaState                 319 //the token after backslash was a number
#define waitOctaState                   320 //waiting for 2 numbers
#define endOctaState                    321 //end of octal escape sequence
#define decimalState                    322 //token was a decimal point => float number
#define exponentState                   323 //token was e or E => exponent number
#define exponentPlusOrMinusState        324 //checking if we have + or - after exponent sign
#define endExponentState                325 //waiting for end of exponent
#define notEqualState                   326 //token was ! waiting for assign
#define notEqualStateEnd                327 //waiting for last assign
#define epilogStateOrType               328 //token was ? which means only epilog can occur
#define decimalEndState                 329 //end of decimal number



//initializing source file
FILE *source;

//function to set source file
void setSourceFile(FILE *f){
    source = f;
}



int hexaToDecimal(const char arr[]){
    int decimal = 0;
    for(int i = 0; i< 2; i++){
        if(arr[i] >= '0' && arr[i] <= '9'){
            decimal += (arr[i] - 48) * 1;
        }
        else if(arr[i] >= 'A' && arr[i] <= 'F'){
            decimal += (arr[i] -55) * 16;
        }
        else if(arr[i] >= 'a' && arr[i] <= 'f'){
            decimal += (arr[i] -87) * 16;
        }
    }
    return decimal;
}

int isTypeFnc(token *attr){
    if(strCmpConstStr(attr->content.str, "string") == 0){
        attr->type = KEYWORD_STRING;
        return SUCCES;
    }
    else if(strCmpConstStr(attr->content.str, "float") == 0){
        attr->type = KEYWORD_FLOAT;
        return SUCCES;
    }
    else if(strCmpConstStr(attr->content.str, "int") == 0){
        attr->type = KEYWORD_INT;
        return SUCCES;
    }
    return LEX_ERROR;
}
/**
 * @brief function that checks if a string is a keyword and sets the token depending on the output
 * @param str dynamic string that we want to compare with a keyword
 * @param attr main token attribute that we set depending on the outcome
 * @return function is always succesfull
 */
void keywordCmp(string *str, token *attr){
    if(strCmpConstStr(str, "else") == 0){
        attr->type = KEYWORD_ELSE;
    }
    else if(strCmpConstStr(str, "if") == 0){
        attr->type = KEYWORD_IF;
    }
    else if(strCmpConstStr(str, "function") == 0){
        attr->type = KEYWORD_FUNCTION;
    }
    else if(strCmpConstStr(str, "float") == 0){
        attr->type = KEYWORD_FLOAT;
    }
    else if(strCmpConstStr(str, "int") == 0){
        attr->type = KEYWORD_INT;
    }
    else if(strCmpConstStr(str, "null") == 0){
        attr->type = KEYWORD_NULL;
    }
    else if(strCmpConstStr(str, "return") == 0){
        attr->type = KEYWORD_RETURN;
    }
    else if(strCmpConstStr(str, "string") == 0){
        attr->type = KEYWORD_STRING;
    }
    else if(strCmpConstStr(str, "void") == 0){
        attr->type = KEYWORD_VOID;
    }
    else if(strCmpConstStr(str, "while") == 0){
        attr->type = KEYWORD_WHILE;
    }
    else{
        attr->type = TYPE_IDENTIFIER;
        strCpyStr(attr->content.str, str);
    }

}

/**
 * @brief function for checking if prolog is correct
 * @param str token passing initialized token for getnexttoken call in function
 * @return returns syntax error if prefix is wrong else returns SUCCESS => 0
 */

int prolog(token *str){
    //initializing and cleaning token
    if(strInit(str->content.str) == 1){
        return INT_ERROR;
    }
    strClean(str->content.str);

    //checking first character from stdin into character variable
    char character = (char) fgetc(stdin);
    ///variable for comparing first part of prolog
    char prolog1[6] = "<?php";
    int i = 0;

    //checking for empty space before first part of prolog
    while(isspace(character)){
        character = (char) fgetc(stdin);
    }

    //getting first 4 tokens which should be <?ph
    while(i<4){
        strAddChar(str->content.str, character);
        character = (char) fgetc(stdin);
        i++;
    }
    //adding lat character which should be 'p' outside of loop and comparing to prolog1
    strAddChar(str->content.str, character);
    if(strCmpConstStr(str->content.str, prolog1) != 0){
        return SYN_ERROR;
    }


    //next tokens should be in order identifier:declare->leftBracket->
    //identifier:strict_types->=->integernumber:1->rightBracket->semicolon
    //calling as tokens to ignore white space
    getNextToken(str);
    if(strCmpConstStr(str->content.str, "declare") != 0){
        return SYN_ERROR;
    }
    getNextToken(str);
    if(str->type != TYPE_LBRACKET){
        return SYN_ERROR;
    }
    getNextToken(str);
    if(strCmpConstStr(str->content.str, "strict_types") != 0){
        return SYN_ERROR;
    }
    getNextToken(str);
    if(str->type != TYPE_ASSIGN){
        return SYN_ERROR;
    }
    getNextToken(str);
    if(strCmpConstStr(str->content.integerNumber, "1") != 0){
        return SYN_ERROR;
    }
    getNextToken(str);
    if(str->type != TYPE_RBRACKET){
        return SYN_ERROR;
    }
    getNextToken(str);
    if(str->type != TYPE_SEMICOLON){
        return SYN_ERROR;
    }

    return 0;
}


/**
 * @brief main function of lexical analysis, finite state machine that creates tokens
 * @param attr token sent to parser
 * @return returns different LEX_ERROR if something goes wrong else returns 0
 */
int getNextToken(token *attr) {
    int state = basicState;
    char character = '\0';
    char hexaEscape1 = '0';
    char hexaEscape2 = '0';
    //char hexaEscape3 = '0'; //todo
    //char octaEscape[3];

    if(source == NULL){
        return INT_ERROR;
    }

    //initializing and cleaning token string
    strInit(attr->content.str);
    strClean(attr->content.str);


    //** finite state machine that calls new character from stdin and ends with a correct token or Lexical eror **//
    while(1){
        /** reading value from stdin **/
        character = (char) fgetc(stdin);

        switch (state){

            /** beginning state
             * ignores white space and jumps into next state depending on the value
             * if character doesn't jump into any state returns lex error
             * **/
            case basicState :
                if(isspace(character)){
                    state = basicState;
                }
                else if(character == '/'){
                    state = possibleCommentState;
                }
                else if(character == '$'){
                    strAddChar(attr->content.str, character);
                    state = variableRead;
                }
                else if(isalpha(character) || character == '_'){
                    state = keywordOrIdentifierStateBegin;
                    ungetc(character, stdin);
                }
                else if(isdigit(character)){
                    ungetc(character, stdin);
                    state = numberState;
                }
                else if(character == '*'){
                    attr->type = TYPE_MULTIPLY; //330
                    strFree(attr->content.str);
                    return SUCCES;
                }
                else if(character == '+'){
                    attr->type = TYPE_ADDITION; //331
                    strFree(attr->content.str);
                    return SUCCES;
                }
                else if(character == '-'){
                    attr->type = TYPE_SUBTRACTION; //332
                    strFree(attr->content.str);
                    return SUCCES;
                }
                else if(character == '.'){
                    attr->type = TYPE_CONCATENATE; //333
                    strFree(attr->content.str);
                    return SUCCES;
                }
                else if(character == EOF){
                    attr->type = TYPE_END_OF_FILE; //334
                    return SUCCES;
                }
                else if(character == '('){
                    attr->type = TYPE_LBRACKET;//335
                    return SUCCES;
                }

                else if(character == ')'){
                    attr->type = TYPE_RBRACKET;//336
                    return SUCCES;
                }
                else if(character == ';'){
                    attr->type = TYPE_SEMICOLON;//337
                    return SUCCES;
                }
                else if(character == ':') {
                    attr->type = TYPE_COLON;//338
                    return SUCCES;
                }
                else if(character == ',') {
                    attr->type = TYPE_COMMA;//339
                    return SUCCES;
                }
                else if(character == '{'){
                    attr->type = TYPE_LVINCULUM;//340
                    return SUCCES;
                }

                else if(character == '}'){
                    attr->type = TYPE_RVINCULUM;//341
                    return SUCCES;
                }

                else if(character == '<'){
                    state = smallerThanOrEqualState;
                }
                else if(character == '?'){
                    state = epilogStateOrType;
                }
                else if(character == '!'){
                    state = notEqualState;
                }
                else if(character == '>'){
                    state = greaterThanOrEqualState;
                }
                else if(character == '='){
                    state = assignOrEqualState;
                }
                else if(character == '"'){
                    state = waitForStringEnd;
                }
                else{
                    return LEX_ERROR;
                }
                break;
                /** if character is a backlash
                 *  the next token can be a comment, comments are ignored
                 * **/
            case possibleCommentState :
                if(character == '/'){
                    state = oneLineCommentState;
                    break;
                }
                    //checking for block comment
                else if(character == '*'){
                    state = blockCommentState;
                    break;
                }
                    //if we don't get an asterisk it means the sign was a divide
                else {
                    attr->type = TYPE_DIVIDE;
                    ungetc(character, source);
                    return SUCCES;
                }
                // we got 2 consecutive '/' which means oneline comment
            case oneLineCommentState :
                if(character == '\n'){
                    state = basicState;
                }
                else if(character == EOF){
                    attr->type = TYPE_END_OF_FILE;
                    return SUCCES;
                }
                break;
                //if we get an asterisk the block comment can end alse we read other characters,
                // if we get EOF returns lex error
            case blockCommentState:
                if(character == '*'){
                    state = waitForBlockCommentEnd;
                    break;
                }
                else if(character == EOF){
                    return LEX_ERROR;
                }
                break;

                //waiting for end of block comment
            case waitForBlockCommentEnd:
                if(character == '/'){
                    state = basicState;
                    break;
                }
                else{
                    state = blockCommentState;
                }
                if(character == EOF){
                    return LEX_ERROR;
                }
                break;

            case keywordOrIdentifierStateBegin:
                if(isalpha(character) || character == '_'){
                    character =  (char) tolower(character);
                    strAddChar(attr->content.str, character);
                    state = keywordOrIdentifierState;
                    break;
                }
            case keywordOrIdentifierState:
                if(isalnum(character) || character == '_'){
                    character = (char) tolower(character);
                    strAddChar(attr->content.str, character);
                }
                else{
                    ungetc(character, source);
                    keywordCmp(attr->content.str, attr);
                    return SUCCES;

                }
                break;
                //waiting for end of variable
            case variableRead:
                if(!isspace(character) && (isalnum(character) != 0 || character == '_')){
                    strAddChar(attr->content.str, character);
                    break;
                }
                else{
                    attr->type = TYPE_VARIABLE;
                    ungetc(character, source);
                    return SUCCES;

                }
            case waitForStringEnd:
                if(character > 31 && character != 92 ) {
                    if (character == '"') {
                        attr->type = TYPE_STRING;
                        return SUCCES;
                    }
                        //if we get dollar without escape sequence it is a lex error
                    else if(character == 36){
                        return LEX_ERROR;

                    }
                        //adding space character as octal escape sequence because of codegen
                    else if(isspace(character)){
                        strAddChar(attr->content.str, '\\');
                        strAddChar(attr->content.str, '0');
                        strAddChar(attr->content.str, '3');
                        strAddChar(attr->content.str, '2');
                    }
                        //adding an exclamation point character as octal escape sequence because of codegen
                    else if(character == '!'){
                        strAddChar(attr->content.str, '\\');
                        strAddChar(attr->content.str, '0');
                        strAddChar(attr->content.str, '3');
                        strAddChar(attr->content.str, '3');
                    }
                    else if((character != EOF)){
                        strAddChar(attr->content.str, character);

                    }
                }
                    //if we get backslash jumb into possible escape sequence
                else if(character == 92){
                    strAddChar(attr->content.str, '\\');
                    state = backslashState;
                }
                else{
                    return LEX_ERROR;
                }
                break;
            case backslashState:
                if(character == 'x'){
                    strAddChar(attr->content.str, 'x');
                    state = escapeHexaState;
                    break;
                }
                else if(isdigit(character)){
                    hexaEscape1 = character;
                    state = escapeOctaState;
                    break;
                }
                    //adding tab character as octal escape sequence because of codegen
                else if(character == 't') {
                    strAddChar(attr->content.str, 'x');
                    strAddChar(attr->content.str, '0');
                    strAddChar(attr->content.str, '9');
                    state = waitForStringEnd;
                    break;
                }
                    //adding newline character as octal escape sequence because of codegen
                else if(character == 'n'){
                    strAddChar(attr->content.str, '0');
                    strAddChar(attr->content.str, '1');
                    strAddChar(attr->content.str, '0');
                    state = waitForStringEnd;
                    break;
                }
                    //adding quotation mark character as octal escape sequence because of codegen
                else if(character == '"'){
                    character = '"';
                    strAddChar(attr->content.str, '0');
                    strAddChar(attr->content.str, '3');
                    strAddChar(attr->content.str, '4');
                    state = waitForStringEnd;
                    break;
                }
                    //dollar can come only after backslash
                else if(character == '$'){
                    character = '$';
                    strAddChar(attr->content.str, character);
                    state = waitForStringEnd;
                    break;
                }
                    //anything else after backslash is not an error
                else {
                    strAddChar(attr->content.str, 92);
                    state = waitForStringEnd;
                    break;
                }
            case escapeHexaState:
                if(isxdigit(character)){
                    strAddChar(attr->content.str, character);
                    state = endHexaState;
                    break;
                }
                else{
                    strAddChar(attr->content.str, 92);
                    strAddChar(attr->content.str, 'x');
                    strAddChar(attr->content.str, character);
                    state = waitForStringEnd;
                    break;
                }
            case endHexaState:
                if(isxdigit(character)){
                    strAddChar(attr->content.str, character);
                    state = waitForStringEnd;
                    break;

                }
                else{
                    strAddChar(attr->content.str, 92);
                    strAddChar(attr->content.str, 'x');
                    strAddChar(attr->content.str, character);
                    state = waitForStringEnd;
                    break;
                }
            case escapeOctaState:
                if(isdigit(character)){
                    hexaEscape2 = character;
                    state = waitOctaState;
                    break;
                }
                else{
                    strAddChar(attr->content.str, hexaEscape1);
                    strAddChar(attr->content.str, hexaEscape2);
                    strAddChar(attr->content.str, character);
                    state = waitForStringEnd;
                    break;
                }
            case waitOctaState:
                if(isdigit(character)){
                    strAddChar(attr->content.str, hexaEscape1);
                    strAddChar(attr->content.str, hexaEscape2);
                    strAddChar(attr->content.str, character);
                    state = waitForStringEnd;
                    break;

                }
                else{

                    state = waitForStringEnd;

                }
                //next token is a number
                //a number can be a regular int or a float or a float with n amount of exponents
            case numberState:
                if(isdigit(character)){
                    strAddChar(attr->content.str, character);
                }
                    //checking if number is decimal
                else if(character == '.'){
                    strAddChar(attr->content.str, character);
                    state = decimalState;
                }
                    //checking if number is exponent
                else if(character == 'e' || character == 'E'){
                    strAddChar(attr->content.str, character);
                    state = exponentPlusOrMinusState;
                }
                else{
                    //the next character could be for example ; so we unget it because of syntax analysis
                    ungetc(character, source);
                    attr->type = TYPE_INTEGER_NUMBER;
                    strCpyStr(attr->content.integerNumber, attr->content.str);
                    return SUCCES;
                }
                break;
            case decimalState:
                if(isdigit(character)){
                    strAddChar(attr->content.str, character);
                    state = decimalEndState;
                }
                else if(character == 'e' || character == 'E'){
                    strAddChar(attr->content.str, character);
                    state = exponentPlusOrMinusState;
                }
                else{
                    return LEX_ERROR;
                }
                break;
            case decimalEndState:
                if(isdigit(character)){
                    strAddChar(attr->content.str, character);
                }
                else if(character == 'e' || character == 'E'){
                    strAddChar(attr->content.str, character);
                    state = exponentPlusOrMinusState;
                }
                else{
                    attr->type = TYPE_DOUBLE_NUMBER;
                    ungetc(character, source);
                    strCpyStr(attr->content.doubleNumber, attr->content.str);
                    return SUCCES;
                }
                break;
                //we can get a - or + sign after an exponent
            case exponentPlusOrMinusState:
                if(character == '+' || character == '-' || isdigit(character) == 1){
                    strAddChar(attr->content.str, character);
                    state = endExponentState;
                    break;
                }
                else{
                    return LEX_ERROR;
                }
            case endExponentState:
                if(isdigit(character)){
                    strAddChar(attr->content.str,character);
                }
                else {
                    ungetc(character, source);
                    attr->type = TYPE_EXPONENT_NUMBER;
                    return SUCCES;
                }
                break;
                /**
                 * next states check if next token is <=, >=, ===, !== or epilog
                 *else returns lex error
                 */
            case smallerThanOrEqualState:
                if(character == '='){
                    attr->type = TYPE_SMALLER_OR_EQUAL;
                    return SUCCES;
                }
                else{
                    ungetc(character, source);
                    attr->type = TYPE_SMALLER_THAN;
                    return SUCCES;
                }
            case greaterThanOrEqualState:
                if(character == '='){
                    attr->type = TYPE_GREATER_OR_EQUAL;
                    return SUCCES;
                }
                else{
                    ungetc(character, source);
                    attr->type = TYPE_GREATER_THAN;
                    return SUCCES;

                }

            case assignOrEqualState:
                if(character == '='){
                    state = equalState;
                    break;
                }
                else{
                    attr->type = TYPE_ASSIGN;
                    ungetc(character, stdin);
                    return SUCCES;
                }
            case equalState:
                if(character == '='){
                    attr->type = TYPE_EQUAL;
                    return SUCCES;
                }
                else{
                    return LEX_ERROR;
                }
                break;
            case notEqualState:
                if(character == '='){
                    state = notEqualStateEnd;
                }
                else{
                    return LEX_ERROR;
                }
                break;
            case epilogStateOrType:
                if(character == '>'){
                    character = (char) getc(stdin);
                    if(character != EOF){
                        return LEX_ERROR;
                    }
                    else{
                        attr->type = TYPE_END_OF_FILE;
                        return SUCCES;
                    }
                }
                else if(isalpha(character)){
                    strAddChar(attr->content.str, character);
                    state = typeIdentifierState;
                }
                break;
            case typeIdentifierState:
                if(isalpha(character)){
                    strAddChar(attr->content.str, character);
                    break;
                }
                else{
                    return isTypeFnc(attr);
                }
            case notEqualStateEnd:
                if(character == '='){
                    attr->type = TYPE_NOT_EQUAL;
                    return SUCCES;
                }
                else{
                    return LEX_ERROR;
                }
                break;



        }

    }

}
