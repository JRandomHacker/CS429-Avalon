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
	$(MAKE)
	$(MAKE) -C ../tests
	cp ../tests/*.exe $(BIN_DIR)

clean : 
	cd $(PROTOS_DIR) && $(MAKE) clean
	cd $(CLIENT_DIR) && $(MAKE) clean
	cd $(SERVER_DIR) && $(MAKE) clean
	cd $(COMMON_DIR) && $(MAKE) clean
	rm -f *.exe
	rm -f gui/*.o gui/moc_* gui/ui_* gui/Makefile*
	rm -rf gui/debug gui/release
	rm -f ../tests/*.exe ../tests/*.o ../tests/*.a

protos :
	$(MAKE) -C $(PROTOS_DIR)

client : common
	$(MAKE) -C $(CLIENT_DIR)

server : common
	$(MAKE) -C $(SERVER_DIR)

common : protos
	$(MAKE) -C $(COMMON_DIR)

server.exe : $(SERVER_DIR)/server_main.cpp server
	$(CXX) $(CXXOPTS) $(INCLUDES) $< $(SERVER_OBJS) -o $(BIN_DIR)/$@ $(LIBS)
	
# Depend on server, client and common, since apparently shell globbing
# doesn't work properly in Windows qmake if the files don't exist yet
$(GUI_DIR)/Makefile : $(GUI_DIR)/AVL_Interface.pro server client common
	cd $(GUI_DIR) && qmake

AVL_Interface.exe : $(GUI_DIR)/Makefile client
	$(MAKE) -C $(GUI_DIR)
	mv `find $(GUI_DIR) -regextype posix-egrep -regex ".*/AVL_Interface(|.exe)$$"` $(BIN_DIR)/AVL_Interface.exe
