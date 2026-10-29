#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

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
	if (argc != 3) {
		fprintf(stderr, "usage: %s <shared area name> <shared area size>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	const char *shm_name = argv[1];
	const int shm_size = atoi(argv[2]);

	int shfd = shm_open(shm_name, O_RDWR | O_CREAT | O_EXCL, 0644);
	error_verify_posix(shfd != -1, "shm_open(%s)", shm_name);

	int result = ftruncate(shfd, shm_size);
	error_verify_posix(result != -1, "ftruncate(-, %d)", shm_size);

	suspend("Antes de sair");
}