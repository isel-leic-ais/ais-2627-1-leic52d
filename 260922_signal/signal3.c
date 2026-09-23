#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int main()
{
	signal(SIGALRM, SIG_IGN);
	signal(SIGINT,  SIG_IGN);

    if (signal(SIGSEGV, SIG_IGN) == SIG_ERR) { // Verifica-se que o sistema não ignora SIGSEGV
        perror("Error in signal");
        exit(EXIT_FAILURE);
    }

    printf("Premir Contol-C gera SIGINT\n\n");
    printf("Enviar signal \"kill SIG %d\"\n\n", getpid() );
    printf("Gerar signal por \"alarm(10)\" SIGALRM\n\n" );
    alarm(10);

#if  0
    char *msg = "Bom dia";
    char *first = strtok(msg, " ");
    printf("First token= %s\n", first);
#endif

    pause();
}