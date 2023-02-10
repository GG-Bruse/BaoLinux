#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
using namespace std;

int main()
{
	signal(SIGCHLD, SIG_IGN);
	if (fork() == 0) { //child
		cout << "child is running, PID: " << getpid() << endl;
		sleep(3);
		exit(1);
	}
	//father
	while (1);

	return 0;
}
