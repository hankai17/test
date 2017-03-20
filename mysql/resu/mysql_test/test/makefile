target = $(wildcard *.c)
obj = $(patsubst %.c, %, $(target))

lib_path = /usr/lib64/mysql/ 
inc_path = /usr/include/mysql/
oth_lib = -lstdc++ -ldl -lpthread -lrt

ALL:$(obj)

$(obj):%:%.c
	gcc $< -o $@ -L$(lib_path)-lmysqlclient -I$(inc_path) $(oth_lib)
	
clean:
	-rm -rf $(obj)
	
PHONY: ALL clean
	