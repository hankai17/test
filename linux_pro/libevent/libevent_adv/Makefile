
src=$(wildcard ./*.c) 
target=$(patsubst %.c, %, $(src))  

include=./

CC=gcc
CFLAGS=-Wall -g
LIBS=-levent


ALL:$(target)

$(target):%:%.c
	$(CC) $< -o $@ $(CFLAGS) $(CPPFLAGES) $(LIBS)
	

clean: 
	-rm -rf $(target)

.PHONY: clean ALL
