#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "lexical.h"
#include "str.h"

//todo ",",":"

#define basicState 300 //beginning state waiting for the first character
#define possibleCommentState 301 //state that found the '/' symbol and checks if comment will be one-line block, or won't be at all
#define oneLineCommentState 302 //one line comment state waiting for '\n' to end else returns LEX_ERROR
#define blockCommentState 303 //block comment state waiting for a '*' symbol that will jump to the next state waitForBlockCommentEnd
#define waitForBlockCommentEnd 304 //waitForBlockCommentEnd checks if the '/' symbol followed after the '*' symbol then ends comment otherwise jumps back to blockCommentState
#define keywordOrIdentifierStateBegin 305 //the $ symbol tells us the next token will be a variable
#define keywordOrIdentifierState 306 //reading the variable
#define identifierOrKeywordState 307 //the next token will be an identifier or a keyword
#define numberState 308 //the next token will be a number, we don't know the specific data type yet //todo numberstate
#define smallerThanOrEqualState 309
#define greaterThanOrEqualState 310
#define assignOrEqualState 311
#define equalState 312
#define stringStartState 313
#define waitForStringEnd 314
#define variableRead 315
#define backslashState 316
#define escapeHexaState 317
#define endHexaState 318
#define escapeOctaState 319
#define waitOctaState 320
#define endOctaState 321
#define decimalState 322
#define exponentState 323
#define exponentPlusOrMinusState 324
#define endExponentState 325
#define notEqualState 326
#define notEqualStateEnd 327


//todo token initialization, change returns of getNextToken function
FILE *source;

void setSourceFile(FILE *f){
    source = f;
}

void initToken(token *token){
    token->type = TYPE_INITIAL;
    strInit(token->content.str);
}
int octaToDecimal(const char *arr){
    int octal = atoi(arr);
    int rightmost;
    int decimal;
    int position;

    while(octal > 0){
        rightmost = octal / 10;
        octal = octal / 10;

        decimal = decimal + rightmost * (int) pow(8, position);
        position++;
    }
    return decimal;
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
/**
 * @brief function that checks if a string is a keyword and sets the token depending on the output
 * @param str dynamic string that we want to compare with a keyword
 * @param attr main token attribute that we set depending on the outcome
 * @return returns 0 if successful
 */
int keywordCmp(string *str, token *attr){
    if(strCmpConstStr(str, "else") == 0){
        attr->type = TYPE_KEYWORD;
        attr->content.keyword = KEYWORD_ELSE;
    }
    else if(strCmpConstStr(str, "if") == 0){
        attr->type = TYPE_KEYWORD;
        attr->content.keyword = KEYWORD_IF;
    }
    else if(strCmpConstStr(str, "function") == 0){
        attr->type = TYPE_KEYWORD;
        attr->content.keyword = KEYWORD_FUNCTION;
    }
    else if(strCmpConstStr(str, "float") == 0){
        attr->type = TYPE_KEYWORD;
        attr->content.keyword = KEYWORD_FLOAT;
    }
    else if(strCmpConstStr(str, "int") == 0){
        attr->type = TYPE_KEYWORD;
        attr->content.keyword = KEYWORD_INT;
    }
    else if(strCmpConstStr(str, "null") == 0){
        attr->type = TYPE_KEYWORD;
        attr->content.keyword = KEYWORD_NULL;
    }
    else if(strCmpConstStr(str, "return") == 0){
        attr->type = TYPE_KEYWORD;
        attr->content.keyword = KEYWORD_RETURN;
    }
    else if(strCmpConstStr(str, "string") == 0){
        attr->type = TYPE_KEYWORD;
        attr->content.keyword = KEYWORD_STRING;
    }
    else if(strCmpConstStr(str, "void") == 0){
        attr->type = TYPE_KEYWORD;
        attr->content.keyword = KEYWORD_VOID;
    }
    else if(strCmpConstStr(str, "while") == 0){
        attr->type = TYPE_KEYWORD;
        attr->content.keyword = KEYWORD_WHILE;
    }
    else{
        attr->type = TYPE_IDENTIFIER;
        strCopyStr(attr->content.str, str);
    }
    return SUCCES;

}

/**
 * @brief main function of lexical analysis, finite state machine that creates tokens
 * @param attr token sent to parser
 * @return returns different LEX_ERROR if something goes wrong else returns 0
 */

int getNextToken(token *attr) {
    int state = basicState;
    char character;
    char *endptr;
    char hexaEscape[2];
    char octaEscape[3];
    strInit(attr->content.str);

    strClean(attr->content.str);

    while(1){
        /** reading value from stdin **/
        character = (char) getc(source);

        switch (state){

            /** beginning state **/
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
                    state = identifierOrKeywordState;
                }
                else if(isdigit(character)){
                    state = numberState;
                }
                else if(character == '*'){
                    attr->type = TYPE_MULTIPLY;
                    strFree(attr->content.str);
                    return SUCCES;
                }
                else if(character == '+'){
                    attr->type = TYPE_ADDITION;
                    strFree(attr->content.str);
                    return SUCCES;
                }
                else if(character == '-'){
                    attr->type = TYPE_SUBTRACTION;
                    strFree(attr->content.str);
                    return SUCCES;
                }
                else if(character == '.'){
                    attr->type = TYPE_CONCATENATE;
                    strFree(attr->content.str);
                    return SUCCES;
                }
                else if(character == EOF){
                    attr->type = TYPE_END_OF_FILE;
                    return SUCCES;
                }
                else if(character == '('){
                    attr->type = TYPE_LBRACKET;
                    return SUCCES;
                }

                else if(character == ')'){
                    attr->type = TYPE_RBRACKET;
                    return SUCCES;
                }
                else if(character == ';'){
                    attr->type = TYPE_SEMICOLON;
                    return SUCCES;
                }
                else if(character == ':') {
                    attr->type = TYPE_COLON;
                    return SUCCES;
                }
                else if(character == ',') {
                    attr->type = TYPE_COMMA;
                    return SUCCES;
                }
                else if(character == '{'){
                    attr->type = TYPE_LVINCULUM;
                    return SUCCES;
                }

                else if(character == '}'){
                    attr->type = TYPE_RVINCULUM;
                    return SUCCES;
                }

                else if(character == '>'){
                    state = smallerThanOrEqualState;
                }
                else if(character == '!'){
                    state = notEqualState;
                }
                else if(character == '<'){
                    state = greaterThanOrEqualState;
                }
                else if(character == '='){
                    state = assignOrEqualState;
                }
                break;
                /** if character is a comment **/
            case possibleCommentState :
                if(character == '/'){
                    state = oneLineCommentState;
                    break;
                }
                else if(character == '*'){
                    state = blockCommentState;
                    break;
                }
                else {
                    attr->type = TYPE_DIVIDE;
                    ungetc(character, source);
                    return SUCCES;
                }
            case oneLineCommentState :
                if(character == '\n'){
                    state = basicState;
                }
                else if(character == EOF){
                    return LEX_ERROR;
                }
                break;
            case blockCommentState:
                if(character == '*'){
                    state = waitForBlockCommentEnd;
                    break;
                }
                else if(character == EOF){
                    return LEX_ERROR;
                }
                break;

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
            case keywordOrIdentifierStateBegin:
                if(isalpha(character) || character == '_'){
                    (char) tolower(character);
                    strAddChar(attr->content.str, character);
                    state = keywordOrIdentifierState;
                }
            case keywordOrIdentifierState:
                if(isalnum(character) || character == '_'){
                    (char) tolower(character);
                    strAddChar(attr->content.str, character);
                }
                else{
                    ungetc(character, source);
                    keywordCmp(attr->content.str, attr);

                }
                break;
            case variableRead:
                if(!(isspace(character))) {
                    strAddChar(attr->content.str, character);
                }
                else{
                    attr->type = TYPE_VARIABLE;
                    ungetc(character, source);

                }
            case stringStartState:
                if(character == '"'){
                    state = waitForStringEnd;
                }
                else{
                    return LEX_ERROR;
                }
                break;
            case waitForStringEnd:
                if(character > 31 && character != 92 ) {
                    if (character == '"') {
                        attr->type = TYPE_STRING;
                        ungetc(character, source);
                        return SUCCES;
                    }
                    else {
                        strAddChar(attr->content.str, character);

                    }
                }
                else if(character == 92){
                    state = backslashState;
                }
                    //backslash

                else{
                    return LEX_ERROR;
                }
            case backslashState:
                if(character == 'x'){
                    state = escapeHexaState;
                }
                else if(isdigit(character)){
                    state = escapeOctaState;
                }
                else if(character == 't') {
                    character = '\t';
                    strAddChar(attr->content.str, character);
                    state = waitForStringEnd;
                }
                else if(character == 'n'){
                    character = '\n';
                    strAddChar(attr->content.str, character);
                    state = waitForStringEnd;
                }
                else {
                    strAddChar(attr->content.str, 92);
                    strAddChar(attr->content.str, character);
                    state = waitForStringEnd;
                }
            case escapeHexaState:
                if(isxdigit(character)){
                    strncat(hexaEscape, &character, 1);
                    state = endHexaState;
                }
                else{
                    strAddChar(attr->content.str, 92);
                    strAddChar(attr->content.str, 'x');
                    strAddChar(attr->content.str, character);
                    state = waitForStringEnd;
                }
            case endHexaState:
                if(isxdigit(character)){
                    strncat(hexaEscape, &character, 1);
                    strAddChar(attr->content.str, (char) hexaToDecimal(hexaEscape));

                }
                else{
                    strAddChar(attr->content.str, character);
                    state = waitForStringEnd;
                }
            case escapeOctaState:
                if(isdigit(character)){
                    strncat(octaEscape, &character, 1);
                    state = waitOctaState;
                }
                else{
                    strAddChar(attr->content.str, 92);
                    strAddChar(attr->content.str, character);
                    state = waitForStringEnd;
                }
            case waitOctaState:
                if(isdigit(character)){
                    strncat(octaEscape, &character, 1);
                    state = endOctaState;

                }
                else{
                    strAddChar(attr->content.str, character);
                    state = waitForStringEnd;
                }
            case endOctaState:
                if(isdigit(character)){
                    strncat(octaEscape, &character, 1);
                    strAddChar(attr->content.str, (char) octaToDecimal(octaEscape));
                    state = waitForStringEnd;

                }
                else{
                    strAddChar(attr->content.str, character);
                    state = waitForStringEnd;
                }
            case numberState:
                if(isdigit(character)){
                    strAddChar(attr->content.str, character);
                }
                else if(character == '.'){
                    strAddChar(attr->content.str, character);
                    state = decimalState;
                }
                else if(character == 'e' || character == 'E'){
                    strAddChar(attr->content.str, character);
                    state = exponentState;
                }
                else{
                    ungetc(character, source);
                    attr->type = TYPE_INTEGER_NUMBER;
                    attr->content.integerNumber = atoi(attr->content.str->str);
                    return SUCCES;
                }
                break;
            case decimalState:
                if(isdigit(character)){
                    strAddChar(attr->content.str, character);
                }
                else if(character == 'e' || character == 'E'){
                    strAddChar(attr->content.str, character);
                    state = exponentPlusOrMinusState;
                }
                else if(isspace(character)){
                    attr->type = TYPE_DOUBLE_NUMBER;
                    attr->content.doubleNumber = strtod(attr->content.str->str, &endptr);
                }
            case exponentPlusOrMinusState:
                if(character == '+' || character == '-' || isdigit(character) == 1){
                    strAddChar(attr->content.str, character);
                    state = endExponentState;
                }
            case endExponentState:
                if(isdigit(character)){
                    strAddChar(attr->content.str,character);
                }
                else if(isspace(character)){
                    ungetc(character, source);
                    attr->type = TYPE_EXPONENT_NUMBER;
                    return SUCCES;
                }
                else{
                    return LEX_ERROR;
                }

            case smallerThanOrEqualState:
                if(character == '='){
                    attr->type = TYPE_SMALLER_OR_EQUAL;
                }
                else{
                    ungetc(character, source);
                    attr->type = TYPE_SMALLER_THAN;
                }
            case greaterThanOrEqualState:
                if(character == '='){
                    attr->type = TYPE_GREATER_OR_EQUAL;
                }
                else{
                    ungetc(character, source);
                    attr->type = TYPE_GREATER_THAN;

                }
            case assignOrEqualState:
                if(character == '='){
                    state = equalState;
                    break;
                }
                else{
                    attr->type = TYPE_ASSIGN;
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
            case notEqualState:
                if(character == '='){
                    state = notEqualStateEnd;
                }
                else{
                    return LEX_ERROR;
                }
            case notEqualStateEnd:
                if(character == '='){
                    attr->type = TYPE_NOT_EQUAL;
                }
                else{
                    return LEX_ERROR;
                }



        }

    }

}
