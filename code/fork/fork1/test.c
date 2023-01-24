#include <stdio.h>
#include <unistd.h>

int main()
{
    printf("I am running!\n");
    pid_t id = fork();
    printf("PID:%d PPID:%d\n",getpid(),getppid());
    return 0;
}

