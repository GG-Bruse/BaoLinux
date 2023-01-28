#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main()
{
    close(0);
    close(2);
    int fd1 = open("./log1.txt",O_RDWR | O_CREAT | O_TRUNC,0666);
    int fd2 = open("./log2.txt",O_RDWR | O_CREAT | O_TRUNC,0666);
    int fd3 = open("./log3.txt",O_RDWR | O_CREAT | O_TRUNC,0666);
    int fd4 = open("./log4.txt",O_RDWR | O_CREAT | O_TRUNC,0666);
    int fd5 = open("./log5.txt",O_RDWR | O_CREAT | O_TRUNC,0666);
    printf("fd1: %d\n",fd1);
    printf("fd2: %d\n",fd2);
    printf("fd3: %d\n",fd3);
    printf("fd4: %d\n",fd4);
    printf("fd5: %d\n",fd5);
    close(fd1);
    close(fd2);
    close(fd3);
    close(fd4);
    close(fd5);
    return 0;
}
