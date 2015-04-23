PROGS=server.exe AVL_Interface.exe

BIN_DIR=.
COMMON_DIR=./common
CLIENT_DIR=./client
SERVER_DIR=./server
PROTOS_DIR=./protos
GUI_DIR=./gui
TESTS_DIR=../tests
LUA_DIR=./lua

SERVER_OBJS=$(COMMON_DIR)/*.o $(SERVER_DIR)/*.o $(PROTOS_DIR)/*.o
PROTOS_LIB=`pkg-config --cflags protobuf --libs protobuf`

LIBS=-pthread $(PROTOS_LIB)
INCLUDES=-I$(PROTOS_DIR) -I$(COMMON_DIR) -I$(CLIENT_DIR) -I$(SERVER_DIR) -I$(LUA_DIR)/src
CXXOPTS=-Wall -Wextra -std=c++11 -ggdb
CXX=g++

LUA_PLAT=posix

ifeq ($(OS),Windows_NT)
	LIBS+=-lws2_32
	CXXOPTS += -static-libgcc -static-libstdc++
	LUA_PLAT=mingw
endif

.PHONY : protos common client server tests lua clean

all : $(PROGS)

tests :
	$(MAKE)
	$(MAKE) -C $(TESTS_DIR)
	cp $(TESTS_DIR)/*.exe $(BIN_DIR)

clean : 
	cd $(PROTOS_DIR) && $(MAKE) clean
	cd $(CLIENT_DIR) && $(MAKE) clean
	cd $(SERVER_DIR) && $(MAKE) clean
	cd $(COMMON_DIR) && $(MAKE) clean
	cd $(LUA_DIR) && $(MAKE) clean
	rm -f *.exe
	rm -f $(GUI_DIR)/moc_* $(GUI_DIR)/ui_* $(GUI_DIR)/object_*
	rm -f $(GUI_DIR)/Makefile* $(GUI_DIR)/*.o
	rm -rf $(GUI_DIR)/debug $(GUI_DIR)/release
	rm -f $(TESTS_DIR)/*.exe $(TESTS_DIR)/*.o $(TESTS_DIR)/*.a

protos :
	$(MAKE) -C $(PROTOS_DIR)

lua :
	$(MAKE) -C $(LUA_DIR) $(LUA_PLAT) 

client : common
	$(MAKE) -C $(CLIENT_DIR)

server : common lua
	$(MAKE) -C $(SERVER_DIR)

common : protos
	$(MAKE) -C $(COMMON_DIR)

server.exe : $(SERVER_DIR)/server_main.cpp server lua
	$(CXX) $(CXXOPTS) $(INCLUDES) $< $(SERVER_OBJS) -o $(BIN_DIR)/$@ $(LIBS) \
	-L./lua/src -llua
	
# Depend on server, client and common, since apparently shell globbing
# doesn't work properly in Windows qmake if the files don't exist yet
$(GUI_DIR)/Makefile : $(GUI_DIR)/AVL_Interface.pro server client common
	cd $(GUI_DIR) && qmake

AVL_Interface.exe : $(GUI_DIR)/Makefile client
	$(MAKE) -C $(GUI_DIR)
	mv `find $(GUI_DIR) -regextype posix-egrep -regex ".*/AVL_Interface(|.exe)$$"` $(BIN_DIR)/AVL_Interface.exe
