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

#if 0
       struct sockaddr_un {
           sa_family_t  sun_family;  /* Address family */
           char         sun_path[];  /* Socket pathname */
       };
#endif

int main()
{

	int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	
	struct sockaddr_un server_address;
	server_address.sun_family = AF_UNIX;
	strcpy(server_address.sun_path, "server_socket");
	
	int result = connect(sock_fd, (struct sockaddr*)&server_address, sizeof server_address);
	error_verify_posix(result != -1, "connect");

	char line[100];
	char response[100];
	while (true) {
		char *s = fgets(line, sizeof line, stdin);
		if (s == nullptr || strcmp(s, "exit") == 0)
			break;
		printf("sending message: %s\n", line);
		write(sock_fd, line, strlen(line) + 1);
		read(sock_fd, response, sizeof response);
		printf("received message: %s\n", response);
	}
	close(sock_fd);
}
