#include "TcpServer.hpp"
#include <memory>
using std::unique_ptr;

int main()
{
    unique_ptr<TcpServer> tcpSvrPtr(new TcpServer());
    tcpSvrPtr->Dispather();
    return 0;
}