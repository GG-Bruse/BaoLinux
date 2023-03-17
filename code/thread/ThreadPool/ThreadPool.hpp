#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unistd.h>
#include "Thread.hpp"
#include "LockGuard.hpp"
#include "Log.hpp"

//懒汉模式
const int g_threadNum = 3;
template<class T>
class ThreadPool
{
public://为routine()静态函数提供
    pthread_mutex_t *GetMutex() { return &_mutex; }
    bool isEmpty() { return _taskQueue.empty(); }
    void WaitCond() { pthread_cond_wait(&_cond, &_mutex); }
    T GetTask() {
        T task = _taskQueue.front();
        _taskQueue.pop();
        return task;
    }

public:
    //需考虑多线程申请单例的情况
    static ThreadPool<T>* GetThreadPool(int num = g_threadNum)
    {
        if(nullptr == pool_ptr) {
            {
                LockGuard lockguard(&_init_mutex);
                if(nullptr == pool_ptr) {
                    pool_ptr = new ThreadPool<T>(num);
                }
            }
        }
        return pool_ptr;
    }

    static void* routine(void* args) {
        ThreadDate* thread_date = (ThreadDate*)args;
        ThreadPool<T>* thread_pool = (ThreadPool<T>*)thread_date->_args;
        while(true) {
            T task;
            {
                LockGuard lockguard(thread_pool->GetMutex());
                while(thread_pool->isEmpty()) thread_pool->WaitCond();
                task = thread_pool->GetTask();
            }
            task(thread_date->_name);//仿函数
        }
    }
    void PushTask(const T& task)
    {
        LockGuard lockguard(&_mutex);
        _taskQueue.push(task);
        pthread_cond_signal(&_cond);
    }
    void Run()
    {
        for(auto& iter : _threads) {
            iter->Start(); 
            LogMessage(DEBUG, "%s %s", iter->Name().c_str(), "启动成功");
        }
    }
    ~ThreadPool()
    {
        for (auto &iter : _threads) {
            iter->Join();
            delete iter;
        }
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
    }

private:
    ThreadPool(int threadNum):_num(threadNum) {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_cond, nullptr);
        for (int i = 1; i <= _num; i++) {
            _threads.push_back(new Thread(i, routine, this));
        }
    }
    ThreadPool(const ThreadPool<T>& others) = delete;
    ThreadPool<T>& operator= (const ThreadPool<T>& others) = delete;

private:
    std::vector<Thread*> _threads;
    size_t _num;
    std::queue<T> _taskQueue;
private:
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;
private:
    static ThreadPool<T>* pool_ptr;
    static pthread_mutex_t _init_mutex;
};

template<typename T>
ThreadPool<T>* ThreadPool<T>::pool_ptr = nullptr;
template<typename T>
pthread_mutex_t ThreadPool<T>::_init_mutex = PTHREAD_MUTEX_INITIALIZER;
