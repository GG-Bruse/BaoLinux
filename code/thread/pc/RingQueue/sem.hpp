#ifndef _SEM_HPP
#define _SEM_HPP

#include <iostream>
#include <semaphore.h>
class Sem
{
public:
    Sem(size_t value) { sem_init(&_sem,0,value); }
    ~Sem() { sem_destroy(&_sem); }
    void P() { sem_wait(&_sem); }
    void V() { sem_post(&_sem); }
private:
    sem_t _sem;
};
#endif
