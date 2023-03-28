#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BACKLOG 10
#define BUFF_SIZE 1024

using std::string;
using std::cout;
using std::cerr;
using std::endl;


class TcpServer
{
public:
    TcpServer(uint16_t port):_socket_listen_fd(-1),_server_port(port) {}
    void InitServer();
    void StartUp();
    void Service(int, string, uint16_t);
    ~TcpServer();
private:
    int _socket_listen_fd;
    uint16_t _server_port;
};









