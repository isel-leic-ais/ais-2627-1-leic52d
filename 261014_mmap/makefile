all:	mmap_anon mmap_file build_file

CFLAGS= -Werror -Wall -Wextra

mmap_anon: mmap_anon.c
	gcc -o mmap_anon ${CFLAGS} mmap_anon.c

mmap_file: mmap_file.c
	gcc -o mmap_file ${CFLAGS} mmap_file.c

build_file: build_file.c
	gcc -o build_file ${CFLAGS} build_file.c

clean:
	rm -rf  mmap_anon mmap_file build_file

