//网络套接字封装
#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>
#include <cassert>
#include <unistd.h>
#include <memory>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include "Log.hpp"

class Socket
{
    const static int gbacklog = 20;
public://服务端客户端通用
    static int SocketCreate() {
        int SocketFd = socket(AF_INET, SOCK_STREAM, 0);
        if(SocketFd < 0) {
            LogMessage(FATAL, "socket create fail, %d:%s", errno, strerror(errno));
            exit(1);
        }
        LogMessage(NORMAL, "socket create success, SocketFd:%d", SocketFd);
        return SocketFd;
    }

    static bool SetNonBlock(int socket) {
        int fl = fcntl(socket, F_GETFL);
        if(fl < 0) return false;
        fcntl(socket, F_SETFL, fl | O_NONBLOCK);
        return true;
    } 

public://服务端专用
    static void Bind(int listenSocketFd, uint16_t serverPort, std::string serverIp = "0.0.0.0") {
        struct sockaddr_in local;
        memset(&local, '\0', sizeof local);
        local.sin_family = AF_INET;
        local.sin_port = htons(serverPort);
        inet_pton(AF_INET, serverIp.c_str(), &local.sin_addr);
        if(bind(listenSocketFd, (struct sockaddr*)&local, sizeof local) < 0) {
            LogMessage(FATAL, "bind fail, %d:%s", errno, strerror(errno));
            exit(2);
        }
        LogMessage(NORMAL, "bind success, serverPort:%d", serverPort);
    }

    static void Listen(int listenSocketFd) {
        if(listen(listenSocketFd, gbacklog) < 0) {
            LogMessage(FATAL, "listen fail, %d:%s", errno, strerror(errno));
            exit(3);
        }
        LogMessage(NORMAL, "listen success");
    }

    static int Accept(int listenSocketFd, std::string* clientIp, uint16_t* clientPort, int* acceptErrno) {
        struct sockaddr_in client;
        socklen_t length = sizeof client;
        int serviceSocketFd = accept(listenSocketFd, (struct sockaddr*)&client, &length);
        if(serviceSocketFd < 0) {
            *acceptErrno = errno;
            return -1;
        }
        if(clientIp != nullptr) *clientIp = inet_ntoa(client.sin_addr);
        if(clientPort != nullptr) *clientPort = ntohs(client.sin_port);
        return serviceSocketFd;
    }

public://客户端专用
    bool Connect(int clientSocketFd, std::string& serverIp, uint16_t& serverPort) {
        struct sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = inet_addr(serverIp.c_str());
        server.sin_port = htons(serverPort);
        if(connect(clientSocketFd, (struct sockaddr*)&server, sizeof server) == 0) return true;
        else return false;
    }

public:
    Socket() {}
    ~Socket() {}
};


