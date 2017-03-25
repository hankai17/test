objects = capture.o get_conf.o function.o dns.o http.o queue.o capture_poll.o cmn_log.o
obj = capture
capture: $(objects)
		cc  -o $(obj) $(objects)  -lpcap -lnet  -g  -rdynamic -lpthread -lrt
function.o:
		cc -g -c function.c prot.h dns.c http.c  -lpthread

get_conf.o:
		cc -g -c get_conf.c prot.h
capture.o:
		cc -g -c capture.c prot.h  -lpcap -lnet
 										
queue.o:
		cc -g -c queue.c queue.h

capture_poll.o:
		cc -g -c capture_poll.c capture_poll.h

cmn_log.o:
		cc -g -c cmn_log.c cmn_log.h

.PHONY: clean

clean:
		rm $(obj) $(objects) *.gch





