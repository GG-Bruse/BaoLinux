#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main()
{
    int fd = open("./log.txt",O_RDONLY);
    if(fd < 0){
        perror("open:");
        exit(1);
    }

    char buf[64] = {0};
    while(read(fd, buf, 63)){
        printf("%s",buf);
        memset(buf,0x00,64);   
    }

    close(fd);
    return 0;
}
