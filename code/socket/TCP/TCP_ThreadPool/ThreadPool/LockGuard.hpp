#pragma once
#include <iostream>
#include <pthread.h>

class Mutex
{
public:
    Mutex(pthread_mutex_t *mtx):_pmtx(mtx) {}
    void Lock() { pthread_mutex_lock(_pmtx); }
    void UnLock() { pthread_mutex_unlock(_pmtx); }
    ~Mutex() {}
private:
    pthread_mutex_t *_pmtx;
};

// RAII风格的加锁方式
class LockGuard
{
public:
    LockGuard(pthread_mutex_t *mtx):_mutex(mtx) { _mutex.Lock(); }
    ~LockGuard() { _mutex.UnLock(); }
private:
    Mutex _mutex;
};