#########################################################################
# File Name: make.sh
# Author: guaz
# mail: hankai17@126.com
# Created Time: 2017-02-22 17:47:07
#########################################################################
#!/bin/bash
#g++ 1.cpp -L/usr/local/lib  -I/usr/local/include/google  
#-Bdynamic -lprofiler
#gcc -fno-omit-frame-pointer -g -c 2.cpp -o a.o
#gcc -fno-omit-frame-pointer -g a.o -L/usr/local/lib -I/usr/local/include/google -Bdynamic -lprofiler -o a.out 
g++ main.cc conf.cc -lprofiler

