#ifndef __EPOLL_SERVER_HPP__
#define __EPOLL_SERVER_HPP__

#include <iostream>
#include <string>
#include <functional>
#include <cassert>
#include <unistd.h>
#include "Epoll.hpp"
#include "Log.hpp"
#include "Socket.hpp"
using namespace std;

namespace ns_epoll
{
    class EpollServer
    {
        using func_t = function<void(string)>;
    public:
        EpollServer(func_t handler, const uint16_t& port = 9090):_port(port), _revsNum(64),_handlerRequest(handler) {
            //申请空间
            _revs = new struct epoll_event[_revsNum];
            //创建监听套接字
            _listenSocketFd = Socket::SocketCreate();
            Socket::Bind(_listenSocketFd, _port);
            Socket::Listen(_listenSocketFd);
            //创建epoll模型
            _epollFd = Epoll::EpollCreate();
            LogMessage(DEBUG, "init success, listenSocketFd : %d, epollFd : %d", _listenSocketFd, _epollFd);
            //将监听套接字添加到epoll中
            if(Epoll::EpollCtl(_epollFd, EPOLL_CTL_ADD, _listenSocketFd, EPOLLIN))
                LogMessage(DEBUG, "Add listenSocketFd to epoll success");
            else exit(6);
        }
        ~EpollServer() {
            if(_listenSocketFd >= 0) close(_listenSocketFd);
            if( _epollFd >= 0) close(_epollFd);
            if(_revs != nullptr) delete[] _revs;
        }
    public:
        void Start()
        {
            int timeout = -1;
            while(true) 
            {
                LoopOnce(timeout);
            }
        }
    public:
        void LoopOnce(int timeout) {
            int num = Epoll::EpollWait(_epollFd, _revs, _revsNum, timeout);
            switch (num)
            {
            case 0:
                LogMessage(DEBUG, "Time Out...");
                break;
            case -1:
                LogMessage(WARNING, "epoll wait error: %s", strerror(errno));
                break;
            default:
                LogMessage(DEBUG, "Get a event");
                HandlerEvents(num);
                break;
            }
        }

        void HandlerEvents(int number)
        {
            assert(number);
            for(int i = 0; i < number; ++i) 
            {
                uint32_t revent = _revs[i].events;
                int socketFd = _revs[i].data.fd;
                if(revent & EPOLLIN) //读事件就绪
                {
                    if(socketFd == _listenSocketFd) Accetper(_listenSocketFd);
                    else Recver(socketFd);
                }
            }
        }

        void Accetper(int listenSocketFd) 
        {
            string clientIp;
            uint16_t clientPort;
            int socketFd = Socket::Accept(listenSocketFd, &clientIp, &clientPort);
            if(socketFd < 0) {
                LogMessage(WARNING, "Accept error");
                return;
            }
            if(!Epoll::EpollCtl(_epollFd, EPOLL_CTL_ADD, socketFd, EPOLLIN)) return;
            LogMessage(DEBUG, "Add new link : %d to epoll success", socketFd);
        }

        void Recver(int socketFd)
        {
            char buffer[10240];
            ssize_t n = recv(socketFd, buffer, sizeof(buffer) - 1, 0);
            if(n > 0) {
                buffer[n] = 0;
                _handlerRequest(buffer);
            }
            else if(n == 0) {
                LogMessage(NORMAL, "client %d close link, me too...", socketFd);
                bool ret = Epoll::EpollCtl(_epollFd, EPOLL_CTL_DEL, socketFd, 0);
                assert(ret);
                close(socketFd);
            }
            else {
                LogMessage(NORMAL, "client %d recv error, close error socketFd", socketFd);
                bool ret = Epoll::EpollCtl(_epollFd, EPOLL_CTL_DEL, socketFd, 0);
                assert(ret);
                close(socketFd);
            }
        }

    private:
        int _listenSocketFd;
        int _epollFd;
        uint16_t _port;

        struct epoll_event* _revs;
        int _revsNum;

        func_t _handlerRequest;
    };
}

#endif