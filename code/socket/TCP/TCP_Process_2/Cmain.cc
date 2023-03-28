#include "TcpClient.h"

void Usage(std::string proc)
{
	cout << "Usage: " << proc << "server_ip server_port" << endl;
}
int main(int argc, char* argv[])
{
	if (argc != 3) {
		Usage(argv[0]);
		exit(1);
	}
	string server_ip = argv[1];
	int server_port = atoi(argv[2]);

	TcpClient* client = new TcpClient(server_ip, server_port);
	client->InitClient();
	client->StartUp();
	return 0;
}
