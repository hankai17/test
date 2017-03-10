.PHONY:clean all
WORKDIR=.
CC=gcc
CFLGS= -Wall -g -I$(WORKDIR)
LIBFLAG = -L /usr

BIN = log_test
all:$(BIN)

log_test:1.o hk_log.o
	$(CC) $(LIBFLAG) $^ -o $@

%.o:%.c
	$(CC) $(CFLGS) -c $< -o $@	

clean:
	rm -f *.o $(BIN)
