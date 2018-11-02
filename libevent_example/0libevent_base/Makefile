


src=$(wildcard ./*.c) #gdbtest.c  hello.c
target=$(patsubst %.c, %, $(src))   #gdbtest hello

include=./

CC=gcc
CFLAGS=-Wall -g
CPPFLAGES= -I $(include)
LIBS=-lpthread -levent


ALL:$(target)

$(target):%:%.c
	$(CC) $< -o $@ $(CFLAGS) $(CPPFLAGES) $(LIBS)
	


clean: 
	-rm -rf $(target)

.PHONY: clean ALL
