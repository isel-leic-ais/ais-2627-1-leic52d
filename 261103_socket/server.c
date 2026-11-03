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
#include <sys/socket.h>
#include <sys/un.h>

#define	error_verify_posix(condition, fmt, ...) ({ \
	if (!(condition)) { \
		fprintf(stderr, "%s: " fmt "\n", strerror(errno), ##__VA_ARGS__); \
		exit(EXIT_FAILURE); \
	} \
})

int main()
{

	int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	
	struct sockaddr_un server_address;
	server_address.sun_family = AF_UNIX;
	strcpy(server_address.sun_path, "server_socket");
	int result = bind(sock_fd, (struct sockaddr*)&server_address, sizeof server_address);
	error_verify_posix(result != -1, "bind()");
	
	result = listen(sock_fd, 10);
	error_verify_posix(result != -1, "bind()");
	
	int client_fd = accept(sock_fd, nullptr, 0);
	
	char buffer[100];
	while(true) {
		ssize_t nbytes = read(client_fd, buffer, sizeof buffer);
		if (nbytes == 0)
			break;
		write(client_fd, buffer, nbytes);
	}
}
