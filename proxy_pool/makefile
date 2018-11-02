.PHONY:clean all
CC=g++
CFLGS= -Wall -g -fpermissive -I.

BIN = proxy_pool 
all:$(BIN)

proxy_pool:main.o
	$(CC) -lcurl -lpthread $^ -o $@ 

%.o:%.cc
	$(CC) $(CFLGS) -c $< -o $@ 

clean:
	rm -f *.o $(BIN)
