#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

void *Routine(void *args)
{
    printf("%s : %lld : %p\n", args, pthread_self(), pthread_self());
    sleep(3);
    return (void *)0;
}
int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, Routine, (void *)"new thread");
    sleep(3);
    printf("main thread : %lld : %p\n", pthread_self(), pthread_self());
    pthread_join(tid, nullptr);
    return 0;
}