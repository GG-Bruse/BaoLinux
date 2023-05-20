#ifndef __POLL_SVR_H__
#define __POLL_SVR_H__

#include <iostream>
#include <string>
#include <poll.h>
#include "Socket.hpp"
#include "Log.hpp"
#include <unistd.h>
#include <cstring>
#include <cerrno>
using namespace std;

#define FD_NONE -1

// 只完成读取，写入和异常不做处理
class PollServer
{
public:
    PollServer(const nfds_t nfds, const uint16_t &port = 9090) : _port(port), _nfds(nfds), _fds(nullptr)
    {
        _listenSocketFd = Socket::SocketCreate();
        Socket::Bind(_listenSocketFd, _port);
        Socket::Listen(_listenSocketFd);
        LogMessage(DEBUG, "create base socket success");

        _fds = new struct pollfd[_nfds];
        _fds[0].fd = _listenSocketFd;
        _fds[0].events = POLLIN;
        for(int i = 1; i < _nfds; ++i) {
            _fds[i].fd = FD_NONE;
            _fds[i].events = _fds[i].revents = 0;
        }
        _timeout = 1000;
    }
    ~PollServer() { 
        if (_listenSocketFd > 0) close(_listenSocketFd); 
        if (_fds != nullptr) delete[] _fds;
    }

public:
    void Start()
    {
        while (true)
        {
            DebugPrint();
            int number = poll(_fds, _nfds, _timeout);
            switch (number)
            {
            case 0:
                LogMessage(DEBUG, "%s", "Time Out ...");
                break;
            case -1:
                LogMessage(WARNING, "Poll Fail: %d : %s", errno, strerror(errno));
                break;
            default:
                HandlerEvent();
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
        for (; pos < _nfds; ++pos)
            if (_fds[pos].fd == FD_NONE) break;
        if (pos == _nfds) { // 满了
            //可以进行自动扩容
            LogMessage(ERROR, "%s:%d", "PollServer already full, close:", socketfd);
            close(socketfd);
        }
        else { // 找到空位置
            _fds[pos].fd = socketfd;
            _fds[pos].events = POLLIN;
        }
    }

    void Recver(int i) 
    {
        LogMessage(DEBUG, "message in , get IO event:%d", _fds[i].fd);
        char buffer[1024];
        int num = recv(_fds[i].fd, buffer, sizeof(buffer) - 1, 0);
        if(num > 0) {
            buffer[num] = 0;
            LogMessage(DEBUG, "client[%d]#%s", _fds[i].fd, buffer);
        }
        else if(num == 0) {
            LogMessage(DEBUG, "client[%d] link close, me too...", _fds[i].fd);
            close(_fds[i].fd);
            _fds[i].fd = FD_NONE;
            _fds[i].events = _fds[i].revents = 0;
        }
        else {
            LogMessage(WARNING, "%d recv error, %d : %s", _fds[i].fd, errno, strerror(errno));
            close(_fds[i].fd);
            _fds[i].fd = FD_NONE;
            _fds[i].events = _fds[i].revents = 0;
        }
    }

    void HandlerEvent()
    {
        for (int i = 0; i < _nfds; ++i)
        {
            // 去掉不合法的fd
            if (_fds[i].fd == FD_NONE) continue;
            // 判断是否就绪
            if (_fds[i].revents & POLLIN)
            {
                if (_fds[i].fd == _listenSocketFd) Accepter(); //链接事件
                else  Recver(i);// 读事件
            }
        }
    }

    void DebugPrint()
    {
        cout << "fds[]:";
        for(int i = 0; i < _nfds; ++i) {
            if(_fds[i].fd == FD_NONE) continue;
            cout << _fds[i].fd << " ";
        }
        cout << endl;
    }

private:
    uint16_t _port;
    int _listenSocketFd;
    struct pollfd* _fds; 
    nfds_t _nfds = 100;
    int _timeout;
};

#endif