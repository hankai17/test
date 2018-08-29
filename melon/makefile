.PHONY:clean all
VERSION=0.1
WORKDIR=.
VPATH=./src
CC=g++
CFLGS= -Wall -g -I$(WORKDIR)/inc/
LIBFLAG = -L$(HOME)/lib
BIN=main

all:$(BIN)
main:socket.o async_server.o server_frame.o  web_server.o select_mplexer.o epoll_mplexer.o main.o global.o miobuffer.o event.o
	$(CC) $(LIBFLAG) -lpthread $^ -o $@ 
%.o:%.cc
	$(CC) $(CFLGS) -c $< -o $@

clean:
	rm -f *.o *~ $(BINARIES)
