all: ActionDemo

Action: Action.hpp Action.cpp
	g++ Action.cpp -c -std=c++11 -o Action.o

ActionHandler: ActionHandler.hpp ActionHandler.cpp
	g++ ActionHandler.cpp -c -std=c++11 -o ActionHandler.o

ActionDemo: ActionDemo.cpp ActionHandler Action
	g++ ActionDemo.cpp Action.o ActionHandler.o -std=c++11 -o ActionDemo.exe
