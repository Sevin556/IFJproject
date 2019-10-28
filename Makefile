CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic -g
PROJ = proj
FILES = parser.c scanner.c main.c 

all: $(PROJ)

$(PROj): $(FILES)
  gcc $(CFLAGS) -o $(PROJ) $(FILES)

clean:
  rm *.o $(PROJ)
