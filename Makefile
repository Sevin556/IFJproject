CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic -g

all: main

main: main.o parser.o string.o stack.o symtable.o expression.o
	gcc $(CFLAGS) -o main main.o parser.o string.o stack.o symtable.o expression.o

main.o: main.c
	gcc $(CFLAGS) -c main.c -o main.o

parser.o: parser.c
	gcc $(CFLAGS) -c parser.c -o parser.o

string.o: string.c
	gcc $(CFLAGS) -c string.c -o string.o

stack.o: stack.c
	gcc $(CFLAGS) -c stack.c -o stack.o

symtable.o: symtable.c
	gcc $(CFLAGS) -c symtable.c -o symtable.o

expression.o: expression.c
	gcc $(CFLAGS) -c expression.c -o expression.o

clear:
	rm main *.o
