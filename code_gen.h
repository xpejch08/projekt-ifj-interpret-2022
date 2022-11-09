#ifndef CODE_GEN_H
#define CODE_GEN_H

#include "stack.h"

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
#define WRITE 409

typedef struct TInstList instrList;
typedef struct TNode tnode;

typedef struct 
{
    int instrID;
    void *addr1;
    void *addr2;
    void *addr3;
}TInst;

typedef struct
{
    struct instrList *first;
    struct instrList *last; 
    struct instrList *active;
}TInstList;

typedef struct instrList
{
    struct instrList *next;
    TInst instruction;
}TItemList;

// funkce inicializuje seznam instrukcí
void instructionInit(TInstList *instrList);

//funkce uvolni instrukce ze seznamu
void instructionFree(TInstList *instrList);

// funkce vlozi instrukci na konec seznamu
void instructionInsertLast(TInstList *instrList, TInst I);

// funkce vlozi isntrukci na zacatek seznamu
void instructionInsertFirst(TInstList *instrList, TInst I);

//funkce zaktivuje prvni instrukci
void instructionActiveFirst(TInstList *instrList);

//funkce aktivuje nasledujici instrukci
void instructionActiveNext(TInstList *instrList);

// funkce vrati data instrukce
TInst *instructionGetData(TInstList *instrList);

#endif