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

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "usage: %s <fifo name>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	const char *fifo_name = argv[1];

	int fifofd = open(fifo_name, O_RDONLY);
	error_verify_posix(fifofd != -1, "open(%s)", fifo_name);
	
	printf("Processo recetor\n");
	
	char buffer[10];
	while(true) {
		ssize_t nbytes = read(fifofd, buffer, sizeof buffer);
		if (nbytes == 0)
			break;
		printf("\n[%ld] ", nbytes);
		fwrite(buffer, 1, nbytes, stdout);
	}
}
