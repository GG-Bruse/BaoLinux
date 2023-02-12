#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;

void* Routine(void* args)
{
	while (1) {
        cout << "I am " << (char*)args << endl;
		sleep(1);
	}
}
int main()
{
	pthread_t tid;
	pthread_create(&tid, NULL, Routine, (void*)"thread 1");
	while (1) {
        cout << "I am main thread!" << endl;
		sleep(2);
	}
	return 0;
}
