OBJS=server.o serverController.o serverControllerState.o serverStateHelpers.o serverCustomActions.o

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

server.o : server.cpp server.hpp
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<
	
serverController.o : serverController.cpp server.hpp
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<

serverControllerState.o : serverControllerState.cpp serverControllerState.hpp
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<

serverStateHelpers.o : serverStateHelpers.cpp serverStateHelpers.hpp
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<

serverCustomActions.o : serverCustomActions.cpp serverCustomActions.hpp
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<
