#include <stdio.h>
#include <stdlib.h>

int un_val;
int init_val = 100;

int main(int argc,char* argv[],char* env[])
{
    int i = 0;
    int count = 0;
    while(env[i] != NULL && count < 5){
        printf("环境变量地址: %p\n",env[i]);
        ++count;
    }

    for(int i = 0;i < argc; ++i){
        printf("命令行参数地址: %p\n",argv[i]);
    }

    char* p1 = (char*)malloc(10);
    char* p2 = (char*)malloc(10);
    char* p3 = (char*)malloc(10);

    printf("栈区地址: %p\n",&p3);
    printf("栈区地址: %p\n",&p2);
    printf("栈区地址: %p\n",&p1);

    printf("堆区地址: %p\n",p3);
    printf("堆区地址: %p\n",p2);
    printf("堆区地址: %p\n",p1);

    printf("未初始化数据区: %p\n",&un_val);
    printf("初始化数据区: %p\n",&init_val);

    printf("代码区: %p\n",main);
    return 0;
}
