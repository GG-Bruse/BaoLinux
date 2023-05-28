#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <unistd.h>
#include <cassert>
#include "Socket.hpp"
#include "Log.hpp"
#include "Epoll.hpp"
#include "Protocol.hpp"
using std::string;
using std::function;
using std::unordered_map;
using std::make_pair;
using std::bind;

class TcpServer;
class Connection;
using func_t = function<void(Connection*)>;
using callback_t = function<void(Connection*, string&)>;

class Connection
{
public:
    Connection(int sock = -1):_socketFd(sock),_svrPtr(nullptr) {}
    ~Connection() {}

public:
    void SetCallBack(func_t recvCb, func_t sendCb, func_t exceptCb) {
        _recvCb = recvCb;
        _sendCb = sendCb;
        _exceptCb = exceptCb;
    }

public:
    int _socketFd;
    func_t _recvCb;
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
        AddConnection(_listenSocketFd, bind(&TcpServer::Accepter, this, std::placeholders::_1), nullptr, nullptr);
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
        _epoll.AddSockToEpoll(socket, EPOLLIN | EPOLLET);//一般多路转接服务器默认监视读事件，其他事件按需设置
        //对应的链接添加到映射表中管理
        _connections.insert(make_pair(socket, con));
    }

    void LoopOnce() {
        int number = _epoll.EpollWait(_revs, _revsNum);
        for(int i = 0; i < number; ++i) {
            int socket = _revs[i].data.fd;
            uint32_t revent = _revs[i].events;

            //将所有异常交给read和write处理
            if(revent & EPOLLERR) revent |= (EPOLLIN | EPOLLOUT);
            if(revent & EPOLLHUP) revent |= (EPOLLIN | EPOLLOUT);

            if(revent & EPOLLIN) {
                if((_connections.find(socket) != _connections.end()) && (_connections[socket]->_recvCb != nullptr)) {//存在且回调不为空
                    _connections[socket]->_recvCb(_connections[socket]);
                }
            }
            if(revent & EPOLLOUT) {
                if((_connections.find(socket) != _connections.end()) && (_connections[socket]->_sendCb != nullptr)) {
                    _connections[socket]->_sendCb(_connections[socket]);
                }
            }
        }
    }

    void Dispather(callback_t cb)//根据就绪事件，进行特定事件的派发
    {
        _cb = cb;
        while(true)
        {
            LoopOnce();
        }
    }

    void EnableReadWrite(Connection* con ,bool readable, bool writable) 
    {
        uint32_t event = (readable ? EPOLLIN : 0) | (writable ? EPOLLOUT : 0);
        bool ret = _epoll.EpollCtrl(con->_socketFd, event);
        assert(ret);
    }

public://各种回调方法
    void Accepter(Connection* con) 
    {
        while(true)
        {
            string clientIp;
            uint16_t clientPort;
            int acceptErrno = 0;
            int socket = Socket::Accept(con->_socketFd, &clientIp, &clientPort, &acceptErrno);
            if(socket < 0) 
            {
                if(acceptErrno == EAGAIN || acceptErrno == EWOULDBLOCK) break;//底层已无链接
                else if(acceptErrno == EINTR) continue;//信号中断
                else {//读取失败
                    LogMessage(WARNING, "Accept error, %d : %s", acceptErrno, strerror(acceptErrno));
                    break;
                }
            }
            AddConnection(socket, bind(&TcpServer::Recver, this, std::placeholders::_1), \
            bind(&TcpServer::Sender, this, std::placeholders::_1), bind(&TcpServer::Excepter, this, std::placeholders::_1));
            LogMessage(DEBUG, "Accept client [%s : %d] success, socket: %d", clientIp.c_str(), clientPort, socket);
        }
    }

    void Recver(Connection* con) 
    {
        bool error = false;
        while(true)
        {
            char buffer[1024];
            ssize_t num = recv(con->_socketFd, buffer, sizeof(buffer) - 1, 0);
            if(num < 0) 
            {
                if(errno == EAGAIN || errno == EWOULDBLOCK) break;
                else if(errno == EINTR) continue;
                else {
                    LogMessage(WARNING, "recv error %d : %s", errno, strerror(errno));
                    error = true;
                    con->_exceptCb(con);
                    break;
                }
            }
            else if(num == 0) {
                LogMessage(DEBUG, "client[%d] quit, serve close %d", con->_socketFd, con->_socketFd);
                error = true;
                con->_exceptCb(con);
                break;
            }
            else {
                buffer[num] = '\0';
                con->_inBuffer += buffer;//放入链接的输入缓冲区中
            }
        }
        LogMessage(DEBUG, "socket: %d , con->_inBuffer: %s", con->_socketFd, (con->_inBuffer).c_str());

        if(!error)//无错
        {
            vector<string> messages;
            SpliteMessage(con->_inBuffer, &messages);
            for(auto& msg : messages) _cb(con, msg);
        }
    }

    void Sender(Connection* con)
    {
        while(true)
        {
            ssize_t size = send(con->_socketFd, con->_outBuffer.c_str(), con->_outBuffer.size(), 0);
            if(size > 0) {
                con->_outBuffer.erase(0,size);
                if(con->_outBuffer.empty()) break;
            }
            else {
                if(errno == EAGAIN || errno == EWOULDBLOCK) break;
                else if(errno == EINTR) continue;
                else {
                    LogMessage(WARNING, "send error %d : %s", errno, strerror(errno));
                    con->_exceptCb(con);
                    break;
                }
            }
        }
        if(con->_outBuffer.empty()) EnableReadWrite(con, true, false);
        else EnableReadWrite(con, true, true);
    }

    void Excepter(Connection* con)
    {
        if(!(_connections.find(con->_socketFd) != _connections.end())) return;
        else //还存在
        {
            //从epoll中移除
            bool ret = _epoll.DelFromEpoll(con->_socketFd);
            assert(ret);
            //从映射表中移除
            _connections.erase(con->_socketFd);
            //关闭
            close(con->_socketFd);
            //释放链接对象
            delete con;
        }
        LogMessage(DEBUG, "Excepter 回收完毕");
    }

private:
    int _listenSocketFd;
    uint16_t _port;
    unordered_map<int, Connection*> _connections;//管理服务器链接

    Epoll _epoll;
    struct epoll_event* _revs;//获取就绪事件的缓冲区
    int _revsNum;//缓冲区大小

    callback_t _cb;//上层业务处理
};