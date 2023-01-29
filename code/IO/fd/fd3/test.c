#include <stdio.h>
int main()
{
	printf("hello printf\n"); //stdout
	perror("perror"); //stderr

	fprintf(stdout, "stdout:hello fprintf\n"); //stdout
	fprintf(stderr, "stderr:hello fprintf\n"); //stderr
	return 0;
}

