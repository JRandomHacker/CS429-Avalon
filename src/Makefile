PROGS=

LIBS=-pthread
CXXOPTS=-Wall -Wextra
CXX=g++

all : $(PROGS)

clean : 
	rm -f $(PROGS) *.o 

player.o : player.cpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $^
