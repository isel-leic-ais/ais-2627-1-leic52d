#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>

#define	error_verify_posix(condition, fmt, ...) ({ \
	if (!(condition)) { \
		fprintf(stderr, "%s: " fmt "\n", strerror(errno), ##__VA_ARGS__); \
		exit(EXIT_FAILURE); \
	} \
})

int main(int argc, char *argv[])
{
	if (argc != 3) {
		fprintf(stderr, "usage: %s <server fifo name> <client fifo name>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	const char *server_name = argv[1];
	const char *client_name = argv[2];

	printf("Server FIFO name: %s\n", server_name);
	printf("Client FIFO name: %s\n", client_name);

	int result = mkfifo(server_name, 0622);
	error_verify_posix(result != -1 || errno == EEXIST, "mkfifo(%s)", server_name);

	void (*old)(int) = signal(SIGPIPE, SIG_IGN);
	error_verify_posix(old != SIG_ERR, "signal(SIGPIPE, SIG_IGN)");

	//	Esta abertura em RDWR serve para manter o bloqueio na leitura mesmo se
	int server_fd = open(server_name, O_RDWR);
	error_verify_posix(server_fd != -1, "open(%s)", server_name);

	printf("Server: waiting client\n");

	int client_fd = open(client_name, O_WRONLY);
	error_verify_posix(server_fd != -1, "open(%s)", client_name);

	printf("Server: waiting requests\n");

	char buffer[100];
	while(true) {
		ssize_t nbytes = read(server_fd, buffer, sizeof buffer);
		if (nbytes == 0)
			break;
		write(client_fd, buffer, nbytes);
	}
}
