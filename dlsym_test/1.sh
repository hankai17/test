#环境变量LD_LIBRARY_PATH主要用于查找共享库时除了默认路径外的其他路径;
#此处是把当前路径加入到查找路径的意思
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.

#优先链接libfirst_one.so
gcc -o first main.o  -lwrap -lfirst_one  -lsecond_one -ldl -L.
 
#优先链接libsecond_one.so
gcc -o second main.o  -lwrap -lsecond_one -lfirst_one  -ldl -L.
