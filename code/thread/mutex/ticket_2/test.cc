#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
using namespace std;

const int thread_num = 4;
int tickets = 1000;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void* GetTickets(void* args) {
	while (true) {
        pthread_mutex_lock(&mtx);
		if (tickets > 0) {
			usleep(1000);//抢票所耗费的时间
            printf("[%s] get a ticket, left: %d\n", (char*)args, --tickets);
            pthread_mutex_unlock(&mtx);
            usleep(10);
		}
		else {
            pthread_mutex_unlock(&mtx);
			break;
		}
	}
	printf("%s quit!\n", (char*)args);
	pthread_exit((void*)0);
}
int main()
{
    pthread_t tids[thread_num];
    pthread_create(tids, nullptr, GetTickets, (void*)"thread 1");
    pthread_create(tids + 1, nullptr, GetTickets, (void*)"thread 2");
	pthread_create(tids + 2, nullptr, GetTickets, (void*)"thread 3");
	pthread_create(tids + 3, nullptr, GetTickets, (void*)"thread 4");

    for(int i = 0;i < thread_num; ++i) {
        pthread_join(tids[i], nullptr);
    }
    return 0;
}