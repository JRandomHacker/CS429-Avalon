OBJS=client.o clientController.o clientControllerState.o clientCustomActionsForChat.o clientCustomActionsFromNet.o clientCustomActionsFromGUI.o voteHistory.o mockClient.o lobbyState.o teamSelectionState.o votingState.o questVotingState.o

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

%.o : %.cpp %.hpp
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<
