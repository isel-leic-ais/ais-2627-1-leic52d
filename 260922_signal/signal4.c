#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

static bool run = true;

void sigint_handler(int signum)
{
    printf("Signal received: %d - %s\n", signum, strsignal(signum));
    run = false;
}


void sigalarm_handler(int signum)
{
    printf("Signal received: %d - %s\n", signum, strsignal(signum));
}

void sigsegv_handler(int signum)
{
    printf("Signal received: %d - %s\n", signum, strsignal(signum));
}

int main()
{
	struct sigaction action_sigint = {0};
	action_sigint.sa_handler = sigint_handler;
    sigaction(SIGINT, &action_sigint, NULL);

    struct sigaction action_sigalrm = {0};
	action_sigalrm.sa_handler = sigalarm_handler;
    sigaction(SIGALRM, &action_sigalrm, NULL);

    struct sigaction action_sigsegv = {0};
	action_sigsegv.sa_handler = sigsegv_handler;
    sigaction(SIGSEGV, &action_sigsegv, NULL);

    printf("Premir Contol-C gera SIGINT\n\n");
    printf("Enviar signal \"kill SIG %d\"\n\n", getpid() );
    printf("Gerar signal por \"alarm(10)\" SIGALRM\n\n" );
    alarm(10);

#if  1
    char *msg = "Bom dia";
    char *first = strtok(msg, " ");
    printf("First token= %s\n", first);
#endif

    while (true) {   //  Evita terminar o processo de cada vez que um signal é atendido
        int result = pause();
        fprintf(stderr, "pause() returned %d", result);
        perror("");
    }
}