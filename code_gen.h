#ifndef CODE_GEN_H
#define CODE_GEN_H

#include "symtable.h"

//Instr
#define MOVS 400
#define ADDS 401
#define SUBS 402
#define MULS 403
#define DIVS 404
#define PUSHS 405
#define POPS 406
#define LTS 407
#define GTS 408

typedef struct TInstList instrList;
typedef struct TNode tnode;


typedef struct
{
    struct instrList *first;
    struct instrList *last; 
    struct instrList *active;
}TInstList;

typedef struct instrList
{
    instrList *next;
    int instruction;
}TItemList;

// funkce inicializuje seznam instrukcí
void instructionInit(TInstList *instrList);

//funkce uvolni instrukci ze seznamu
void instructionFree(TInstList *instrList);


#endif