CFLAGS=-c -g -Og -Werror -Wall

LFLAGS=

main: main.o
	gcc $(LFLAGS) main.o -L. -ldemo -o main

main.o: main.c
	gcc $(CFLAGS) main.c -o main.o

clean:
	rm -rf main *.o
