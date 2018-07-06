.PHONY:all

all:tclient userver uclient tserver tserverMP tserverMT
userver:userver.c
	gcc -o $@ $^ -g
uclient:uclient.c
	gcc -o $@ $^ 
tserver:tserver.c
	gcc -o $@ $^
tclient:tclient.c
	gcc -o $@ $^
tserverMP:

tserverMT:tserverMT.c
	gcc -o $@ $^ -pthread
clean:
	rm -rf *.o
