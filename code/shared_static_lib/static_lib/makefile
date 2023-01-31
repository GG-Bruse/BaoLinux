libtest.a:test1.o test2.o
	ar -rc $@ $^
test1.o:test1.c
	gcc -c $^ -o $@
test2.o:test2.c
	gcc -c $^ -o $@

.PHONY:output
output:
	mkdir -p my_lib/include
	mkdir -p my_lib/lib
	cp ./*.h ./my_lib/include 
	cp ./*.a ./my_lib/lib

.PHONY:clean
clean:
	rm -rf ./my_lib ./*.o ./*.a
