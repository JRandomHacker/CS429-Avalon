PROGS=server.exe AVL_Interface.exe

BIN_DIR=.
COMMON_DIR=./common
CLIENT_DIR=./client
SERVER_DIR=./server
PROTOS_DIR=./protos
GUI_DIR=./gui

SERVER_OBJS=$(COMMON_DIR)/*.o $(SERVER_DIR)/*.o $(PROTOS_DIR)/*.o
PROTOS_LIB=`pkg-config --cflags protobuf --libs protobuf`

LIBS=-pthread $(PROTOS_LIB)
INCLUDES=-I$(PROTOS_DIR) -I$(COMMON_DIR) -I$(CLIENT_DIR) -I$(SERVER_DIR)
CXXOPTS=-Wall -Wextra -std=c++11 -ggdb
CXX=g++

ifeq ($(OS),Windows_NT)
	LIBS+=-lws2_32
	CXXOPTS += -static-libgcc -static-libstdc++
endif

.PHONY : protos common client server tests clean

all : $(PROGS)

tests :
	make
	cd ../tests && make
	cp ../tests/*.exe $(BIN_DIR)

clean : 
	cd $(PROTOS_DIR) && make clean
	cd $(CLIENT_DIR) && make clean
	cd $(SERVER_DIR) && make clean
	cd $(COMMON_DIR) && make clean
	rm -f *.exe
	rm -f gui/*.o gui/moc_* gui/ui_* gui/Makefile*
	rm -rf gui/debug gui/release
	rm -f ../tests/*.exe ../tests/*.o ../tests/*.a

protos :
	cd $(PROTOS_DIR) && make

client : common
	cd $(CLIENT_DIR) && make

server : common
	cd $(SERVER_DIR) && make

common : protos
	cd $(COMMON_DIR) && make

server.exe : $(SERVER_DIR)/server_main.cpp server
	$(CXX) $(CXXOPTS) $(INCLUDES) $< $(SERVER_OBJS) -o $(BIN_DIR)/$@ $(LIBS)
	
$(GUI_DIR)/Makefile : $(GUI_DIR)/AVL_Interface.pro
	cd $(GUI_DIR) && qmake

AVL_Interface.exe : $(GUI_DIR)/Makefile client
	cd gui/ && make
	mv `find gui/ -regextype posix-egrep -regex ".*/AVL_Interface(|.exe)$$"` $(BIN_DIR)/AVL_Interface.exe
