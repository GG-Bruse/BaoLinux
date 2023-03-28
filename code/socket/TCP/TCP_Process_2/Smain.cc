#include "TcpServer.h"

void Usage(std::string proc)
{
	cout << "Usage: " << proc << "server_ip server_port" << endl;
}
int main(int argc, char* argv[])
{
	if (argc != 2) {
		Usage(argv[0]);
		exit(1);
	}
	int server_port = atoi(argv[1]);

	TcpServer* server = new TcpServer(server_port);
	server->InitServer();
	server->StartUp();
	return 0;
}