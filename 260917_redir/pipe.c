#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>

/*
 * Este programa simula:
 * $ cat file1.txt | grep aaaa
 * É invocado assim:
 * $ ./pipe
 * 
 * Referências:
 * Advanced Programming in UNIX Environment, 3rd edition, section 15.2
 */
int main(int argc, char *argv[])
{   
    int fds[2];

    if (pipe(fds) < 0) {
        perror("pipe error: ");
        exit(EXIT_FAILURE);
    }

    int ch1_pid = fork();
    if (ch1_pid == -1) {
        perror("fork error");
        exit(EXIT_FAILURE);
    }
    if (ch1_pid == 0) {
        close(1);
        dup(fds[1]);
     //   close(fds[0]);
        execlp("cat", "cat", "file1.txt", NULL);
     
        perror("exec error\n");
        exit(EXIT_FAILURE);
    }
    int ch2_pid = fork();
    if (ch2_pid == -1) {
        perror("fork error");
        exit(EXIT_FAILURE);
    }
    if (ch2_pid == 0) {
        close(0);
        dup(fds[0]);
        close(fds[1]);

        execlp("grep", "grep", "aaaa", NULL);
     
        perror("exec error\n");
        exit(EXIT_FAILURE);
    }
    close(fds[0]);
    close(fds[1]);
    printf("Pai %d, filho 1 %d filh0 2 %d\n", getpid(), ch1_pid, ch2_pid);
    int status;
    int ch_pid = wait(&status);
    if (WIFEXITED(status))
        printf("Child %d, termination status %d\n", ch_pid, WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        printf("Child %d, termination signal %d\n", ch_pid, WTERMSIG(status));

    ch_pid = wait(&status);
    if (WIFEXITED(status))
        printf("Child %d, termination status %d\n", ch_pid, WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        printf("Child %d, termination signal %d\n", ch_pid, WTERMSIG(status));
}