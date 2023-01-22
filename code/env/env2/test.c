#include <stdio.h>
int main()
{
    extern char** environ;
    for(int i = 0;environ[i] != NULL; ++i){
        printf("%s\n",environ[i]);
    }
    return 0;
}
