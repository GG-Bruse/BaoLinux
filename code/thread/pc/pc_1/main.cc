#include <unistd.h>
#include "BlockQueue.hpp"

void* Producer(void* arg)
{
    BlockQueue<int>* bq = (BlockQueue<int>*)arg;
	
	while (true) { //生产者不断进行生产
		sleep(1);
		int data = rand() % 100 + 1;
		bq->push(data);
	}
}
void* Consumer(void* arg)
{
    int data = 0;
    BlockQueue<int>* bq = (BlockQueue<int>*)arg;
	while (true) { //消费者不断进行消费
		sleep(1);
		bq->pop(data);
	}
}

int main() 
{
    pthread_t producer,consumer;
    BlockQueue<int>* bq = new BlockQueue<int>;

    pthread_create(&producer,nullptr,Producer,(void*)bq);
    pthread_create(&consumer,nullptr,Consumer,(void*)bq);

    pthread_join(producer,nullptr);
    pthread_join(consumer,nullptr);
    delete bq;

    return 0;
}