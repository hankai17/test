.PHONY:clean all

WORKDIR=.
VPATH=./src

CC=g++
CFLAGS=-Wall -g -I$(WORKDIR)/inc/

all:demo

demo:util_cgi.o c_search.o cgi_parse.o
	$(CC) -lfcgi $^ -o $@
cgi_parse.o:cgi_parse.cc
	$(CC) -c $(CFLAGS) $< -o $@
c_search.o:c_search.cc
	$(CC) -c $(CFLAGS) $< -o $@
util_cgi.o:util_cgi.c
	#$(CC) -c $(CFLAGS) $< -o $@
	gcc -c $(CFLAGS) $< -o $@
	#切记不能这样写gcc -c $(CFLAGS) util_cgi.c -o util_cgi.o
clean:
	rm -rf *.o demo
