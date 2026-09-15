#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdlib.h>

int main()
{   int pid;
    if ((pid = fork()) != 0) {
        printf("Pai %d, filho %d\n", getpid(), pid);
 //       sleep(30);
        exit(44);
        int status;
        wait(&status);
        printf("status %04x\n", status);
        if (WIFEXITED(status))
            printf("Terminação normal %d\n", WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            printf("Terminação anormal %d\n", WTERMSIG(status));
    }
    else {
        printf("Filho %d, pai %d\n", getpid(), getppid());
//        *(int *)0 = 0;
//        while (true)
//            ;
        sleep(20);
        exit(33);
    }
    printf("THE END\n");
}