#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <syslog.h>
#include <signal.h>
#include <threads.h>

#include "echo_service.h"

static volatile bool terminated = false;

static void sigterm_handler(int signum)
{
	printf("Signal \"%d - %s\" handled\n", signum, strsignal(signum));
	terminated = true;
}

static int process_connection(void *param)
{
	int client_fd = (intptr_t) param;

	char message[MESSAGE_SIZE_MAX];
	ssize_t nread = read(client_fd, message, sizeof message);
	while (nread == MESSAGE_SIZE_MAX) {
		write(client_fd, message, nread);
		nread = read(client_fd, message, sizeof message);
	}
	close(client_fd);
	return 0;
}

int main()
{
	char pidfile[100];
	sprintf(pidfile, "echo %d > %sechod.pid", getpid(), ECHO_DIRECTORY);
	int result = system(pidfile);
	if (result == -1) {
		fprintf(stderr, "%s: Error creating PID file\n", strerror(errno));
	}

	struct sigaction action_sigterm = {0};
	action_sigterm.sa_handler = sigterm_handler;
	sigaction(SIGTERM, &action_sigterm, NULL);


	result = unlink(ECHO_SERVICE_NAME);
	if (result == -1 && errno != ENOENT) {
		fprintf(stderr, "%s: Error unlink old socket name\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (server_fd == -1) {
		fprintf(stderr, "%s: Error creating socket\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	struct sockaddr_un server_addr;
	memset(&server_addr, 0, sizeof server_addr);
	server_addr.sun_family = AF_UNIX;
	strncpy(server_addr.sun_path, ECHO_SERVICE_NAME, sizeof server_addr.sun_path - 1);
	result = bind(server_fd, (struct sockaddr *)&server_addr, sizeof server_addr);
	if (result == -1) {
		fprintf(stderr, "%s: Error in binding socket\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	result = listen(server_fd, 10);
	if (result == -1) {
		fprintf(stderr, "%s: Error setting listen queue\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	puts("server started");

	while (!terminated) {

		int client_fd = accept(server_fd, nullptr, nullptr);
		if (terminated)
			break;
		if (client_fd == -1) {
			fprintf(stderr, "%s: Error on accept connection\n", strerror(errno));
			exit(EXIT_FAILURE);
		}

		puts("new connection");

		thrd_t thread;
		int result = thrd_create(&thread, process_connection, (void*)(intptr_t)client_fd);
		if (result != thrd_success) {
			fprintf(stderr, "%s: Error on thrd_create()\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		thrd_detach(thread);
	}
	puts("server stopped");
	close (server_fd);
}



