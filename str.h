#ifndef STR_H
#define STR_H
typedef struct {
    char *str; //content of string
    int length; //length of string
    int alloc; //amount of memory allocated
} string;


int strInit(string *str);
void strFree(string *str);

void strClean(string *str);
int strAddChar(string *str, char c);
int strCpyStr(string *str1, string *str2);
int strCmpStr(string *str1, string *str2);
int strCmpConstStr(string *str1, char *str2);

char *strGetStr(string *s);
int strGetLen(string *s);

#endif
