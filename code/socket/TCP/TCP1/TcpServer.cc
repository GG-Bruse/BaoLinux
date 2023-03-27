#include "TcpServer.h"

void TcpServer::InitSrever()
{
    //创建套接字
    _socket_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(_socket_listen_fd < 0) {
        cerr << "socket fail" << endl;
        exit(1);
    }
    //绑定
    struct sockaddr_in local;
    memset(&local, '\0', sizeof local);
    local.sin_family = AF_INET;
    local.sin_port = htons(_server_port);
    local.sin_addr.s_addr = INADDR_ANY;
    if(bind(_socket_listen_fd, (struct sockaddr*)&local, sizeof local) < 0) {
        cerr << "bind fail" << endl;
        exit(2);
    }
    //设置服务器监听状态
    if(listen(_socket_listen_fd, BACKLOG) < 0) {
        cerr << "listen fail" << endl;
        exit(3);
    }
}

TcpServer::~TcpServer() { if(_socket_listen_fd >= 0) close(_socket_listen_fd); }





