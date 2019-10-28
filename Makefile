CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic -g

all: main

main: main.o parser.o string.o
	gcc $(CFLAGS) -o main main.o parser.o string.o

main.o: main.c
	gcc $(CFLAGS) -c main.c -o main.o

parser.o: parser.c
	gcc $(CFLAGS) -c parser.c -o parser.o

string.o: string.c
	gcc $(CFLAGS) -c string.c -o string.o

clear:
	rm main *.o
