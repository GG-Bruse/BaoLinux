#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int val = 100;
int main()
{
    pid_t id = fork();
    if(id < 0)//err
    {
        exit(-1);
    }
    else if(id > 0)//father
    {
        sleep(3);
        printf("PID:%d PPID:%d val:%d &val:%p\n",getpid(),getppid(),val,&val);
    }
    else//id == 0
    {
        val = 200;
        printf("PID:%d PPID:%d val:%d &val:%p\n",getpid(),getppid(),val,&val);
    }
    return 0;
}
