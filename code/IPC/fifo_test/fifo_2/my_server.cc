#include "com.h"

int main()
{
	umask(0);
	if (mkfifo(FILE_NAME, 0666) < 0) {
		perror("mkfifo");
		return 1;
	}

	int fd = open(FILE_NAME, O_RDONLY); //以读的方式打开命名管道文件
	if (fd < 0) {
		perror("open");
		return 1;
	}

	char msg[128] = {0};
	while (1){
		memset(msg,'\0',128);
		ssize_t s = read(fd, msg, sizeof(msg)-1);
		if (s > 0) {
			printf("client: %s", msg);
		}
		else if (s == 0) {
			printf("client quit!\n");
			break;
		}
		else {
			printf("read error!\n");
			break;
		}
	}
	close(fd);
    unlink(FILE_NAME);
	return 0;
}
