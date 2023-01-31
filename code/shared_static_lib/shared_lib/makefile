libtest.so:test1.o test2.o
	gcc -shared $^ -o $@
test1.o:test1.c
	gcc -fPIC -c test1.c -o test1.o
test2.o:test2.c
	gcc -fPIC -c test2.c -o test2.o

.PHONY:output
output:
	mkdir -p my_lib/include
	mkdir -p my_lib/lib
	cp ./*.h ./my_lib/include 
	cp ./*.so ./my_lib/lib

.PHONY:clean
clean:
	rm -rf *.so *.o my_lib
