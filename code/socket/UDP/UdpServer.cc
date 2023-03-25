#include "UdpServer.h"

bool UdpServer::InitServer() {
    //创建套接字
    _socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(_socket_fd < 0) {
        cerr << "socket fail" << endl;
        return false;
    }
    cout << "socket create success , fd:" << _socket_fd << endl;

    //填充网络通信相关信息
    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(_port);
    local.sin_addr.s_addr = INADDR_ANY;
    //绑定
    if(bind(_socket_fd, (struct sockaddr*)&local, sizeof(local)) < 0) {
        cerr << "bind fail" << endl;
        return false;
    }
    cout << "bind success" << endl;

    return true;
}

void UdpServer::Start()
{
    char buffer[SIZE];
    while(true) 
    {
        struct sockaddr_in ping;
        socklen_t length =  sizeof(ping);
        ssize_t size = recvfrom(_socket_fd, buffer, SIZE - 1, 0, (struct sockaddr*)&ping, &length);
        if(size > 0) {
            buffer[size] = '\0';
            uint16_t port = ntohs(ping.sin_port);
            string ip = inet_ntoa(ping.sin_addr);
            cout << "[" << ip << ":" << port << "]#" << buffer << endl;
        }
        else {
            cerr << "recvfrom fail" << endl;
        }

        string echo_message = "server echo:";
		echo_message += buffer;
		sendto(_socket_fd, echo_message.c_str(), echo_message.size(), 0, (struct sockaddr*)&ping, length);
    } 
}


UdpServer::~UdpServer() {
    if(_socket_fd > 0) close(_socket_fd);
}






