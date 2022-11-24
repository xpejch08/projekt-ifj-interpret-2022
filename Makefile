VAR = parser lexical symtable stack experssion expstack scanner semantic str errors
CFLAGS = -std=gnu99 -Wall -Wextra -Werror 
CC = gcc
RM = rm -f

$(VAR) : $(VAR).c
	$(CC) $(CFLAGS) -o $(VAR) $(VAR).c

clean :
	$(RM) *.o $(VAR)