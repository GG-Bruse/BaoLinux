#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
	pid_t ids[10] = {0};
	for (int i = 0; i < 10; i++)
    {
		pid_t id = fork();
		if (id == 0)//child
        {
			printf("child process created successfully...PID:%d\n", getpid());
			sleep(3);
			exit(i); //将子进程的退出码设置为该子进程PID在数组ids中的下标
		}
        else if(id > 0)//father
        {
		    ids[i] = id;
        }
        else{ //fork error
            exit(-1);
        }
	}
	for (int i = 0; i < 10; i++)
    {
		int status = 0;
		pid_t ret = waitpid(ids[i], &status, 0);
		if (ret > 0){
			//wait child success
			printf("wait child success..PID:%d\n", ids[i]);
			if (WIFEXITED(status)){
				printf("exit code:%d\n", WEXITSTATUS(status));
			}
			else{
				printf("killed by signal %d\n", WTERMSIG(status));
			}
		}
        else{
            printf("wait child error\n");
        }
	}
	return 0;
}

