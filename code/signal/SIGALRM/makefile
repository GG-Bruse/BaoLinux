.PHONY:all
all:test_plus test

test_plus:test_plus.c
	gcc $^ -o $@
test:test.c
	gcc $^ -o $@

.PHONY:clean
clean:
	rm -f test