#include "TcpClient.h"

void TcpClient::InitClient()
{
    //创建套接字
    _socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(_socket_fd < 0) {
        cerr << "socket fail" << endl;
        exit(1);
    }
}

void TcpClient::StartUp()
{
    //发起连接
    struct sockaddr_in server;
    memset(&server, '\0', sizeof server);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(_server_ip.c_str());
    server.sin_port = htons(_server_port);
    if(connect(_socket_fd, (struct sockaddr*)&server, sizeof(server)) == 0) {
        cout << "connect success" << endl;
        Request(); //发起请求
    }
    else {
        cerr << "connect fail" << endl;
        exit(2);
    }
}

void TcpClient::Request()
{
    char buffer[BUFF_SIZE];
    string message;
    while(true) 
    {
        cout << "Pleses Enter#";
        getline(cin, message);
        send(_socket_fd, message.c_str(), message.size(), 0);
        ssize_t size = recv(_socket_fd, buffer, sizeof(buffer) - 1, 0);
        if (size > 0){
			buffer[size] = '\0';
			cout << "server echo# " << buffer << endl;
		}
		else if (size == 0) {
			cout << "server close!" << endl;
			break;
		}
		else {
			cerr << "read error!" << endl;
			break;
		}
    }
}

TcpClient::~TcpClient() { if(_socket_fd >= 0) close(_socket_fd); }




