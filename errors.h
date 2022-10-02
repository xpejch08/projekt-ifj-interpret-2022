#define SUCCES 0 //  SUCCES
#define LEX_ERROR 1 // error in scanner
#define SYN_ERROR 2 // wrong syntax, missing header
#define SEM_DEFINE_ERROR 3 // undefined function, redefine of function
#define SEM_COUNT_ERROR 4 // wrong number/type of parameters in function call, type of the return value from the function
#define SEM_UNDEFINED_ERROR 5 // undefined variable used
#define SEM_RETURN_ERROR 6 // missing/remaining expression in command return from the function
#define SEM_COMPABILITY_ERROR 7 // type compatibility error in arithmetic, string and relational expressions
#define SEM_ERROR 8 // other semantic errors
#define INT_ERROR // error not affected by the input program