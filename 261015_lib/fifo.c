#include "fifo.h"

#define SIZE_ARRAY(a) (sizeof(a) / sizeof(a[0]))

static int buffer[10];
static int *fifo_put = buffer, *fifo_get = buffer;
int fifo_count;
const int fifo_size = SIZE_ARRAY(buffer);

void fifo_insert(int value)
{
	fifo_count++;
	*fifo_put++ = value;
	if (fifo_put == buffer + SIZE_ARRAY(buffer))
		fifo_put = buffer;
}

int fifo_remove()
{
	fifo_count--;
	int value = *fifo_get++;
	if (fifo_get == buffer + SIZE_ARRAY(buffer))
		fifo_get = buffer;
	return value;
}
