#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "echo_service.h"
#include "utils.h"

int main()
{
	printf("Connecting to: %s\n", ECHO_SERVICE_NAME);

	int client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	error_verify_posix(client_fd != -1, "socket()");

	struct sockaddr_un server_addr;
	memset(&server_addr, 0, sizeof server_addr);
	server_addr.sun_family = AF_UNIX;
	strncpy(server_addr.sun_path, ECHO_SERVICE_NAME, sizeof server_addr.sun_path - 1);

	int result = connect(client_fd, (struct sockaddr *) &server_addr, sizeof server_addr);
	error_verify_posix(result != 1, "connect(client_fd, server_addr)");

	char message[MESSAGE_SIZE_MAX], response[MESSAGE_SIZE_MAX];
	while (fgets(message, sizeof message, stdin) != nullptr) {
		if (strncmp(message, "quit", strlen("quit")) == 0)
			break;

		ssize_t nwrote = write(client_fd, message, MESSAGE_SIZE_MAX);
		error_verify_posix(nwrote > 0, "write(client_fd)");
		printf("sent: %s\n", message);

		ssize_t nread = read(client_fd, response, MESSAGE_SIZE_MAX);
		error_verify_posix(nread > 0, "read(client_fd)");
		printf("received: %s\n", response);
	}
	close(client_fd);
}
