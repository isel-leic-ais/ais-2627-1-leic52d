all:	fifo_server fifo_client

CFLAGS = -Werror -Wall -Wextra

fifo_server: fifo_server.c
	gcc -o fifo_server ${CFLAGS} fifo_server.c

fifo_client: fifo_client.c
	gcc -o fifo_client ${CFLAGS} fifo_client.c

clean:
	rm -rf fifo_server fifo_client client_fifo server_fifo
