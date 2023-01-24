#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
    pid_t id = fork();
    if (id == 0)//child
    {
        for(int i = 0;i < 3; ++i){
            printf("child do something...PID:%d, PPID:%d\n", getpid(), getppid());
            sleep(3);
        }
        exit(0);
    }
    else if(id > 0)//father
    {
        while (1)
        {
            int status = 0;
            pid_t ret = waitpid(id, &status, WNOHANG);
            if (ret > 0)
            {
                printf("wait child success...\n");
                if (WIFEXITED(status)){
                    printf("exit code:%d\n", WEXITSTATUS(status));
                }
                else{
                    printf("killed by signal %d\n", WTERMSIG(status));
                }
                break;
            }
            else if (ret == 0){
                printf("father do other things...\n");
                sleep(1);
            }
            else{
                printf("waitpid error...\n");
                break;
            }
        }
    }
    else
    {
        printf("fork error\n");
    }
    return 0;
}

