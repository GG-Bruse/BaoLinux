#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

using namespace std;

void handler(int signum) {
    cout << "捕捉" << signum << "号信号" << endl;
}
void* Routine(void* args) {
    sigset_t set,oset;
    sigemptyset(&set);
    sigemptyset(&oset);
    sigaddset(&set,2);
    sigprocmask(SIG_BLOCK,&set,&oset);

    signal(2,handler);
    sleep(10);

    sigprocmask(SIG_SETMASK,&oset,nullptr);
    return nullptr;
}
int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, Routine, (void *)"new thread");

    signal(2,handler);
    while(true) sleep(1);

    return 0;
}