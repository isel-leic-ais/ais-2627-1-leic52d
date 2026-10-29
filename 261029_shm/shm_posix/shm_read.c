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

bool suspend(char *msg) {
	printf("\n----------------- %s ----------------", msg);
	return getchar() == 'q';
}

int main(int argc, char *argv[])
{
	if (argc != 3) {
		fprintf(stderr, "usage: %s <shared area name> <shared area size>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	const char *shm_name = argv[1];
	const int shm_size = atoi(argv[2]);

	int shfd = shm_open(shm_name, O_RDWR, S_IRUSR | S_IWUSR);
	error_verify_posix(shfd != -1, "shm_open(%s)", shm_name);

	int *shptr = mmap(nullptr, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shfd, 0);

	int i = 0;
	do {
		if (i == *shptr)
			printf("*%p - %d\n", shptr, *shptr);
		else
			printf("*%p - %d (x)\n", shptr, *shptr);
		i++;
		shptr++;
	} while (!suspend("Premir para ler"));
}