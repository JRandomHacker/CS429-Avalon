PROGS=

LIBS=-pthread
CXXOPTS=-Wall -Wextra -std=c++11
CXX=g++

all : $(PROGS)

clean : 
	rm -f $(PROGS) *.o 

globals.o : globals.cpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $^

player.o : player.cpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $^

server.o : server.cpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $^
