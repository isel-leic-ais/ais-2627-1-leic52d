#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    printf("Premir Control-C gera SIGINT\n\n");
    printf("Enviar signal: \"kill SIG %d\"\n\n", getpid() );
    pause();
}