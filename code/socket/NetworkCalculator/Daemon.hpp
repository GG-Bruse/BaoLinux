#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void DaemonSelf()
{
    //1. 设置文件掩码
    umask(0);
    //2. 忽略SIGCHLD、SIGPIPE信号
    signal(SIGCHLD, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    //3. fork后终止父进程,保证子进程不为进程组组长
    if(fork() > 0) exit(0);
    //4. 创建新会话
    setsid();
    //5. fork后终止父进程，保持子进程不是会话首进程，从而保证后续不会再和其他终端相关联
	//（不是必须的，防御性编程）
    if(fork() > 0) exit(0);
    //6. 更改工作目录为根目录
    chdir("/");
    //7. 标准输入、标准输出、标准错误重定向到/dev/null
    int devNull = open("/dev/null", O_RDWR);
    if(devNull > 0) {
        dup2(devNull, 0);
        dup2(devNull, 1);
        dup2(devNull, 2);
        close(devNull);
    }
}