#include <iostream>
#include <unistd.h>
#include <fcntl.h>
using namespace std;

int main()
{
	char buffer[1024];
	while (true){
		ssize_t size = read(0, buffer, sizeof(buffer)-1);
		if (size < 0){
			cerr << "read error" << endl;
			break;
		}
		else {
            buffer[size] = '\0';
		    cout << "echo# " << buffer << endl;
        }
	}
	return 0;
}
