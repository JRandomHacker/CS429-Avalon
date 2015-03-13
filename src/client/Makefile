OBJS=client.o clientController.o clientControllerState.o customActions.o

COMMON_DIR=../common
PROTOS_DIR=../protos

INCLUDES=-I$(PROTOS_DIR) -I$(COMMON_DIR)
CXXOPTS=-Wall -Wextra -std=c++11 -ggdb
CXX=g++

ifeq ($(OS),Windows_NT)
	CXXOPTS += -static-libgcc -static-libstdc++
endif

all : $(OBJS)

clean :
	rm -f $(OBJS)

client.o : client.cpp client.hpp
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<

clientController.o : clientController.cpp clientController.hpp
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<

clientControllerState.o : clientControllerState.cpp clientControllerState.hpp
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<

customActions.o : customActions.cpp customActions.hpp
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<
