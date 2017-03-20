src = $(wildcard *.c)
target = $(patsubst %.c, %, $(src))

inc_path=/usr/include/mysql
lib_path = /usr/lib64/mysql/
mylib = -lmysqlclient -lstdc++ -ldl -lpthread -lrt

all: $(target)

$(target):%:%.c
	gcc $< -o $@ -I$(inc_path) -L$(lib_path) $(mylib)
	
clean:
	-rm -rf $(target)
	
.PHONY: clean all