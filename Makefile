OBJS	= parser.o symtable.o lexical.o str.o main.o
SOURCE	= parser.c symtable.c lexical.c str.c main.c
HEADER	= parser.h symtable.h lexical.h errors.h str.h
OUT	= main 
CC	 = gcc
FLAGS	 = -g -c -Wall -lm
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

parser.o: parser.c
	$(CC) $(FLAGS) parser.c -std=c99

symtable.o: symtable.c
	$(CC) $(FLAGS) symtable.c -std=c99

#expression.o: expression.c
#$(CC) $(FLAGS) expression.c -std=c99

#expstack.o: expstack.c
#$(CC) $(FLAGS) expstack.c -std=c99

lexical.o: lexical.c
	$(CC) $(FLAGS) lexical.c -std=c99

str.o: str.c
	$(CC) $(FLAGS) str.c -std=c99

main.o: main.c
	$(CC) $(FLAGS) main.c -std=c99


clean:
	rm -f $(OBJS) $(OUT)