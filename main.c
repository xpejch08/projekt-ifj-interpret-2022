#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdbool.h>

#include "lexical.h"
#include "parser.h"
#include "str.h"
#include "symtable.h"
#include "errors.h"


int main(void){

    setSourceFile(stdin);
    token init;
    function_save i;
    string initStr;
    initStr.str = NULL;
    initStr.length = 0;
    initStr.alloc = 0;
    init.type = 110;
    init.content.str = &initStr;
    token *sToken;
    sToken = &init;
    //fopen("testIn.txt", "r");
    //prefix(sToken);
    return parse();
}

#endif