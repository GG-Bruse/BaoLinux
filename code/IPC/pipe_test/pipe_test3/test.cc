#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd[2] = { 0 };
	if (pipe2(fd , O_NONBLOCK) < 0) {
		perror("pipe");
		return 1;
	}

	pid_t id = fork();
	if (id == 0) { //child 
		close(fd[0]); //子进程关闭读端
		char c = 'a';
		int count = 0;
		while (true) {
            int num = write(fd[1], &c, 1);
            if(num == -1) break;
			count++;
		}
        printf("%d\n", count); //打印当前写入的字节数
		close(fd[1]);
		exit(0);
	}
    else if(id > 0) {//father
        close(fd[1]);
	    waitpid(id, NULL, 0);
	    close(fd[0]);
    }
    else {
        perror("fork");
        exit(0);
    }

	return 0;
}
