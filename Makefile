VAR = parser lexical symtable stack experssion expstack scanner semantic str
CFLAGS = -std=gnu99 -Wall -Wextra -Werror 
CC = gcc
RM = rm -f

$(VAR) : $(VAR).c
	$(CC) $(CFLAGS) -o $(parser) $(parser).c
	$(CC) $(CFLAGS) -o $(lexical) $(lexical).c
	$(CC) $(CFLAGS) -o $(symtable) $(symtable).c
	$(CC) $(CFLAGS) -o $(stack) $(stack).c
	$(CC) $(CFLAGS) -o $(experssion) $(experssion).c
	$(CC) $(CFLAGS) -o $(expstack) $(expstack).c
	$(CC) $(CFLAGS) -o $(scanner) $(scanner).c
	$(CC) $(CFLAGS) -o $(semantic) $(semantic).c
	$(CC) $(CFLAGS) -o $(str) $(str).c



clean :
	$(RM) *.o $(VAR)