PROGS=server.exe client.exe
SERVER_OBJS=server.o player.o globals.o player.pb.o settings.pb.o
OBJS=client.o clientController.o model.o subscriber.o model_data.o data_block.o player.o globals.o player.pb.o settings.pb.o customActions.o action.o actionHandler.o controllerState.o

PROTOS_DIR=../protos
PROTOS_LIB=`pkg-config --cflags protobuf --libs protobuf`

INCLUDES=-I$(PROTOS_DIR)

LIBS=-pthread $(PROTOS_LIB)
ifeq ($(OS),Windows_NT)
	LIBS+=-lws2_32
endif

CXXOPTS=-Wall -Wextra -std=c++11 -ggdb
CXX=g++
#CXX=x86_64-w64-mingw32-g++
#CXX=i686-w64-mingw32-g++

all : $(PROGS) $(OBJS)

clean : 
	rm -f *.exe *.o *.gch
	rm -f gui/*.o gui/moc_* gui/ui_* gui/Makefile*
	rm -f $(PROTOS_DIR)/*.h $(PROTOS_DIR)/*.cc
	rm -rf gui/debug gui/release

protofiles :
	protoc -I=$(PROTOS_DIR) --cpp_out=$(PROTOS_DIR) $(PROTOS_DIR)/*.proto

globals.o : globals.cpp
	$(CXX) $(CXXOPTS) -c $<

player.o : player.cpp
	$(CXX) $(CXXOPTS) -c $<

server.o : server.cpp  protofiles
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<
	
client.o : client.cpp  protofiles
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<

clientController.o : clientController.cpp
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<

model.o : model.cpp model.hpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $<

subscriber.o : subscriber.cpp subscriber.hpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $<

model_data.o : model_data.cpp model_data.hpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $<

data_block.o : data_block.cpp data_block.hpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $<

controllerState.o : ControllerState.cpp ControllerState.hpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $<

customActions.o : CustomActions.cpp CustomActions.hpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $<

action.o : Action.cpp Action.hpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $<

actionHandler.o : ActionHandler.cpp ActionHandler.hpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $<

settings.pb.o : $(PROTOS_DIR)/settings.pb.cc
	 $(CXX) $(CXXOPTS) -c $<

player.pb.o : $(PROTOS_DIR)/player.pb.cc
	 $(CXX) $(CXXOPTS) -c $<

server.exe : server_main.cpp $(SERVER_OBJS)
	$(CXX) $(CXXOPTS) $^ -o $@ $(LIBS)
	
client.exe : $(OBJS)
	cd gui/ && qmake && make
	mv `find gui/ -regextype posix-egrep -regex ".*/AVL_Interface(|.exe)$$"` ./AVL_Interface.exe
