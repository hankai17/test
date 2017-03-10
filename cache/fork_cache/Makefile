objects = capture.o get_conf.o function.o dns.o http.o
obj = capture
capture: $(objects)
		cc  -o $(obj) $(objects)  -lpcap -lnet  -g  -rdynamic -lpthread 
function.o:
		cc -g -c function.c prot.h dns.c http.c  -lpthread

get_conf.o:
		cc -g -c get_conf.c prot.h
capture.o:
		cc -g -c capture.c prot.h  -lpcap -lnet

.PHONY: clean

clean:
		rm $(obj) $(objects) *.gch





