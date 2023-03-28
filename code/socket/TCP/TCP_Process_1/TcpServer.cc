#include "TcpServer.h"

void TcpServer::InitServer()
{
    //创建套接字
    _socket_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(_socket_listen_fd < 0) {
        cerr << "socket fail" << endl;
        exit(1);
    }
    cout << "socket success" << endl;
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
    cout << "bind success" << endl;
    //设置服务器监听状态
    if(listen(_socket_listen_fd, BACKLOG) < 0) {
        cerr << "listen fail" << endl;
        exit(3);
    }
    cout << "listen success" << endl;
}

void TcpServer::StartUp()
{
    //设置忽略SIGCHLD信号
    signal(SIGCHLD, SIG_IGN);
    while(true) 
    {
        //获取连接
        struct sockaddr_in foreign;
        memset(&foreign, '\0', sizeof foreign);
        socklen_t length = sizeof foreign;
        int server_socket_fd = accept(_socket_listen_fd, (struct sockaddr*)&foreign, &length);
        if(server_socket_fd < 0) {
            cerr << "accept fail" << endl;
            continue;
        }
        string client_ip = inet_ntoa(foreign.sin_addr);
        uint16_t client_port = ntohs(foreign.sin_port);
        cout << "New Link: [" << server_socket_fd << "] [" <<  client_ip << "] [" << client_port << "]" << endl;
        //处理客户端请求
        pid_t id = fork();
        if(id == 0) { //child
            Service(server_socket_fd, client_ip,client_port);
            exit(4);
        }
    }
}

void TcpServer::Service(int server_socket_fd, string client_ip, uint16_t client_port)
{
    char buffer[BUFF_SIZE];
    while(true) 
    {
        ssize_t size = read(server_socket_fd, buffer, sizeof(buffer) - 1);
        if(size > 0) //读取成功
        {
            buffer[size] = '\0';
            cout << client_ip << ":" << client_port << "#" << buffer << endl;
            write(server_socket_fd, buffer, size);
        }
        else if(size == 0) //对端关闭连接
        {
            cout << client_ip << ":" << client_port << " close" << endl;
            break;
        }
        else //读取失败
        {
            cerr << server_socket_fd << " read error" << endl;
            break;
        }
    }
    close(server_socket_fd);
    cout << client_ip << ":" << client_port << " server done" << endl;
}

TcpServer::~TcpServer() { if(_socket_listen_fd >= 0) close(_socket_listen_fd); }





