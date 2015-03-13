OBJS=action.o actionHandler.o controllerState.o data_block.o globals.o model.o model_data.o player.o subscriber.o

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

globals.o : globals.cpp globals.hpp
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<

player.o : player.cpp player.hpp
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<

model.o : model.cpp model.hpp
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<

subscriber.o : subscriber.cpp subscriber.hpp
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<

model_data.o : model_data.cpp model_data.hpp
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<

data_block.o : data_block.cpp data_block.hpp
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<

controllerState.o : controllerState.cpp controllerState.hpp
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<

action.o : action.cpp action.hpp
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<

actionHandler.o : actionHandler.cpp actionHandler.hpp
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<
