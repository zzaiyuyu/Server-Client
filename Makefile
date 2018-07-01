.PHONY:all

all:tclient userver uclient tserver
userver:userver.c
	gcc -o $@ $^ -g
uclient:uclient.c
	gcc -o $@ $^ 
tserver:tserver.c
	gcc -o $@ $^
tclient:tclient.c
	gcc -o $@ $^
clean:
	rm -rf *.o
