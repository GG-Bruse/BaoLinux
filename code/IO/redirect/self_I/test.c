#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main()
{
    close(0);
    int fd = open("./log.txt",O_RDONLY);
    if(fd < 0){
        perror("opern error:");
        return 1;
    }
    char buf[64] = {0};
    while(scanf("%s",buf) != EOF){
        printf("%s\n",buf);
    }
    return 0;
}
