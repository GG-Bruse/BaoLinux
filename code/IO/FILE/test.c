#include <stdio.h>
#include <unistd.h>
int main()
{
	printf("hello printf\n");
	fputs("hello fputs\n", stdout);
	write(1, "hello write\n", 12);
	fork();
	return 0;
}

