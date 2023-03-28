#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
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

class ThreadDate
{
public:
    ThreadDate(int fd, string ip,uint16_t port):_server_socket_fd(fd),_client_ip(ip),_client_port(port) {}
    ~ThreadDate() {}
public:
    int _server_socket_fd;//accept获取连接得到文件描述符，用于服务
    string _client_ip;
    uint16_t _client_port;
};

class TcpServer
{
public:
    TcpServer(uint16_t port):_socket_listen_fd(-1),_server_port(port) {}
    void InitServer();
    void StartUp();
    static void* HandlerClient(void*);
    static void Service(int, string, uint16_t);
    ~TcpServer();
private:
    int _socket_listen_fd;
    uint16_t _server_port;
};









