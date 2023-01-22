#include <stdio.h>
#include <unistd.h>
int main()
{
    pid_t id = fork();
    printf("%d\n",&id);
    return 0;
}
