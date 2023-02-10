.PHONY:all
all:test kill_self

test:test.cpp
	g++ $^ -o $@ -std=c++11
kill_self:kill_self.cpp
	g++ $^ -o $@ -std=c++11

.PHONY:clean
clean:
	rm -f kill_self test