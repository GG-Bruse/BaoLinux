#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

int main()
{
	//创建套接字
	int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock < 0){
		cerr << "socket error!" << endl;
		return 1;
	}
	//绑定
	struct sockaddr_in local;
	memset(&local, 0, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_port = htons(9090);
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(listen_sock, (struct sockaddr*)&local, sizeof(local)) < 0){
		cerr << "bind error!" << endl;
		return 2;
	}
	//监听
	if (listen(listen_sock, 5) < 0){
		cerr << "listen error!" << endl;
		return 3;
	}
	//启动服务器
	struct sockaddr peer;
	memset(&peer, 0, sizeof(peer));
	socklen_t len = sizeof(peer);
	for (;;){
		int sock = accept(listen_sock, (struct sockaddr*)&peer, &len);
		if (sock < 0){
			cerr << "accept error!" << endl;
			continue;
		}
		if (fork() == 0){ //爸爸进程
			close(listen_sock);
			if (fork() > 0){ //爸爸进程
				exit(0);
			}
			//孙子进程
			char buffer[1024];
			recv(sock, buffer, sizeof(buffer), 0); //读取HTTP请求
			cout << "--------------------------http request begin--------------------------" << endl;
			cout << buffer << endl;
			cout << "---------------------------http request end---------------------------" << endl;
			
#define PAGE "index.html" //网站首页
			//读取index.html文件
			ifstream in(PAGE);
			if (in.is_open()){
				in.seekg(0, in.end);
				int len = in.tellg();
				in.seekg(0, in.beg);
				char* file = new char[len];
				in.read(file, len);
				in.close();
				
				//构建HTTP响应
				string status_line = "http/1.1 200 OK\n"; //状态行
				string response_header = "Content-Length: " + to_string(len) + "\n"; //响应报头
				string blank = "\n"; //空行
				string response_text = file; //响应正文
				string response = status_line + response_header + blank + response_text; //响应报文
				
				//响应HTTP请求
				send(sock, response.c_str(), response.size(), 0);

				delete[] file;
			}
			close(sock);
			exit(0);
		}
		//爷爷进程
		close(sock);
		waitpid(-1, nullptr, 0); //等待爸爸进程
	}
	return 0;
}
