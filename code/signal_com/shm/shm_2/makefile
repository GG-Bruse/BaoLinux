.PHONY:all
all:my_server my_client

my_client:my_client.cxx
	g++ $^ -o $@
my_server:my_server.cxx
	g++ $^ -o $@

.PHONY:clean
clean:
	rm -rf my_client my_server