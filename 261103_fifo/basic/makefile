all:	fifo_recv fifo_send

CFLAGS = -Werror -Wall -Wextra

fifo_recv: fifo_recv.c
	gcc -o fifo_recv ${CFLAGS} fifo_recv.c

fifo_send: fifo_send.c
	gcc -o fifo_send ${CFLAGS} fifo_send.c

clean:
	rm -rf fifo_recv fifo_send my_fifo

