OBJS=settings.pb.o player.pb.o

PROTOS_DIR=.

CXXOPTS=-Wall -Wextra -std=c++11 -ggdb
CXX=g++

ifeq ($(OS),Windows_NT)
	CXXOPTS += -static-libgcc -static-libstdc++
endif

all : $(OBJS)

clean : 
	rm -f *.o *.h *.cc

protos :
	protoc -I=$(PROTOS_DIR) --cpp_out=$(PROTOS_DIR) $(PROTOS_DIR)/*.proto

settings.pb.cc :
	$(MAKE) protos

player.pb.cc :
	$(MAKE) protos

settings.pb.o : settings.pb.cc
	$(CXX) $(CXXOPTS) -c $<

player.pb.o : player.pb.cc
	$(CXX) $(CXXOPTS) -c $<
