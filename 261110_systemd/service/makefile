all:	server client

CFLAGS = -Werror -Wall -Wextra

server: server.c
	gcc -o server ${CFLAGS} server.c

client: client.c
	gcc -o client ${CFLAGS} client.c

clean:
	rm -rf server client

