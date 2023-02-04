#include <stdio.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <unistd.h>
		
#define PATHNAME "/home/bjy/BaoLinux/code/signal_com/shm/shm_1"

#define PROJ_ID 0x666
#define SIZE 4096

int main()
{
	key_t key = ftok(PATHNAME, PROJ_ID);
	if (key < 0) {
		perror("ftok");
		return 1;
	}

	int shm = shmget(key, SIZE, IPC_CREAT | IPC_EXCL); //创建新的共享内存
	if (shm < 0) {
		perror("shmget");
		return 2;
	}
    
	printf("key: %x\n", key);
	printf("shm: %d\n", shm);
    //未释放共享内存...
	return 0;
}
