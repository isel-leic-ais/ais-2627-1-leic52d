#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>

/*  Este programa simula o comando
 *  $ command < input > output
 *  É invocado assim: 
 *  redir   command input   output
 *  argv[0] argv[1] argv[2] argv[3]
*/
int main(int argc, char *argv[])
{   
    int fdi = open(argv[2], O_RDONLY);
    if (fdi < 0) {
        perror("open error: ");
        exit(EXIT_FAILURE);
    }

    int fdo = open(argv[3], O_WRONLY | O_CREAT, 0644);
    if (fdo < 0) {
        perror("open error: ");
        exit(EXIT_FAILURE);
    }

    int pid;
    if ((pid = fork()) != 0) {
        close(fdi);
        close(fdo);
        printf("Pai %d, filho %d\n", getpid(), pid);
        int status;
        wait(&status);
        if (WIFEXITED(status))
            printf("Terminação normal %d\n", WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            printf("Terminação anormal %d\n", WTERMSIG(status));
    }
    else {
        dup2(fdi, 0);
        dup2(fdo, 1);
        close(fdi);
        close(fdo);
        execlp(argv[1], argv[1], NULL);
     
        perror("exec error: ");
    }
}