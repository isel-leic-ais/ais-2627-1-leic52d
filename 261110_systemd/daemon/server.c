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
	int child_pid = fork();

	if (child_pid == 0) {

		openlog("echod", LOG_PID, LOG_DAEMON);

		char pidfile[100];
		sprintf(pidfile, "echo %d > %sechod.pid", getpid(), ECHO_DIRECTORY);
		int result = system(pidfile);
		if (result == -1) {
			syslog(LOG_ERR, "Error creating PID file");
		}

		struct sigaction action_sigterm = {0};
		action_sigterm.sa_handler = sigterm_handler;
		sigaction(SIGTERM, &action_sigterm, NULL);

		int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
		if (server_fd == -1) {
			syslog(LOG_ERR, "Error in socket(AF_UNIX, )");
			exit(EXIT_FAILURE);
		}

		struct sockaddr_un server_addr;
		memset(&server_addr, 0, sizeof server_addr);
		server_addr.sun_family = AF_UNIX;
		strncpy(server_addr.sun_path, ECHO_SERVICE_NAME, sizeof server_addr.sun_path - 1);
		result = bind(server_fd, (struct sockaddr *)&server_addr, sizeof server_addr);
		if (result == -1) {
			syslog(LOG_ERR, "Error in bind(server_fd, %s)", ECHO_SERVICE_NAME);
			exit(EXIT_FAILURE);
		}

		result = listen(server_fd, 10);
		if (result == -1) {
			syslog(LOG_ERR, "Error in iste(server_fd, 10)");
			exit(EXIT_FAILURE);
		}

		syslog(LOG_INFO, "server started");

		while (!terminated) {

			int client_fd = accept(server_fd, nullptr, nullptr);
			if (terminated)
				break;
			if (client_fd == -1) {
				syslog(LOG_ERR, "Error in accept(server_fd, )");
				exit(EXIT_FAILURE);
			}

			syslog(LOG_INFO, "new connection");

			thrd_t thread;
			int result = thrd_create(&thread, process_connection, (void*)(intptr_t)client_fd);
			if (result != thrd_success) {
				syslog(LOG_ERR, "Error in thrd_create())");
				exit(EXIT_FAILURE);
			}
			thrd_detach(thread);
		}
		syslog(LOG_INFO, "server terminated");
		close (server_fd);
		closelog();
	}
	else if (child_pid == -1) {
		fprintf(stderr, "Error creating server echod process");
		return EXIT_FAILURE;
	}
}



