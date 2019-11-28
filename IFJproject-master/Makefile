CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic -g

all: main

main: main.o string.o stack.o symtable.o scanner.o lifo.o exprParser.o instruction-list.o
	gcc $(CFLAGS) -o main main.o string.o stack.o symtable.o scanner.o lifo.o exprParser.o instruction-list.o

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

scanner.o: scanner.c
	gcc $(CFLAGS) -c scanner.c -o scanner.o

lifo.o: lifo.c
	gcc $(CFLAGS) -c lifo.c -o lifo.o

exprParser.o: exprParser.c
	gcc $(CFLAGS) -c exprParser.c -o exprParser.o

instruction-list.o: instruction-list.c
	gcc $(CFLAGS) -c instruction-list.c -o instruction-list.o

clear:
	rm main *.o
