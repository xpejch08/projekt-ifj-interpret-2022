//////////////////////////////////////////////////////////////////
//    @@ Implementace překladače imperativního jazyka IFJ22 @@  //
//                                                              //
//       author: xpejch08                                       //
//                                                              //
//                                                              //
//                                                              //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "lexical.h"
#include "parser.h"
#include "str.h"
#include "symtable.h"
#include "errors.h"

/**]
 * @brief main function of program calling all other functions
 * @return returns result of parsing function
 */
int main(void){

    setSourceFile(stdin);
    token init;
    string initStr;

    initStr.str = NULL;
    initStr.length = 0;
    initStr.alloc = 0;
    init.type = 110;
    init.content.str = &initStr;
    token *sToken;

    sToken = &init;
    int res = prolog(sToken);
    if(res != 0)
    {
        printf("%d\n", res);
        return res;
    }
    return parse();
}

#endif
