.PHONY:clean all

WORKDIR=.
VPATH = ./src

CC=gcc
CFLGS= -Wall -g -I$(WORKDIR)/inc/
LIBFLAG = -L$(HOME)/lib


BIN = keymngclient   keymngserver


all:$(BIN)

keymngclient:keymngclient.o  keymnglog.o  keymngclientop.o  myipc_shm.o keymng_shmop.o
	$(CC) $(LIBFLAG) -lpthread -litcastsocket -lmessagereal $^ -o $@ 

keymngserver:keymngserver.o  keymngserverop.o  keymnglog.o  myipc_shm.o  keymng_shmop.o  keymng_dbop.o 
	$(CC) $(LIBFLAG) $^ -o $@ -lpthread -litcastsocket -lmessagereal  -lclntsh  -licdbapi
 
#testdbapi:testdbapi.o  
#	$(CC) $(LIBFLAG) $^ -o $@ -lpthread  -lclntsh  -licdbapi
		
%.o:%.c
	$(CC) $(CFLGS) -c $< -o $@	

clean:
	rm -f *.o $(BIN)
	
	
	




