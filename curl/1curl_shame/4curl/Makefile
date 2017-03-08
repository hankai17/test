target = app
CPPFLAGS = -I ./ 
src = $(wildcard ./*.cpp)
obj = $(patsubst %.c,%.o,$(src))
$(target):$(obj)
	g++ $(obj) -o $(target) -lcurl
%.o:%.c
	g++ -c $< -o $@ $(CPPFLAGS) 


