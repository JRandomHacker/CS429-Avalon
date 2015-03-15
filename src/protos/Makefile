OBJS=settings.pb.o player.pb.o teamselection.pb.o vote.pb.o

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

%.pb.cc %.pb.h : %.proto
	protoc --proto_path=$(PROTOS_DIR) --cpp_out=$(PROTOS_DIR) $<

%.pb.o : %.pb.cc %.pb.h
	$(CXX) $(CXXOPTS) -c $<
