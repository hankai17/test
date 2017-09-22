VERSION=0.1
CXXFLAGS=-Wall -fpermissive -Wint-to-pointer-cast -I../ -g -O2
SOURCEFILES=../mtasker.hh ../mtasker.cc

BINARIES=wserver menu-example ab-example

all: $(BINARIES)

clean:
	rm -f *.o *~ $(BINARIES)

wserver.o: misc.hh wserver.cc $(SOURCEFILES)

wserver: wserver.o 
	g++ wserver.o -o wserver

menu-example: menu-example.o $(SOURCEFILES)
	g++ menu-example.o -o menu-example

ab-example: ab-example.o $(SOURCEFILES)
	g++ ab-example.o -o ab-example
