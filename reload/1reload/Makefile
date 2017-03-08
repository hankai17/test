
main: demo.o auto_file_loader.h auto_file_loader.hpp
	g++ -o demo demo.o -lpthread

demo.o: demo.cpp
	g++ -c demo.cpp

clean:
	rm -rf demo.o demo

