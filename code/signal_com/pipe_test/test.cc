#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
    int fd[2] = { 0 };
    if (pipe(fd) < 0) { 
        perror("pipe");
        return 1;
    }

    pid_t id = fork();
    if (id == 0){
        close(fd[0]);
        const char* msg = "hello father, I am child...";
        int count = 10;
        while (count--){
            write(fd[1], msg, strlen(msg));
            sleep(1);
        }
        close(fd[1]);
        exit(0);
    }
    else if(id > 0) {
        close(fd[1]);
        char buff[64] = {'\0'};
        while (1){
            ssize_t s = read(fd[0], buff, sizeof(buff));
            if (s > 0){
                buff[s] = '\0';
                printf("child send to father:%s\n", buff);
            }
            else if (s == 0){
                printf("read file end\n");
                break;
            }
            else{
                printf("read error\n");
                break;
            }
        }
        close(fd[0]);
        waitpid(id, NULL, 0);
    }

    return 0;
}
