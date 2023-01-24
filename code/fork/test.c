#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t id = fork();
    if(id < 0)
    {
        printf("fork error!!!\n");
        return 0;
    }
    else if(id == 0)
    {
        printf("子进程 PID:%d PPID:%d\n",getpid(),getppid());
    }
    else//id > 0 
    {
        printf("父进程 PID:%d PPID:%d\n",getpid(),getppid());
        wait(NULL);
    }
    return 0;
}

