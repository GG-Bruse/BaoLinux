#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
using namespace std;
int main()
{
	if (fork() == 0) {//child
		std::cout << "I am running..." << std::endl;
		int *p = nullptr;
		*p = 100;//野指针
		exit(0);
	}
	//father
	int status = 0;
	waitpid(-1, &status, 0);
    std::cout << "exit_code:" << WEXITSTATUS(status) << std::endl;
    std::cout << "core_dump:" << ((status >> 7) & 1) << std::endl;
    std::cout << "trem_signal:" << WTERMSIG(status) << std::endl;
	
	return 0;
}
