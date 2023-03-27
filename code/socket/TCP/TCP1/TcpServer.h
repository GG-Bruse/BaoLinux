#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BACKLOG 10

using std::string;
using std::cerr;
using std::endl;


class TcpServer
{
public:
    TcpServer(uint16_t port):_socket_listen_fd(-1),_server_port(port) {}
    void InitSrever();
    ~TcpServer();
private:
    int _socket_listen_fd;
    uint16_t _server_port;
};









