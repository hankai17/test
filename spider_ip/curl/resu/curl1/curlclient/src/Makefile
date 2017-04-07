INCLUDE = -I.
STD = -std=c++11
CXXFLAGS = $(STD) -g -O2 -Wall $(INCLUDE)
LD = $(CXX)
LIBS = -lstdc++ -lcurl
LDFLAGS = $(STD) $(LIBS)

SRC_DIR = .
OBJ_DIR = ./object
LIB_DIR = ../lib
INC_DIR = ../include/
LIB = libcurlclient.a

MAKE_OBJ_DIR := $(shell mkdir -p $(OBJ_DIR))
MAKE_LIB_DIR := $(shell mkdir -p $(LIB_DIR))
MAKE_INC_DIR := $(shell mkdir -p $(INC_DIR))
OBJS = $(addsuffix .o, $(subst $(SRC_DIR),$(OBJ_DIR),$(basename $(wildcard $(SRC_DIR)/*.cpp))))

all : $(OBJS)
	cp *.h $(INC_DIR)
	ar -r $(LIB_DIR)/$(LIB) $(subst $(OBJ_DIR)/test.o,,$(OBJS))

test : $(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)


.PHONY : clean rm
clean:
	rm -f $(OBJ_DIR)/*
	rmdir $(OBJ_DIR)
	rm -f $(TARGET)
rm : clean
