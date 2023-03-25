#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SIZE 128

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;

class UdpClient
{
public:
    UdpClient(string server_ip,uint16_t server_port):_server_ip(server_ip),_server_port(server_port) {}
    bool InitClient();
    void Start();
    ~UdpClient();
private:
    int _socket_fd;
    string _server_ip;
    uint16_t _server_port;
};



