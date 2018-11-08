echo ------welcome to use guaz's Script
echo ------GDB Script------------------
echo ----------------------------------

tty /dev/pts/1
b main
set follow-fork-mode child
r

