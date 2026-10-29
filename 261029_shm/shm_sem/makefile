all:	shm_writer shm_reader

CFLAGS = -Werror -Wall -Wextra

shm_writer: shm_writer.c
	gcc -o shm_writer ${CFLAGS} shm_writer.c

shm_reader: shm_reader.c
	gcc -o shm_reader ${CFLAGS} shm_reader.c

clean:
	rm -rf shm_writer shm_reader

