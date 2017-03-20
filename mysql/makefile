src = $(wildcard *.c)
target = $(patsubst %.c, %, $(src))

inc_path=/usr/include/mysql
lib_path = /usr/lib64/mysql/
#mylib = -lmysqlclient -lstdc++ -ldl -lpthread -lrt -finput-charset=gbk -fexec-charset=UTF-8
mylib = -lmysqlclient -lstdc++ -ldl -lpthread -lrt -finput-charset=gbk #还是乱码 o开头
#mylib = -lmysqlclient -lstdc++ -ldl -lpthread -lrt #在数据库下显示是乱码 a开头

all: $(target)

$(target):%:%.c
	gcc $< -o $@ -I$(inc_path) -L$(lib_path) $(mylib)
	
clean:
	-rm -rf $(target)
	
.PHONY: clean all
