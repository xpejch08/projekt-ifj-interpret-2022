#ifndef CODE_GEN_H
#define CODE_GEN_H

#include "stack.h"


//Instr
#define MOVE "MOVE"
#define ADD "ADD"
#define SUB "SUB
#define MUL "MUL"
#define DIV "DIV"
#define IDIV "IDIV"
#define PUSH "PUSH"
#define POP "POP"
#define LT "LT
#define GT "GT"
#define WRITE "WRITE"
#define READ "READ"

#define CREATEFRAME "CRATEFRAME"
#define LABEL "LABEL"
#define JUMP "JUMP"
#define JUMPIFEQ "JUMPIFEQ"
#define JUMPIFNEQ "JUMPIFNEQ"
#define EXIT "EXIT"

#define STRING 440
#define INT 441
#define FLOAT 442

#define STRLEN "STRLEN"

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