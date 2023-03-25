#include "UdpClient.h"

int main(int argc, char* argv[])
{
    if(argc < 3) {
        cerr << "Usage " << argv[0] << " ip port " << endl;
        return 1;
    }
    string serve_ip = argv[1];
    uint16_t serve_port = atoi(argv[2]);
    UdpClient* client = new UdpClient(serve_ip, serve_port);
    client->InitClient();
    client->Start();
    return 0;
}


