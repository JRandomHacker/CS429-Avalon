PROGS=server.exe client.exe
SERVER_OBJS=server.o player.o globals.o player.pb.o settings.pb.o
OBJS=client.o clientController.o model.o subscriber.o model_data.o data_block.o player.o globals.o player.pb.o settings.pb.o customActions.o action.o actionHandler.o controllerState.o clientControllerState.o

BIN_DIR=.
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

tests :
	make $(OBJS) $(SERVER_OBJS)
	cd ../tests && make
	cp ../tests/*.exe $(BIN_DIR)

clean : 
	rm -f *.exe *.o *.gch
	rm -f gui/*.o gui/moc_* gui/ui_* gui/Makefile*
	rm -f $(PROTOS_DIR)/*.h $(PROTOS_DIR)/*.cc
	rm -rf gui/debug gui/release
	rm -f ../tests/*.exe ../tests/*.o ../tests/*.a
	rm -f ../bin/*.exe

protofiles :
	protoc -I=$(PROTOS_DIR) --cpp_out=$(PROTOS_DIR) $(PROTOS_DIR)/*.proto

globals.o : globals.cpp
	$(CXX) $(CXXOPTS) -c $<

player.o : player.cpp
	$(CXX) $(CXXOPTS) -c $<

server.o : server.cpp protofiles
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<
	
client.o : client.cpp protofiles
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

controllerState.o : controllerState.cpp controllerState.hpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $<

clientControllerState.o : clientControllerState.cpp clientControllerState.hpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $<

customActions.o : customActions.cpp customActions.hpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $<

action.o : action.cpp action.hpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $<

actionHandler.o : actionHandler.cpp actionHandler.hpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $<

settings.pb.o : $(PROTOS_DIR)/settings.pb.cc
	 $(CXX) $(CXXOPTS) -c $<

player.pb.o : $(PROTOS_DIR)/player.pb.cc
	 $(CXX) $(CXXOPTS) -c $<

server.exe : server_main.cpp $(SERVER_OBJS)
	$(CXX) $(CXXOPTS) $^ -o $(BIN_DIR)/$@ $(LIBS)
	
client.exe : $(OBJS)
	cd gui/ && qmake && make
	mv `find gui/ -regextype posix-egrep -regex ".*/AVL_Interface(|.exe)$$"` $(BIN_DIR)/AVL_Interface.exe
