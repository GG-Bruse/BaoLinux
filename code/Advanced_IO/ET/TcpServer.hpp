#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <unistd.h>
#include "Socket.hpp"
#include "Log.hpp"
#include "Epoll.hpp"
using std::string;
using std::function;
using std::unordered_map;
using std::make_pair;
using std::bind;
using namespace std::placeholders;

class TcpServer;
class Connection;
using func_t = function<void(Connection*)>;

class Connection
{
public:
    Connection(int sock = -1):_socketFd(sock),_svrPtr(nullptr) {}
    ~Connection() {}

public:
    void SetCallBack(func_t reavCb, func_t sendCb, func_t exceptCb) {
        _reavCb = reavCb;
        _sendCb = sendCb;
        _exceptCb = exceptCb;
    }

public:
    int _socketFd;
    func_t _reavCb;
    func_t _sendCb;
    func_t _exceptCb;
    string _inBuffer;//无法处理二进制流
    string _outBuffer;
    TcpServer* _svrPtr;
};



class TcpServer
{
public:
    TcpServer(uint16_t port = 8080, int revsNum = 128):_port(port), _revsNum(revsNum)
    {
        //创建listenSocket
        _listenSocketFd = Socket::SocketCreate();
        Socket::Bind(_listenSocketFd, _port);
        Socket::Listen(_listenSocketFd);
        //创建多路转接对象
        _epoll.EpollCreate();
        //将监听套接字封装为链接并添加至服务器的管理中
        AddConnection(_listenSocketFd, bind(&TcpServer::Accepter, this, _1), nullptr, nullptr);
        //为获取就绪事件的缓冲区开辟内存
        _revs = new struct epoll_event[_revsNum];
    }
    ~TcpServer() { 
        if(_listenSocketFd >= 0) close(_listenSocketFd); 
        if(_revs != nullptr) delete[] _revs;
    }

public:
    void AddConnection(int socket, func_t reavCb, func_t sendCb, func_t exceptCb) //将套接字封装为链接并添加至服务器的管理中
    {
        //设置套接字为非阻塞
        Socket::SetNonBlock(socket);
        //将套接字封装为链接，设置链接的各个属性
        Connection* con = new Connection(socket);
        con->SetCallBack(reavCb, sendCb, exceptCb);//监听套接字只需读取回调函数
        con->_svrPtr = this;
        //添加套接字到epoll中
        _epoll.AddSockToEpoll(socket, EPOLLIN | EPOLLET);//ET模式
        //对应的链接添加到映射表中管理
        _connections.insert(make_pair(socket, con));
    }

    void LoopOnce() {
        int number = _epoll.EpollWait(_revs, _revsNum);
        for(int i = 0; i < number; ++i) {
            int socket = _revs[i].data.fd;
            uint32_t revent = _revs[i].events;
            if(revent & EPOLLIN) {
                if((_connections.find(socket) != _connections.end()) && (_connections[socket]->_reavCb != nullptr)) {//存在且回调不为空
                    _connections[socket]->_reavCb(_connections[socket]);
                }
            }
            if(revent & EPOLLOUT) {
                if((_connections.find(socket) != _connections.end()) && (_connections[socket]->_sendCb != nullptr)) {
                    _connections[socket]->_sendCb(_connections[socket]);
                }
            }
        }
    }
    void Dispather()//根据就绪事件，进行特定事件的派发
    {
        while(true)
        {
            LoopOnce();

        }
    }

public://各种回调方法
    void Accepter(Connection* con) 
    {
        LogMessage(DEBUG, "Accepter been called!!!");
    }

private:
    int _listenSocketFd;
    uint16_t _port;
    unordered_map<int, Connection*> _connections;//管理服务器链接

    Epoll _epoll;
    struct epoll_event* _revs;//获取就绪事件的缓冲区
    int _revsNum;//缓冲区大小
};