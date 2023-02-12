#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>
using namespace std;

int g_val = 100;
int main()
{
	pid_t id = vfork();
	if (id == 0) { //child
		g_val = 200;
		cout << "child:PID:" << getpid() << " PPID:"<< getppid() << " g_val:" <<  g_val << endl;
    	exit(0);
	}
	//father
	sleep(3);
	cout << "father:PID:" << getpid() << " g_val:" <<  g_val << endl;
	return 0;
}
