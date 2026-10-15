#include <stdio.h>
#include "stack.h"
#include "fifo.h"

int a = 3;
int b = 8;

int main()
{
	stack_push(a);
	printf("%p\n", (void *)stack_pointer);
	b = stack_pop();
	printf("%p\n", (void *)stack_pointer);

	fifo_insert(33);
	printf("%d\n", fifo_count);
	fifo_remove();
	printf("%d\n", fifo_count);
}
