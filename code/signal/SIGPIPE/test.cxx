#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

int main()
{
    //使用pipe创建匿名管道
	int fd[2] = { 0 };
	if (pipe(fd) < 0) {
		perror("pipe");
		exit(1);
	}

	pid_t id = fork();
	if (id == 0) { //child
		close(fd[0]);
		const char* msg = "hello father, I am child...";
        for(int i = 0;i < 10; ++i) {
            write(fd[1], msg, strlen(msg));
			sleep(1);
        }
		close(fd[1]);
		exit(0);
	}
    else if(id > 0) { //father
        close(fd[1]);
	    close(fd[0]);
	    int status = 0;
	    waitpid(id, &status, 0);
	    cout << "child get signal:" << WTERMSIG(status) << endl; //打印子进程收到的信号
    }
    else {
        perror("fork error");
        exit(1);
    }
	
	return 0;
}
