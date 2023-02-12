#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

void* Routine(void *args)
{
    cout << (char*)args << endl;
    sleep(3);
    return (void*)0;
}
int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, Routine, (void*)"new thread");
    
    void* ret = nullptr;
    int n = pthread_join(tid,&ret);
    if(n == 0) {
        cout << "等待成功" << endl;
        cout << "返回信息为: " <<(long long)ret << endl;
    }
    else {
        cout << "等待失败" << endl;
    }

    return 0;
}