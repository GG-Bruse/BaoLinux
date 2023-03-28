#pragma once
#include <iostream>
#include <string>
#include <functional>


#pragma once
#include <iostream>
#include <string>
#include <functional>

typedef void(*fun_t)(int, std::string, uint16_t);
class Task
{
public:
	Task() {}
	Task(int sock, std::string client_ip, int client_port, fun_t handler) : _server_socket_fd(sock)
		, _client_ip(client_ip), _client_port(client_port), _handler(handler) {}
	//任务处理函数
	void operator()(const std::string& name) {
        _handler(_server_socket_fd, _client_ip, _client_port);
	}
private:
	int _server_socket_fd;
	std::string _client_ip;
	uint16_t _client_port;
    fun_t _handler;
};
