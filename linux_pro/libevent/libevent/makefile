src = $(wildcard *.c)
obj = $(patsubst %.c, %.o, $(src))
target = $(patsubst %.c, %, $(src))

all:$(target) 

$(target):$(obj)
	gcc $< -o $@ -levent

%.o:%.c
	gcc -c $< -o $@  

.PHONY: clean all
clean: 
	-rm -rf $(obj) $(target)
