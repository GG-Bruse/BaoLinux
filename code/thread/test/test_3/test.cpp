#include <iostream>
#include <string>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
class Sem{
public:
	Sem(int num) { sem_init(&_sem, 0, num); }
	~Sem() { sem_destroy(&_sem); }
	void P() { sem_wait(&_sem); }
	void V() { sem_post(&_sem); }
private:
	sem_t _sem;
};
static Sem sem(1);
int tickets = 2000;
void* TicketGrabbing(void* arg)
{
	std::string name = (char*)arg;
	while (true){
        sem.P();
		if (tickets > 0){
			usleep(1000);
			std::cout << name << " get a ticket, tickets left: " << --tickets << std::endl;
            sem.V();
		}
		else{
            sem.V();
			break;
		}
	}
	std::cout << name << " quit..." << std::endl;
	pthread_exit((void*)0);
}
int main()
{
	pthread_t tid[5];
	pthread_create(tid, nullptr, TicketGrabbing, (void*)"thread 1");
	pthread_create(tid + 1, nullptr, TicketGrabbing, (void*)"thread 2");
	pthread_create(tid + 2, nullptr, TicketGrabbing, (void*)"thread 3");
	pthread_create(tid + 3, nullptr, TicketGrabbing, (void*)"thread 4");
	pthread_create(tid + 4, nullptr, TicketGrabbing, (void*)"thread 5");
	
	pthread_join(tid[0], nullptr);
	pthread_join(tid[1], nullptr);
	pthread_join(tid[2], nullptr);
	pthread_join(tid[3], nullptr);
	pthread_join(tid[4], nullptr);
	return 0;
}
