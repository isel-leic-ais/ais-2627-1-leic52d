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

#include <systemd/sd-daemon.h>

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

	int nfds = sd_listen_fds(0);
	if (nfds != 1) {
		fprintf(stderr, "Wrong number of received fds from systemd");
		return EXIT_FAILURE;
	}

	struct sigaction action_sigterm = {0};
	action_sigterm.sa_handler = sigterm_handler;
	sigaction(SIGTERM, &action_sigterm, NULL);

	int server_fd = SD_LISTEN_FDS_START;

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



