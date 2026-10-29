all:	shm_create shm_write shm_read

CFLAGS = -Werror -Wall -Wextra

shm_create: shm_create.c
	gcc -o shm_create ${CFLAGS} shm_create.c

shm_write: shm_write.c
	gcc -o shm_write ${CFLAGS} shm_write.c

shm_read: shm_read.c
	gcc -o shm_read ${CFLAGS} shm_read.c

clean:
	rm -rf shm_create shm_write shm_read

