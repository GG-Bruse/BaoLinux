.PHONY:all
all: server client

server:TcpServer.cc Smain.cc
	g++ $^ -o $@ -std=c++11 -lpthread
client:TcpClient.cc Cmain.cc
	g++ $^ -o $@ -std=c++11 -lpthread

.PHONY:clean
clean:
	rm -f server client
