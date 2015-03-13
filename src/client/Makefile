OBJS=client.o clientController.o clientControllerState.o clientCustomActionsFromNet.o clientCustomActionsFromGUI.o

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

clientCustomActionsFromNet.o : clientCustomActionsFromNet.cpp clientCustomActionsFromNet.hpp
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<

clientCustomActionsFromGUI.o : clientCustomActionsFromGUI.cpp clientCustomActionsFromGUI.hpp
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<
