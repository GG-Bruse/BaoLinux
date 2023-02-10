#include <iostream>
#include <cstdlib>
#include <vector>
#include <sys/types.h>
#include <signal.h>

void Usage(const char* str) {
    std::cout << "Usage : " << str << "pid signo" << std::endl;   
}
int main(int argc, char* argv[]) 
{
    if(argc < 3) {
        Usage(argv[0]);
    }

    int signum = atoi(argv[1]);

    std::vector<pid_t> v;
    for(int i = 2; i < argc; ++i) {
        v.push_back(atoi(argv[i]));
    }

    for(auto &e : v) {
        kill(e, signum);
    }
    return 0;
}