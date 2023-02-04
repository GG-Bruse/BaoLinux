#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#define FILE_NAME "myfifo"

int main()
{
	umask(0); //将文件默认掩码设置为0
	if (mkfifo(FILE_NAME, 0666) < 0) {
		perror("mkfifo");
		return 1;
	}
	//... ...
	return 0;
}