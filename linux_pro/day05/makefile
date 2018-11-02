src = $(wildcard *.c)
targets = $(patsubst %.c, %, $(src))
obj = $(patsubst %.c, %.o, $(src))

CC = gcc
CFLAGS = -Wall -g

all:$(targets)

$(targets):$(obj)
	$(CC) $< -o $@ 

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@ 

.PHONY:clean all
clean:
	-rm -rf $(targets) $(obj)

