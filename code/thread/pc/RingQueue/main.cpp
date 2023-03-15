#include "ringQueue.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <cstdlib>
#include <ctime>
void* Producer(void* arg)
{
    RingQueue<int>* rq = (RingQueue<int>*)arg;
    while(true)
    {
        //构建数据或任务对象(一般从外部获取)
        int date = rand() % 100 + 1;
        //Push入环形队列
        rq->Push(date);
        std::cout << "生产:" << date << "[" << pthread_self() << "]" <<std::endl;
    }

}
void* Consumer(void* arg)
{
    RingQueue<int>* rq = (RingQueue<int>*)arg;
    while(true)
    {
        sleep(1);
        //从环形队列中读取数据或任务
        int date = 0;
        rq->Pop(&date);
        //处理数据或任务
        std::cout << "消费:" << date << "[" << pthread_self() << "]" <<std::endl;
    }

}
int main()
{
    srand((unsigned)time(nullptr) * getpid());
    RingQueue<int>* rq = new RingQueue<int>();
    // rq->Debug();
    pthread_t producer[3],consumer[2];
    pthread_create(producer,nullptr,Producer,rq);
    pthread_create(producer + 1,nullptr,Producer,rq);
    pthread_create(producer + 2,nullptr,Producer,rq);
    pthread_create(consumer,nullptr,Consumer,rq);
    pthread_create(consumer + 1,nullptr,Consumer,rq);

    for(int i = 0;i < 3; ++i) pthread_join(producer[i],nullptr);
    for(int i = 0;i < 2; ++i) pthread_join(consumer[i],nullptr);
    return 0;
}

