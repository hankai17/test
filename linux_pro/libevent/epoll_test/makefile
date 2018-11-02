src = $(wildcard *.c)
obj = $(patsubst %.c, %.o, $(src))

all: server client

server: server.o 
	gcc server.o -o server -Wall
client: client.o 
	gcc client.o -o client -Wall

%.o:%.c
	gcc -c $< -Wall

.PHONY: clean all
clean: 
	-rm -rf server client $(obj)
