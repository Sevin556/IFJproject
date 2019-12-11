CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic -g

all: main

test:
	./main < testy/text.out
	./main < testy/text2.out
	./main < testy/text3.out
	./main < testy/text4.out
	./main < testy/text5.out
	./main < testy/text6.out
	./main < testy/text7.out
	./main < testy/text8.out

main: main.o string.o symtable.o scanner.o parser.o lifo.o exprParser.o instruction-list.o functionParser.o
	gcc $(CFLAGS) -o main main.o string.o symtable.o scanner.o parser.o lifo.o exprParser.o instruction-list.o functionParser.o

main.o: main.c
	gcc $(CFLAGS) -c main.c -o main.o

parser.o: parser.c
	gcc $(CFLAGS) -c parser.c -o parser.o

string.o: string.c
	gcc $(CFLAGS) -c string.c -o string.o

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

functionParser.o: functionParser.c
	gcc $(CFLAGS) -c functionParser.c -o functionParser.o

clear:
	rm main *.o
