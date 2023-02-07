#include <stdio.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define PATHNAME "/home/bjy/BaoLinux/code/signal_com/shm/shm_2" //路径名
#define PROJ_ID 0x6666 //整数标识符
#define SIZE 4096 //共享内存的大小
