echo ----------welcome useing MyScript--------------\n 
echo --------------Gdb Script ----------------------\n 
echo -----------------------------------Beijihu-----\n
r
b getppid
commands
rep
end
define rep
next
shell echo `ps -A |grep "bash"` >./gdb.Temp
shell echo 'print $eax=arr' >./anti.temp
shell sed -i s@arr@`awk -F' ' '{ print $1}' ./gdb.Temp`@g ./anti.temp
source ./anti.temp
shell rm ./*.temp
c
end
