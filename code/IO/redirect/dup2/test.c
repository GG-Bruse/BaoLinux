#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
int main()
{
	int fd = open("log.txt", O_WRONLY | O_CREAT, 0666);
	if (fd < 0){
		perror("open");
		return 1;
	}
	close(1);
	dup2(fd, 1);
	printf("hello printf\n");
	fprintf(stdout, "hello fprintf\n");
	return 0;
}

