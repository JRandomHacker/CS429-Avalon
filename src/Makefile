PROGS=server.exe
SERVER_OBJS=server.o player.o globals.o

LIBS=-pthread
CXXOPTS=-Wall -Wextra -std=c++11
CXX=g++
#CXX=x86_64-w64-mingw32-g++
#CXX=i686-w64-mingw32-g++

all : $(PROGS)

clean : 
	rm -f $(PROGS) *.o 

globals.o : globals.cpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $<

player.o : player.cpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $<

server.o : server.cpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $<

server.exe : server_main.cpp $(SERVER_OBJS)
	$(CXX) $(CXXOPTS) $(LIBS) $^ -o $@
