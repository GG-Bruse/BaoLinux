#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFF_SIZE 1024

using std::string;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;


class TcpClient
{
public:
    TcpClient(string ip, uint16_t port):_socket_fd(-1),_server_ip(ip),_server_port(port) {}
    void InitClient();
    void StartUp();
    void Request();
    ~TcpClient();
private:
    int _socket_fd;
    string _server_ip;
    uint16_t _server_port;
};
