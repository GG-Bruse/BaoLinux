#include <iostream>
#include <unistd.h>
#include <signal.h>
using namespace std;

void handler(int signum) {
	cout << "handler signo: " << signum << endl;
}
void PrintPending(sigset_t *pending)
{
	for (int i = 1; i <= 31; i++) {
		if (sigismember(pending, i)) cout << "1 ";
		else cout << "0 ";
	}
	cout << endl;
}
int main()
{
	signal(2, handler);

	sigset_t set, oset;
    sigset_t pending;
    //清空信号集
	sigemptyset(&set);
	sigemptyset(&oset);
    sigemptyset(&pending);

	sigaddset(&set, 2);//添加2号信号
	sigprocmask(SIG_SETMASK, &set, &oset);//将数据设置进内核，阻塞2号信号

	int count = 0;
	while (1) {
		sigpending(&pending); //获取pending
		PrintPending(&pending); //打印pending位图
		sleep(1);
		++count;

		if(count == 20) { //恢复曾经的信号屏蔽字
			cout << "恢复信号屏蔽字" << endl;
			sigprocmask(SIG_SETMASK, &oset, NULL);	
		}
	}
	return 0;
}
