#include "ThreadPool.hpp"
#include "Task.hpp"

int main()
{
    ThreadPool<Task>* threadPool = ThreadPool<Task>::GetThreadPool(5);
    threadPool->Run();
    while(true)
    {
        //生产的过程，制作任务的时候，要花时间
        int x = rand()%100 + 1;
        usleep(7721);
        int y = rand()%30 + 1;
        Task task(x, y, [](int x, int y)->int{
            return x + y;
        });

        LogMessage(NORMAL, "制作任务完成: %d+%d=?", x, y);
        //推送任务到线程池中
        threadPool->PushTask(task);
        sleep(1);
    }
    return 0;
}