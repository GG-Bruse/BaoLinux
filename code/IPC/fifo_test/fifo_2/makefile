.PHONY:all
all:my_server my_client

my_server:my_server.cc
	g++ $^ -o $@
my_client:my_client.cc
	g++ $^ -o $@

.PHONY: clean
clean:
	rm -f my_server my_client
