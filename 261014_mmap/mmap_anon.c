/* 
 *	Criar um mapeamento anónimo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/mman.h>

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

int main()
{
	printf("PID = %d\n", getpid());
	suspend("Início da main");

	suspend("Antes do mmap");

	const size_t BLOCK_SIZE	= 100;

	void *pa = mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	error_verify_posix(pa != MAP_FAILED, "mmap");

	printf("memory pointer = %p\n", pa);

	suspend("Antes da leitura");

	char *cp = pa;
	for (size_t i = 0; i < BLOCK_SIZE; ++i) {
		printf("%02x ", cp[i]);
	}

	suspend("Antes da escrita");

	*cp = 23;

	suspend("Antes do munmap");

	int result = munmap(pa, BLOCK_SIZE);
	error_verify_posix(result != -1, "munmap");

	suspend("Antes de sair");
}
