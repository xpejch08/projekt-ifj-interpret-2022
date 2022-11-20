#ifndef CODE_GEN_H
#define CODE_GEN_H

#include "stack.h"

#define COMPLETE 40
#define INCOMPLETE 41

//Instr
#define MOVE 400
#define ADD 401
#define SUB 402
#define MUL 403
#define DIV 404
#define IDIV 405
#define PUSH 406
#define POP 407
#define LT 408
#define GT 409
#define WRITE 410

#define CREATEFRAME 411
#define LABEL 412
#define JUMP 413



typedef struct
{
    int instrID;
    void *leftOp;
    void *rightOp;
    void *result;
    
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

// funkce nastavi parametry instrukce
 TInst setActiveInstruction( int instrID, void* leftOp, void* rightOp, void* result);
// funkce inicializuje seznam instrukcí
void instructionInit(TInstList *instrList);

//funkce uvolni instrukce ze seznamu
void instructionFree(TInstList *instrList);

// funkce vlozi instrukci za aktivni prvek seznamu
void instructionInsertActive(TInstList *instrList, TInst instruction);

//funkce zaktivuje prvni instrukci
void instructionActiveFirst(TInstList *instrList);

//funkce aktivuje nasledujici instrukci
void instructionActiveNext(TInstList *instrList);

// funkce vrati data instrukce
TInst *instructionGetData(TInstList *instrList);

void instructionArgs(TInstList *instrList, TInst *instruction);


#endif