tty /dev/pts/1
b main

b 16
commands 2
gdb_callback
end

r


define gdb_callback
p "this is break "
p i
cont 20
end

