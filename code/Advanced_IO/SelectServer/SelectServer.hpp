#ifndef __SELECT_SVR_H__
#define __SELECT_SVR_H__

#include <iostream>
#include <string>
#include <sys/select.h>
#include "Socket.hpp"
#include "Log.hpp"
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <sys/time.h>
using namespace std;

#define BITS 8
#define NUM (sizeof(fd_set) * BITS)
#define FD_NONE -1

// 只完成读取，写入和异常不做处理
class SelectServer
{
public:
    SelectServer(const uint16_t &port = 9090) : _port(port)
    {
        _listenSocketFd = Socket::SocketCreate();
        Socket::Bind(_listenSocketFd, _port);
        Socket::Listen(_listenSocketFd);
        LogMessage(DEBUG, "create base socket success");

        _fd_array[0] = _listenSocketFd;
        for (int i = 1; i < NUM; ++i)
            _fd_array[i] = FD_NONE;
    }
    ~SelectServer()
    {
        if (_listenSocketFd > 0)
            close(_listenSocketFd);
    }

public:
    void Start()
    {
        while (true)
        {
            DebugPrint();

            fd_set readfds;
            FD_ZERO(&readfds);
            int maxFd = _listenSocketFd;
            for (int i = 0; i < NUM; ++i)
            {
                if (_fd_array[i] == FD_NONE)
                    continue;
                else
                    FD_SET(_fd_array[i], &readfds);
                if (maxFd < _fd_array[i]) maxFd = _fd_array[i];
            }

            int number = select(maxFd + 1, &readfds, nullptr, nullptr, nullptr);
            switch (number)
            {
            case 0:
                LogMessage(DEBUG, "%s", "Time Out ...");
                break;
            case -1:
                LogMessage(WARNING, "Select Fail: %d : %s", errno, strerror(errno));
                break;
            default:
                LogMessage(DEBUG, "Get a event");
                HandlerEvent(readfds);
                break;
            }
        }
    }

private:
    void Accepter()
    {
        string clientIp;
        uint16_t clientPort = 0;
        int socketfd = Socket::Accept(_listenSocketFd, &clientIp, &clientPort);
        if (socketfd < 0)
        {
            LogMessage(ERROR, "accept error");
            return;
        }
        LogMessage(DEBUG, "Get a link success : [%s : %d] , socketFd : %d", clientIp.c_str(), clientPort, socketfd);

        int pos = 1;
        for (; pos < NUM; ++pos)
            if (_fd_array[pos] == FD_NONE) break;
        if (pos == NUM) { // 满了
            LogMessage(ERROR, "%s:%d", "SelectServer already full, close:", socketfd);
            close(socketfd);
        }
        else { // 找到空位置
            _fd_array[pos] = socketfd;
        }
    }

    void Recver(int i) 
    {
        LogMessage(DEBUG, "message in , get IO event:%d", _fd_array[i]);
        char buffer[1024];
        int num = recv(_fd_array[i], buffer, sizeof(buffer) - 1, 0);
        if(num > 0) {
            buffer[num] = 0;
            LogMessage(DEBUG, "client[%d]#%s", _fd_array[i], buffer);
        }
        else if(num == 0) {
            LogMessage(DEBUG, "client[%d] link close, me too...", _fd_array[i]);
            close(_fd_array[i]);
            _fd_array[i] = FD_NONE;
        }
        else {
            LogMessage(WARNING, "%d recv error, %d : %s", _fd_array[i], errno, strerror(errno));
            close(_fd_array[i]);
            _fd_array[i] = FD_NONE;
        }
    }

    void HandlerEvent(const fd_set &readfds)
    {
        for (int i = 0; i < NUM; ++i)
        {
            // 去掉不合法的fd
            if (_fd_array[i] == FD_NONE) continue;
            // 判断是否就绪
            if (FD_ISSET(_fd_array[i], &readfds))
            {
                if (i == 0 && _fd_array[i] == _listenSocketFd) Accepter(); //链接事件
                else  Recver(i);// 读事件
            }
        }
    }

    void DebugPrint()
    {
        cout << "_fd_array[]:";
        for (int i = 0; i < NUM; ++i) {
            if (_fd_array[i] != FD_NONE) cout << _fd_array[i] << " ";
        }
        cout << endl;
    }

private:
    uint16_t _port;
    int _listenSocketFd;
    int _fd_array[NUM];
};

#endif