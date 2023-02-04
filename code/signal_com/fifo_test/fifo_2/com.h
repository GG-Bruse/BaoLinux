#pragma once

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

#define FILE_NAME "myfifo" //让客户端和服务端使用同一个命名管道
