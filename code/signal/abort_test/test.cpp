#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <unistd.h>
using namespace std;

void handler(int signum) {
    cout << "get a signal : " << signum << endl;
}
int main()
{
    signal(SIGABRT,handler);

    while(true) {
        sleep(1);
        abort();
    }
    return 0;
}