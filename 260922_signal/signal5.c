#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

static bool run = true;

static unsigned seconds;

void sigint_handler(int signum)
{
    printf("Signal received %d - %s\n", signum, strsignal(signum));
    run = false;
}

void sigalarm_handler(int signum)
{
    printf("Signal received: %d - %s\n", signum, strsignal(signum));
    seconds++;
    alarm(1);   // Este método de contagem do tempo tem erro cumulativo
}

int main()
{
	struct sigaction action_sigint = {0};
	action_sigint.sa_handler = sigint_handler;
    sigaction(SIGINT, &action_sigint, NULL);

    struct sigaction action_sigalrm = {0};
	action_sigalrm.sa_handler = sigalarm_handler;
    sigaction(SIGALRM, &action_sigalrm, NULL);

    printf("Premir Contol-C gera SIGINT\n\n");
    printf("Enviar signal \"kill SIG %d\"\n\n", getpid() );
    printf("Gerar signal por \"alarm(10)\" SIGALRM\n\n" );
    alarm(1);

    while (run) {
        char buffer[100];
        ssize_t nbytes = read(0, buffer, sizeof buffer);
        if (nbytes >= 0)
            write(1, buffer, nbytes);
        else
            printf("Read nbytes = %ld\n", nbytes);
    }
    printf("Terminação ordeira\n");
    printf("O processo executou durante = %d segundos\n", seconds);
}