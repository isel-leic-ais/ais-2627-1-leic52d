#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

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
	printf("PID = %d\n", getpid());
	suspend("Início da main");

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <file>\n", argv[0]) ,
		exit(EXIT_FAILURE);
	}
	int fd = open(argv[1], O_CREAT | O_RDWR | O_TRUNC, 0666);
	error_verify_posix(fd != -1, "open(%s,...)\n", argv[1]);

	const size_t FILE_SIZE = N_INTS * sizeof (int);
	int result = ftruncate(fd, FILE_SIZE);
	error_verify_posix(result == 0, "ftruncate");

	suspend("Antes da mmap");
	void *pa = mmap(NULL, FILE_SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
	error_verify_posix(pa != MAP_FAILED, "mmap");

	close(fd);
	printf("pa = %p\n", pa);

	suspend("Antes escrever");

	int *ip = pa;
	for (int i = 0; i < N_INTS; ++i) {
		ip[i] = i;
		printf("%d ", i);
	}

	suspend("Antes munmap");

	result = munmap(pa, FILE_SIZE);
	error_verify_posix(result != -1, "munmap");

	suspend("Antes de sair");
}
