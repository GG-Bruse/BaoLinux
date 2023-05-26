#include <iostream>
#include <sys/types.h>
using namespace std;

int main()
{
	cout << sizeof(fd_set)* 8 << endl;//1字节 8bit位
	return 0;
}
