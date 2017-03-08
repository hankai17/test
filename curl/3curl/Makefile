target = app
CPPFLAGS = -I ./ 
src = $(wildcard ./*.cpp)
obj = $(patsubst %.c,%.o,$(src))
$(target):$(obj)
	g++ $(obj) -o $(target) -lcurl -g
%.o:%.c
	g++ -c $< -o $@ $(CPPFLAGS) 


