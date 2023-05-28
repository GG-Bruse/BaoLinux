#pragma once
#include <iostream>
#include <sys/epoll.h>

class Epoll
{
public:
    Epoll() {}
    ~Epoll() {}
public:
    void EpollCreate() {
        _epollFd = epoll_create(128);
        if(_epollFd < 0) exit(5);
    }

    bool AddSockToEpoll(int socket, uint32_t event) 
    {
        struct epoll_event ev;
        ev.events = event;
        ev.data.fd = socket;
        int n = epoll_ctl(_epollFd, EPOLL_CTL_ADD, socket, &ev);
        return n == 0;
    }

    bool EpollCtrl(int socket, uint32_t event) 
    {
        event |= EPOLLET;
        struct epoll_event ev;
        ev.events = event;
        ev.data.fd = socket;
        int n = epoll_ctl(_epollFd, EPOLL_CTL_MOD, socket, &ev);
        return n == 0;
    }

    int EpollWait(struct epoll_event* revs, int revsNum) {
        return epoll_wait(_epollFd, revs, revsNum, 5000);
    }

    bool DelFromEpoll(int socket) 
    {
        int n = epoll_ctl(_epollFd, EPOLL_CTL_DEL, socket, 0);
        return n == 0;
    }

private:
    int _epollFd;
};