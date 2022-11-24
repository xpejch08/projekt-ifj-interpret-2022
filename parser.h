#ifndef PARSER_H
#define PARSER_H

#include "lexical.h"
#include "str.h"
#include "stack.h"
#include "symtable.h"
#include "code_gen.h"

#define PARAM_FUNCTION     800
#define PARAM_IF_WHILE     801
#define PARAM_WRITE        802
#define PARAM_READI        803
#define PARAM_READS        804
#define PARAM_READF        805
#define PARAM_STRLEN       806
#define PARAM_SUBSTRING    807
#define PARAM_ORD          808
#define PARAM_CHR          809
#define PARAM_RETURN       810
#define PARAM_FUNCTON_CALL 811



int declrList();
int statList();
int parametrs(int option, int repeat);
int program();
int parse(void);


#endif //PROJEKT_IFJ_INTERPRET_2022_PARSER_H
