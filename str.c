#include <string.h>
#include <stdlib.h>
//#include <malloc.h>
#include "str.h"


#define oneCharInc 8


/**
 * @brief strInit is a function that initializes and allocates memory for a new empty string
 * @param s of type string defined in the header file str.h
 * @return 1 if malloc throws an error 0 if everything works correctly
 */
int strInit(string *s){
    if ((s->str = (char*) malloc(oneCharInc)) == NULL){
        return 1;
    }
    s->str[0] = '\0';
    s->length = 0;
    s->alloc = oneCharInc;
    return 0;
}

/**
 * @brief strFree is a function that frees a string defined in the header file str.h
 * @param s string that we want to free
 */
void strFree(string *s){
    free(s->str);
}

/**
 * @brief strClean is a function that sets first element of string to null '\0'
 * @param s a string that we want to clean but not free
 */
void strClean(string *s){

    s->str[s->length] = '\0';
    s->length = 0;
}

/**
 * @brief strAddChar is a function that adds a character to the end of a string
 * @param s string defined in the header file str.h
 * @param c character that we want to add to the end of a string
 * @return 1 if realloc fails and 0 if it runs correctly
 */
int strAddChar(string *s, char c){

    if(s->length + 1 >= s->alloc){
        if(((s->str) = (char*) realloc(s->str, s->length + oneCharInc)) == NULL){
            return 1;
        }
        s->alloc = s->length + oneCharInc;
    }
    s->str[s->length] = c;
    s->length++;
    s->str[s->length] = '\0';
    return 0;
}

/**
 * @brief function that copies source string to destination string
 * @param s1 destination string
 * @param s2 source string
 * @return 0 if function ends correctly and 1 if error occurs
 */
int strCpyStr(string *s1, string *s2){

    int len2 = s2->length;
    if(len2 > s1->alloc){
        if(((s1->str) = (char*) realloc(s1->str, len2 + 1)) == NULL){
            return 1;
        }
        s1->alloc = len2 + 1;
    }
    strcpy(s1->str, s2->str);
    s1->length = len2 + 1;
    return 0;
}

/**
 * @brief
 * @param s1
 * @param s2
 * @return an integer less than or higher then zero if the strings are different and zero if the strings are the same
 */
int strCmpStr(string *s1, string *s2){

    return(strcmp(s1->str,s2->str));
}



int strCmpConstStr(string *s1, char *s2){

    return(strcmp(s1->str,s2));
}

char *strGetStr(string *s){
    return(s->str);
}

int strGetLen(string *s){
    return (s->length);
}
