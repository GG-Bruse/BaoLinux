#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main()
{
    int fd = open("./log.txt",O_RDWR | O_CREAT | O_TRUNC,0666);
    if(fd < 0){
        perror("open:");
        exit(1);
    }

    const char* buf = "hello Linux!\n";
    write(fd, buf, strlen(buf));
    close(fd);
    return 0;
}
