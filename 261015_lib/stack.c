#define BUFFER_SIZE	10

static int buffer[BUFFER_SIZE];

int *stack_pointer = buffer + BUFFER_SIZE;

void stack_push(int value)
{
	*--stack_pointer = value;
}

int stack_pop()
{
	return *stack_pointer++;
}
