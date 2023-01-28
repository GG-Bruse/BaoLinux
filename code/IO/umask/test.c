#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main()
{
    umask(0000);
    int fd = open("./log.txt",O_RDWR | O_CREAT | O_TRUNC,0666);
    close(fd);
    return 0;
}
