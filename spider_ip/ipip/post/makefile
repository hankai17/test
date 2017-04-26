CC=g++
BIN=ipip
CFLGS=-Wall -g 
LIBFLAG=-L/usr/local/lib

all:$(BIN)
ipip:ipip.o
	$(CC) -lcurl -lboost_regex $(LIBFLAG) $^ -o $@
ipip.o:my_post.cc
	$(CC) $(CFLGS) -c $< -o $@
clean:
	rm -rf *.o $(BIN)
