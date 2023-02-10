#include <stdio.h>
#include <unistd.h>

int main()
{
	int count = 0;
	alarm(1);
	while (1) {
		count++;
		printf("count: %d\n", count);
	}
	return 0;
}