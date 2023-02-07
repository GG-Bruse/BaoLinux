#include "com.h"

int main()
{
	int fd = open(FILE_NAME, O_WRONLY); //以写的方式打开命名管道文件
	if (fd < 0) {
		perror("open");
		return 1;
	}
	char msg[128];
	while (1){
        memset(msg,'\0',sizeof(msg));
		printf("Please Enter :>");
        fflush(stdout);
		ssize_t s = read(0, msg, sizeof(msg)-1);//从标准输入流中读取
		if (s > 0) {
			write(fd, msg, strlen(msg));
		}
	}
	close(fd);
	return 0;
}
