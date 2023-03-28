#pragma once
#include <iostream>
#include <string>
#include <memory>
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
#include "./ThreadPool/ThreadPool.hpp"
#include "./ThreadPool/Task.hpp"

#define BACKLOG 10
#define BUFF_SIZE 1024

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::unique_ptr;

class TcpServer
{
public:
    TcpServer(uint16_t port):_socket_listen_fd(-1),_server_port(port),_thread_pool(ThreadPool<Task>::GetThreadPool()) {}
    void InitServer();
    void StartUp();
    static void* HandlerClient(void*);
    static void Service(int, string, uint16_t);
    ~TcpServer();
private:
    int _socket_listen_fd;
    uint16_t _server_port;
    unique_ptr<ThreadPool<Task>> _thread_pool;
};









