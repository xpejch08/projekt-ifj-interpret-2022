#ifndef CODE_GEN_H
#define CODE_GEN_H

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

typedef struct instrList TInst;
typedef struct instrNode TInstNode;


typedef struct instrList
{
    TInst *first;
    TInst *last; 
};

typedef struct instrNode
{
    TInstNode *param;
    TInstNode *next;
    int instruction;
};

#endif