#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define NUM 102
int main()
{
    char bar[NUM] = {0};
    memset(bar, 0, sizeof(bar));
    const char* lable = "|/-\\";
    size_t count = 0;
    size_t i = 0;
    for(;i<=100;++i){
        printf("[%-100s][%d%%]%c\r",bar,count,lable[count%4]);
        bar[count++] = '#';
        fflush(stdout);
        usleep(20000);
    }
    printf("\n");
    return 0;
}
