#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

const int port = 8082;
const int num = 2;

int main()
{
	//创建监听套接字
	int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock < 0){
		std::cerr << "socket error" << std::endl;
		return 1;
	}
	int opt = 1;
	setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	//绑定
	struct sockaddr_in local;
	memset(&local, 0, sizeof(local));
	local.sin_port = htons(port);
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(listen_sock, (struct sockaddr*)&local, sizeof(local)) < 0){
		std::cerr << "bind error" << std::endl;
		return 2;
	}
	//监听
	if (listen(listen_sock, num) < 0){
		std::cerr << "listen error" << std::endl;
		return 3;
	}
	//启动服务器
	for (;;){
		//不调用accept获取连接
	}
	return 0;
}
