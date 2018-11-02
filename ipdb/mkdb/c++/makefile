.PHONY:clean all
CC=g++
CFLGS= -Wall -g -I./

all:api_example
api_example:c_search.o c_search_test.o
	$(CC) c_search.o c_search_test.o -o api_example

c_search.o:c_search.cc
	$(CC) $(CFLGS) -c c_search.cc -o c_search.o

c_search_test.o:c_search_test.cc
	$(CC) $(CFLGS) -c c_search_test.cc -o c_search_test.o
	
clean:
	rm -rf  *.o api_example
