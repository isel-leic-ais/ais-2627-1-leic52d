#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

    write(1, "aaaaaaaaa\n", 10);        // Escreve na consola

    int fd = open(argv[1], O_WRONLY | O_CREAT, 0644);   // Criar ou abrir um ficheiro regular

    printf("fd = %d\n", fd);

    write(fd, "aaaaaaaaa\n", 10);       // Escrever no ficheiro que acabou de ser aberto

    close(1);           //  Fechar stdout. A entrada 1 fica livre

    dup(fd);            //  Duplica a entrada fd (3) na entrada 1. Ambas as entradas correspondem ao ficheiro

    write(1, "bbbbbbbbb\n", 10);    //  Esta escrita concretiza-se sobre o ficheiro
}