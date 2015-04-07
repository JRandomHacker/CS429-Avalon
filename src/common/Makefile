OBJS=action.o \
	 actionHandler.o \
	 chat_message.o \
	 controllerState.o \
	 data_block.o \
	 globals.o \
	 model.o \
	 model_data.o \
	 player.o \
	 subscriber.o

PROTOS_DIR=../protos

INCLUDES=-I$(PROTOS_DIR)
CXXOPTS=-Wall -Wextra -std=c++11 -ggdb
CXX=g++

ifeq ($(OS),Windows_NT)
	CXXOPTS += -static-libgcc -static-libstdc++
endif

all : $(OBJS)

clean :
	rm -f $(OBJS)

%.o : %.cpp %.hpp
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<
