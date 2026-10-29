#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>

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

struct shared_data {
	sem_t semaphore;
	int a, b;
};

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "usage: %s <shared area name>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	const char *shm_name = argv[1];
	const int shm_size = sizeof (struct shared_data);

	int shfd = shm_open(shm_name, O_RDWR | O_CREAT | O_EXCL, 0644);
	error_verify_posix(shfd != -1, "shm_open(%s)", shm_name);

	int result = ftruncate(shfd, shm_size);
	error_verify_posix(result != -1, "ftruncate(-, %d)", shm_size);

	void *shptr = mmap(nullptr, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shfd, 0);

	sem_t *mutex = &((struct shared_data *)shptr)->semaphore;
	sem_init(mutex, 1, 0);

	int *a = &((struct shared_data *)shptr)->a;
	int *b = &((struct shared_data *)shptr)->b;

	*a = 0; *b = 1;
	sem_post(mutex);
	sleep(1);
	for (int i = 0; i < 10; ++i) {
		sem_wait(mutex);
		(*a)++;
		(*b)++;
		sem_post(mutex);
		sleep(1);
	}

	result = munmap(shptr, shm_size);
	error_verify_posix(result != -1, "munmap");

	close(shfd);

	shm_unlink(shm_name);
}