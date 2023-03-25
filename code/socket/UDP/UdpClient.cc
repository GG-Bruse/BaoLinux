#include "UdpClient.h"

UdpClient::~UdpClient() { if(_socket_fd < 0) close(_socket_fd); }

bool UdpClient::InitClient() 
{
	_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (_socket_fd < 0){
		std::cerr << "socket create error" << std::endl;
		return false;
	}
	return true;
}

void UdpClient::Start()
{
    string message;
    struct sockaddr_in receive;
    memset(&receive, 0, sizeof(receive));
    receive.sin_port = htons(_server_port);
    receive.sin_family = AF_INET;
    receive.sin_addr.s_addr = inet_addr(_server_ip.c_str());

    while(true)
    {
        cout << "please Enter#";
        getline(cin, message);
        sendto(_socket_fd, message.c_str(), strlen(message.c_str()), 0, (struct sockaddr*)&receive, sizeof(receive));

        char buffer[SIZE];
		struct sockaddr_in tmp;
		socklen_t length = sizeof(tmp);
		ssize_t size = recvfrom(_socket_fd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&tmp, &length);
		if (size > 0) {
			buffer[size] = '\0';
			cout << buffer << endl;
		}
    }
}
