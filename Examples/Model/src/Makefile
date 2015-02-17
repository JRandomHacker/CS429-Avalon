PROGS=model_demo.exe
MODEL_OBJS=model.o subscriber.o model_data.o

LIBS=
CXXOPTS=-Wall -Wextra -std=c++11
CXX=g++
#CXX=x86_64-w64-mingw32-g++
#CXX=i686-w64-mingw32-g++

all : $(PROGS)

clean : 
	rm -f $(PROGS) *.o 

model.o : model.cpp model.hpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $<

subscriber.o : subscriber.cpp subscriber.hpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $<

model_data.o : model_data.cpp model_data.hpp
	$(CXX) $(CXXOPTS) $(LIBS) -c $<

model_demo.exe : model_demo.cpp $(MODEL_OBJS)
	$(CXX) $(CXXOPTS) $(LIBS) $^ -o $@
