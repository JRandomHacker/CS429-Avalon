PROGS=server.exe client.exe
SERVER_OBJS=server.o player.o globals.o player.pb.o
CLIENT_OBJS=model.o subscriber.o model_data.o data_block.o player.o globals.o player.pb.o

PROTOS_DIR=../protos
PROTOS_LIB=`pkg-config --cflags protobuf --libs protobuf`

INCLUDES=-I$(PROTOS_DIR)

LIBS=-pthread $(PROTOS_LIB)
ifeq ($(OS),Windows_NT)
	LIBS+=-lws2_32
endif

CXXOPTS=-Wall -Wextra -std=c++11
CXX=g++
#CXX=x86_64-w64-mingw32-g++
#CXX=i686-w64-mingw32-g++

all : $(PROGS)

clean : 
	rm -f $(PROGS) *.o  $(PROTOS_DIR)/*.h $(PROTOS_DIR)/*.cc

protofiles :
	protoc -I=$(PROTOS_DIR) --cpp_out=$(PROTOS_DIR) $(PROTOS_DIR)/*.proto

globals.o : globals.cpp
	$(CXX) $(CXXOPTS) -c $<

player.o : player.cpp
	$(CXX) $(CXXOPTS) -c $<

server.o : server.cpp  protofiles
	$(CXX) $(CXXOPTS) $(INCLUDES) -c $<
	
model.o : model.cpp model.hpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $<

subscriber.o : subscriber.cpp subscriber.hpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $<

model_data.o : model_data.cpp model_data.hpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $<

data_block.o : data_block.cpp data_block.hpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $<

player.pb.o : $(PROTOS_DIR)/player.pb.cc
	 pkg-config --cflags protobuf
	 $(CXX) $(CXXOPTS) -c $<

server.exe : server_main.cpp $(SERVER_OBJS)
	$(CXX) $(CXXOPTS) $^ -o $@ $(LIBS)
	
client.exe : mock_client.cpp $(CLIENT_OBJS)
	$(CXX) $(CXXOPTS) $^ -o $@ $(LIBS)
