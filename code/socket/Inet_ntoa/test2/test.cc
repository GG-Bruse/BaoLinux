#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>

void* Func1(void* arg)
{
	struct sockaddr_in* p = (struct sockaddr_in*)arg;
	while (1){
		char* ptr1 = inet_ntoa(p->sin_addr);
		std::cout << "ptr1: " << ptr1 << std::endl;
		sleep(1);
	}
}
void* Func2(void* arg)
{
	struct sockaddr_in* p = (struct sockaddr_in*)arg;
	while (1){
		char* ptr2 = inet_ntoa(p->sin_addr);
		std::cout << "ptr2: " << ptr2 << std::endl;
        sleep(1);
	}
}
int main()
{
	struct sockaddr_in addr1;
	struct sockaddr_in addr2;
	addr1.sin_addr.s_addr = 0;
	addr2.sin_addr.s_addr = 0xffffffff;
	
	pthread_t tid1 = 0;
	pthread_create(&tid1, nullptr, Func1, &addr1);
    sleep(1);
	pthread_t tid2 = 0;
	pthread_create(&tid2, nullptr, Func2, &addr2);
	
	pthread_join(tid1, nullptr);
	pthread_join(tid2, nullptr);
	return 0;
}
