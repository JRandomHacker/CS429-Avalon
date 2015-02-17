PROGS=server.exe
SERVER_OBJS=server.o player.o globals.o player.pb.o

PROTOS_DIR=../protos
PROTOS_LIB=`pkg-config --cflags protobuf --libs protobuf`

INCLUDES=-I$(PROTOS_DIR)

LIBS=-pthread $(PROTOS_LIB)
ifeq ($(OS),Windows_NT)
	LIBS+=-lwsock32
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

player.pb.o : $(PROTOS_DIR)/player.pb.cc
	 pkg-config --cflags protobuf
	 $(CXX) $(CXXOPTS) -c $<

server.exe : server_main.cpp $(SERVER_OBJS)
	$(CXX) $(CXXOPTS) $^ -o $@ $(LIBS)
