#include <stdio.h>
#include <unistd.h>
#include <string.h>S
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
    int fd[2] = {0};
    if (pipe(fd) < 0) {
        perror("pipe");
        return 1;
    }

    pid_t id = fork();
    if (id == 0) { // child
        close(fd[0]);
        const char *msg = "hello father, I am child...";
        for (int i = 0; i < 10; ++i)
        {
            write(fd[1], msg, strlen(msg));
            sleep(1);
        }
        close(fd[1]);
        exit(0);
    }
    else if (id > 0) { // father
        close(fd[1]);
        close(fd[0]); //父进程直接关闭读端（导致子进程被操作系统杀死）S
        int status = 0;
        waitpid(id, &status, 0);
        printf("child get signal:%d\n", WTERMSIG(status));
    }
    else {
        perror("fork");
        exit(0);
    }

    return 0;
}
