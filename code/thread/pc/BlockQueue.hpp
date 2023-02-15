#include <iostream>
#include <queue>
#include <pthread.h>

template <class T>
class BlockQueue
{
public:
    BlockQueue(size_t capacity = 4) : _capacity(capacity)
    {
        pthread_mutex_init(&_mutex,nullptr);
        pthread_cond_init(&_full,nullptr);
        pthread_cond_init(&_empty,nullptr);
    }
    ~BlockQueue()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_full);
        pthread_cond_destroy(&_empty);
    }

    void push(const T& data)
    {
        pthread_mutex_lock(&_mutex);
		while (IsFull()) {//不能进行生产，直到阻塞队列可以容纳新的数据
			pthread_cond_wait(&_full, &_mutex);
		}
		_queue.push(data);
        std::cout << "Producer: " << data << std::endl;
		pthread_mutex_unlock(&_mutex);
		pthread_cond_signal(&_empty); //唤醒在empty条件变量下等待的消费者线程
    }
    void pop(T& data)
    {
        pthread_mutex_lock(&_mutex);
		while (IsEmpty()) {//不能进行消费，直到阻塞队列有新的数据
			pthread_cond_wait(&_empty, &_mutex);
		}
		data = _queue.front();
		_queue.pop();
        std::cout << "Consumer: " << data << std::endl;
		pthread_mutex_unlock(&_mutex);
		pthread_cond_signal(&_full); //唤醒在full条件变量下等待的生产者线程
    }
    
private:
    bool IsFull() { return _queue.size() == _capacity; }
    bool IsEmpty() { return _queue.empty(); }

private:
    std::queue<T> _queue;
    size_t _capacity;
    pthread_mutex_t _mutex;
    pthread_cond_t _full;
    pthread_cond_t _empty;
};