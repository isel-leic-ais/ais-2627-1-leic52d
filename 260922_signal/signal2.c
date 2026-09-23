#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    printf("Premir Contol-C gera SIGINT\n\n");
    printf("Enviar signal \"kill SIG %d\"\n\n", getpid() );
    printf("Gerar signal por \"alarm(10)\" SIGALRM\n\n" );
    alarm(10);

    pause();
}