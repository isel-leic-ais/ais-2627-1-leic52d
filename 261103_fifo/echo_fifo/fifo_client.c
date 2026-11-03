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

	printf("Client: waiting server\n");

	int server_fd = open(server_name, O_WRONLY);
	error_verify_posix(server_fd != -1, "open(%s)", server_name);

	printf("Client: make fifo\n");

	int result = mkfifo(client_name, 0622);
	error_verify_posix(result != -1 || errno == EEXIST, "mkfifo(%s)", client_name);

	int client_fd = open(client_name, O_RDONLY);
	error_verify_posix(client_fd != -1, "open(%s)", client_name);

	printf("Client: sending data\n");

	char line[100];
	char response[100];
	while (true) {
		char *s = fgets(line, sizeof line, stdin);
		if (s == nullptr || strcmp(s, "exit") == 0)
			break;
		printf("sending message: %s\n", line);
		write(server_fd, line, strlen(line) + 1);
		read(client_fd, response, sizeof response);
		printf("received message: %s\n", response);
	}
	close(server_fd);
	close(client_fd);
}
