#ifndef _RING_QUEUE_HPP_
#define _RING_QUEUE_HPP_

#include <iostream>
#include <vector>
#include <pthread.h>
#include "sem.hpp"

const size_t g_default_num = 5;
template<class T>
class RingQueue
{
public:
    RingQueue(size_t default_num = g_default_num)
    :_ring_queue(default_num),_num(default_num),_producer_index(0),_consumer_index(0) 
    ,_space_sem(default_num),_data_sem(0) {
        pthread_mutex_init(&_producer_mtx,nullptr);
        pthread_mutex_init(&_consumer_mtx,nullptr);
    }

    ~RingQueue() {
        pthread_mutex_destroy(&_producer_mtx);
        pthread_mutex_destroy(&_consumer_mtx);
    }

    void Push(const T& in)
    {
        _space_sem.P();
        pthread_mutex_lock(&_producer_mtx);
        _ring_queue[_producer_index++] = in;
        _producer_index %= _num;
        pthread_mutex_unlock(&_producer_mtx);
        _data_sem.V();
    }
    void Pop(T* out)
    {
        _data_sem.P();
        pthread_mutex_lock(&_consumer_mtx);
        *out = _ring_queue[_consumer_index++];
        _consumer_index %= _num;
        pthread_mutex_unlock(&_consumer_mtx);
        _space_sem.V();
    }
    void Debug() { 
        std::cerr << "size:" << _ring_queue.size() << std::endl; 
        std::cerr << "num:" << _num << std::endl; 
    }
private:
    std::vector<T> _ring_queue;
    int _num;
    int _producer_index;
    int _consumer_index;
    Sem _space_sem;
    Sem _data_sem;
    pthread_mutex_t _producer_mtx;
    pthread_mutex_t _consumer_mtx;
};
#endif

