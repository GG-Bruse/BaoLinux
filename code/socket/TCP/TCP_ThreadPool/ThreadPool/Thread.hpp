#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <cstdio>

class ThreadDate
{
public:
    void* _args;
    std::string _name;
};

typedef void*(*func_t)(void*);
class Thread
{
public:
    Thread(size_t num,func_t callback,void* args): _function(callback)
    {
        char nameBuffer[64];
        memset(nameBuffer, '\0', sizeof nameBuffer);
        snprintf(nameBuffer, sizeof nameBuffer, "Thread-%d", num);
        _threadDate._name = nameBuffer;
        _name = nameBuffer;
        _threadDate._args = args;
    }
    ~Thread() {}
    void Start() { pthread_create(&_tid, nullptr, _function, (void*)&_threadDate); }
    void Join() { pthread_join(_tid, nullptr); }
    std::string Name() { return _name; }
private:
    std::string _name;
    func_t _function;
    ThreadDate _threadDate;
    pthread_t _tid;
};