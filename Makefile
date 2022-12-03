OBJS	= main.o lexical.o parser.o expression.o expstack.o stack.o str.o symtable.o
SOURCE	= main.c lexical.c parser.c expression.c expstack.c stack.c str.c symtable.c
HEADER	= errors.h lexical.h parser.h expression.h expstack.h stack.h str.h symtable.h
OUT	= interpret
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = -lm

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c -std=c99

lexical.o: lexical.c
	$(CC) $(FLAGS) lexical.c -std=c99

parser.o: parser.c
	$(CC) $(FLAGS) parser.c -std=c99

expression.o: expression.c
	$(CC) $(FLAGS) expression.c -std=c99

expstack.o: expstack.c
	$(CC) $(FLAGS) expstack.c -std=c99

stack.o: stack.c
	$(CC) $(FLAGS) stack.c -std=c99

str.o: str.c
	$(CC) $(FLAGS) str.c -std=c99

symtable.o: symtable.c
	$(CC) $(FLAGS) symtable.c -std=c99


clean:
	rm -f $(OBJS) $(OUT)

run: $(OUT)
	./$(OUT)