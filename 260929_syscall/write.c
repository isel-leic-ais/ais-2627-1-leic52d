#include <unistd.h>

size_t mywrite(int fd, char *buffer, size_t size);

int main()
{
	mywrite(1, "abcd", 4);
}
