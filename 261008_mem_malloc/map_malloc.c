#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void suspend(char *msg)
{
	puts(msg);
	getchar();
}

int main()
{
	printf("PID - %d\n", getpid());

	const size_t BLOCK_SIZE = 1000;
	
	suspend("Antes do malloc");
	
	void *ptr = malloc(BLOCK_SIZE);
	printf("ptr = %p\n", ptr);

	suspend("Mais mallocs");

	for (int i = 0; i < 200; ++i) {		
		ptr = malloc(BLOCK_SIZE);
//		printf("ptr = %p\n", ptr);
	}
	suspend("Malloc grande");
	
	ptr = malloc(BLOCK_SIZE * 1000);
	printf("ptr = %p\n", ptr);

	suspend("Antes de sair");
}
