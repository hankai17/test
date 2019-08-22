echo ------welcome to use guaz's Script
echo ------GDB Script------------------
echo ----------------------------------

tty /dev/pts/2
b menu-example.cc:main 
commands 1
p "!main"
end

b menu-example.cc:26
commands 2
p "!for(;;)"
end

r

