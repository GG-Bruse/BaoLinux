#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main()
{
    close(1);
    int fd = open("./log.txt",O_RDWR | O_CREAT | O_TRUNC, 0666);
    if(fd < 0){
        perror("opern error:");
        return 1;
    }
    printf("hello world\n");
	fflush(stdout);
    return 0;
}
