#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t id = fork();
    if(id == 0)//child
    {
        for(int i = 0;i < 10;++i){
            printf("PID:%d PPID:%d\n",getpid(),getppid());
            sleep(1);
        }
        exit(0);
    }
    else if(id > 0)//father 
    {
        int status = 0;
        pid_t ret = wait(&status);
        if(ret > 0)
        {
            printf("等待成功!\n");
            if(WIFEXITED(status)){
                printf("子程序正常退出，退出码:%d\n",WEXITSTATUS(status));
            }
            else{
                printf("子程序异常退出，终止信号:%d\n",WTERMSIG(status));
            }
        }
        else{
            printf("等待失败!\n");
        }
    }
    else{ // fork error
        exit(-1);
    }

    return 0;
}
