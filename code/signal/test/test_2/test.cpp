#include <iostream>
#include <signal.h>
#include <unistd.h>
void handler(int signum) {
    std::cout << "get a signal : " << signum << std::endl;
}
int main()
{
    signal(2, handler);
    while (1) {
		std::cout << "hello signal!" << std::endl;
		sleep(1);
	}
    return 0;
}