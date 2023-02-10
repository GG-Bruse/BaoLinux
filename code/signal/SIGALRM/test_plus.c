#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int count = 0;
void handler(int signum) {
    printf("get a signal: %d\n", signum);
    printf("count: %d\n", count);
    exit(0);
}
int main()
{
    signal(SIGALRM, handler);
	alarm(1);
	while (1) {
		count++;
	}
	return 0;
}