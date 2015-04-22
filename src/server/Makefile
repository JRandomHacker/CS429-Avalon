OBJS=server.o \
	serverController.o \
	serverControllerState.o \
	serverCustomActions.o \
	mockServer.o \
	waitingForClientsState.o \
	teamSelectionState.o \
	teamVotingState.o \
	serverInfo.o \
	serverSettings.o \
	questVotingState.o \
	assassinState.o \
	endGameState.o \
	decideEndState.o

COMMON_DIR=../common
PROTOS_DIR=../protos
LUA_DIR=../lua/src

INCLUDES=-I$(PROTOS_DIR) -I$(COMMON_DIR) -I$(LUA_DIR)
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
