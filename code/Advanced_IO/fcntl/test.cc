#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
using namespace std;

bool SetNonBloack(int fd) 
{
    int fl = fcntl(fd, F_GETFL);//获取该fd对应的文件读写标志位
    if(fl < 0) return false;
    fcntl(fd, F_SETFL, fl | O_NONBLOCK);//设置非阻塞
    return true;
}

int main()
{
    SetNonBloack(0);
    char buffer[1024];
    while(true) 
    {
        sleep(1);
        errno = 0;
        ssize_t size = read(0,buffer,sizeof(buffer) - 1);//默认阻塞式IO
        if(size > 0) {
            buffer[size - 1] = 0;
            cout << "echo# " << buffer << " errno[success]:" << errno << " errString:" << strerror(errno) <<endl;
        }
        else {
            if(errno == EWOULDBLOCK || errno == EAGAIN) {
                cout << "当前0号fd数据没有就绪,请待会再进行尝试" << endl;
                continue;
            } 
            else if(errno == EINTR) {
                cout << "当前IO可能被信号中断,请再次尝试" << endl;
                continue;
            }
            else {//差错处理
                cout << "read error" << " errno:" << errno << " errString:" << strerror(errno) <<endl;
            }
        }
    }
    return 0;
}