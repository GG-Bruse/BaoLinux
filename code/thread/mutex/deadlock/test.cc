#include <iostream>
#include <pthread.h>
using namespace std;
void *Routine(void *pmtx)
{
    pthread_mutex_lock((pthread_mutex_t*)pmtx);
    pthread_mutex_lock((pthread_mutex_t*)pmtx);
    pthread_mutex_unlock((pthread_mutex_t*)pmtx);//无法执行
    pthread_exit(nullptr);
}
int main()
{
    pthread_mutex_t mtx;
    pthread_mutex_init(&mtx, nullptr);

    pthread_t tid;
    pthread_create(&tid, nullptr, Routine, (void *)&mtx);

    pthread_join(tid, NULL);//等待不到
	pthread_mutex_destroy(&mtx);
    return 0;
}