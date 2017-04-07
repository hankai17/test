.PHONY:clean all

WORKDIR = .
VPATH = ./src

CC=g++
CFLGS= -Wall -g -I./inc/
LIBFLAG = -L$(HOME)/lib

BIN = spider

all:$(BIN)

spider:spider_ip.o tb.o ip138.o ipcn.o log.o main.o  
	$(CC) -lcurl -lpthread $^ -o $@ 

%.o:%.cc
	$(CC) $(CFLGS) -c $< -o $@ 

clean:
	rm -f *.o $(BIN)

