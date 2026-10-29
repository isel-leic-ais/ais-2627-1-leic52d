/*
 *	Criar zona de memória partilhada entre processos na mesma filiação
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/mman.h>
#include <sys/wait.h>

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
	suspend("Parent: Início da main");

	const size_t BLOCK_SIZE	= 100;

	char *pa = mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
	error_verify_posix(pa != MAP_FAILED, "mmap");

	int child_pid = fork();
	if (child_pid == 0) {
		suspend("Child: Antes da leitura");
		puts(pa);
		exit(0);
	}
	strcpy(pa, "Dados em memória partilhada\n");

	int status;
	wait(&status);

	int result = munmap(pa, BLOCK_SIZE);
	error_verify_posix(result != -1, "munmap");

	suspend("Parent: Antes de sair");
}
