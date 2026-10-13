#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#define	DATA_SIZE 128 * 1024 * 1024

#define VALUES_SIZE 64 * 1024 * 1024

char data[DATA_SIZE] = {5};

char values[VALUES_SIZE];


void suspend(char *msg)
{
	puts(msg);
	getchar();
}

int sum_array(char array[], size_t size)
{
	int sum = 0;
	for (size_t i = 0; i < size; ++i)
		sum += array[i];
	return sum;
}


void fill_array(char array[], size_t size)
{
	for (size_t i = 0; i < size; ++i)
		array[i] = i;
}

int main()
{
	suspend("Pai: Antes de sum_array(data, ...)");
	
	sum_array(data, sizeof data);

	suspend("Pai: Antes de sum_array(values, ...)");
	
	sum_array(values, sizeof values);

	suspend("Pai: Antes de fill_array(data, ...)");

	fill_array(data, sizeof data);

	suspend("Pai: Antes de fill_array(values, ...)");

	fill_array(values, sizeof values);

	suspend("Pai: Antes do fork()");
	
	if (fork() == 0) {
		suspend("Filho: Antes de alterar a variável data");
		
		data[1000] = 200;
//		fill_array(data, sizeof data);

//		PSS = Private_Clean + Private_Dirty + (Shared_Clean + Shared_Dirty) / 2		

		suspend("Filho: Antes do filho terminar");

		exit(0);
	}
	
	int status;
        wait(&status);
	
	suspend("Pai: Antes de terminar");
}
