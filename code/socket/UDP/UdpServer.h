#pragma once
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

#define SIZE 128

using std::cout;
using std::cerr;
using std::endl;
using std::string;

class UdpServer
{
public:
    UdpServer(string ip, uint16_t port):_socket_fd(-1),_ip(ip),_port(port) {}
    bool InitServer();
    void Start();
    ~UdpServer();
private:
    int _socket_fd;
    string _ip;
    uint16_t _port;
};








