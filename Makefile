OBJS	= parser.o symtable.o expression.o expstack.o lexical.o str.o main.o
SOURCE	= parser.c symtable.c expression.c expstack.c lexical.c str.c main.c
HEADER	= parser.h symtable.h expression.h expstack.h lexical.h errors.h str.h
OUT	= 
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

parser.o: parser.c
	$(CC) $(FLAGS) parser.c -std=c99

symtable.o: symtable.c
	$(CC) $(FLAGS) symtable.c -std=c99

expression.o: expression.c
	$(CC) $(FLAGS) expression.c -std=c99

expstack.o: expstack.c
	$(CC) $(FLAGS) expstack.c -std=c99

lexical.o: lexical.c
	$(CC) $(FLAGS) lexical.c -std=c99

str.o: str.c
	$(CC) $(FLAGS) str.c -std=c99

main.o: main.c
	$(CC) $(FLAGS) main.c -std=c99


clean:
	rm -f $(OBJS) $(OUT)