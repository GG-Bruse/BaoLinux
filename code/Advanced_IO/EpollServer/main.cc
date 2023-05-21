#include "EpollServer.hpp"
#include <memory>
using namespace ns_epoll;
using namespace std;

void Change(string request) {
    cout << "change:" << request << endl;
}

int main()
{
    unique_ptr<EpollServer> epollServer(new EpollServer(Change, 9090));
    epollServer->Start();
    return 0;
}