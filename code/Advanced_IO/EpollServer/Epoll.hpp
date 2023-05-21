#include <iostream>
#include <sys/epoll.h>
#include <cstdlib>
using namespace std;

class Epoll
{
public:
    static const int gsize = 256; 
public:
    static int EpollCreate() 
    {
        int epollFd = epoll_create(gsize);
        if(epollFd > 0) return epollFd;
        exit(5);//创建失败直接终止
    }

    static bool EpollCtl(int epollFd, int op, int socketFd, uint32_t events) 
    {
        struct epoll_event ev;
        ev.events = events;
        ev.data.fd = socketFd;
        int num = epoll_ctl(epollFd, op, socketFd, &ev);
        return num == 0;
    }

    static int EpollWait(int epollFd, struct epoll_event* revs, int num, int timeout) {
        return epoll_wait(epollFd, revs, num, timeout);
    }
};