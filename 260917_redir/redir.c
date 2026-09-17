#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
/*
    Este programa simula a execução do comando:
    $ cat file1.txt > file2.txt
    É invocado da seguite forma:
    redir file2.txt
*/
int main(int argc, char *argv[])
{  
    int fd = open(argv[1], O_WRONLY | O_CREAT, 0644);
    if (fd < 0) {
        perror("open error: ");
        exit(EXIT_FAILURE);
    }
    int pid;
    if ((pid = fork()) != 0) {
        printf("Pai %d, filho %d\n", getpid(), pid);
        int status;
        wait(&status);
        if (WIFEXITED(status))
            printf("Terminação normal %d\n", WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            printf("Terminação anormal %d\n", WTERMSIG(status));
    }
    else {
        close(1);   //  dup2(fd, 1) é equivalente a close(1); dup(fd);
        dup(fd);
        close(fd);
        execlp("cat", "cat", "file1.txt", NULL);
     
        perror("exec error: \n");
    }
}