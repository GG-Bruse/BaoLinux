#include "UdpServer.h"
#include <cstdlib>

int main(int argc, char* argv[]) 
{
    if(argc != 2) {
        cerr << "Usage " << argv[0] << " ip port" << endl;
        return 1;
    }
    string ip = "127.0.0.1";
    UdpServer* server = new UdpServer(ip, atoi(argv[1]));
    server->InitServer();
    server->Start();
    return 0;
}


