.PHONY:clean all

WORKDIR = .
VPATH = ./src

CC=g++
CFLGS= -Wall -g -I./inc/
LIBFLAG = -L$(HOME)/lib

BIN = spider

all:$(BIN)

spider:spider_ip.o tb.o ipcn.o ip138.o main.o  
	$(CC) -lcurl $^ -o $@ 

%.o:%.cc
	$(CC) $(CFLGS) -c $< -o $@ 

clean:
	rm -f *.o $(BIN)

