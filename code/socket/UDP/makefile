.PHONY:all
all:server client

server:UdpServer.cc Smain.cc
	g++ $^ -o $@ -std=c++11
client:UdpClient.cc Cmain.cc
	g++ $^ -o $@ -std=c++11

.PHONY:clean
clean:
	rm -f server client

