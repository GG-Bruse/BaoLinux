.PHONY:all
all: tcp_server tcp_client

tcp_server:TcpServer.cc Smain.cc
	g++ $^ -o $@ -std=c++11 -lpthread
tcp_client:TcpClient.cc Cmain.cc
	g++ $^ -o $@ -std=c++11 -lpthread

.PHONY:clean
clean:
	rm -f tcp_server tcp_client
