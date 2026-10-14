#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define N_INTS	(1024 * 1024)

#define	error_verify_posix(condition, fmt, ...) ({ \
	if (!(condition)) { \
		fprintf(stderr, "%s: " fmt "\n", strerror(errno), ##__VA_ARGS__); \
		exit(EXIT_FAILURE); \
	} \
})


void  suspend(char *msg) {
	printf("\n----------------- %s ----------------", msg);
	getchar();
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <file>\n", argv[0]) ,
		exit(EXIT_FAILURE);
	}
//	int fd = open(argv[1], O_RDONLY);
	int fd = open(argv[1], O_RDWR);
	error_verify_posix(fd != -1, "open(%s,...)\n", argv[1]);

	struct stat sbuf;
	int result = fstat(fd, &sbuf);
	error_verify_posix(result == 0, "ftruncate");

	size_t file_size = sbuf.st_size;

	suspend("Antes do mmap");
	
//	void *pa = mmap(NULL, file_size, PROT_READ, MAP_SHARED, fd, 0);
	void *pa = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	error_verify_posix(pa != MAP_FAILED, "mmap");

	close(fd);
	printf("memory pointer = %p\n", pa);

	suspend("Anter de ler");

	int *ip = pa;
	for (int i = 0; i < N_INTS; ++i) {
		printf("%d ", ip[i]);
	}

	suspend("Antes de escrever");

	ip[N_INTS - 1] = 0x12345678;

	suspend("Antes de munmap");

	result = munmap(pa, file_size);
	error_verify_posix(result != -1, "munmap");
	
	suspend("Antes de sair");
}
