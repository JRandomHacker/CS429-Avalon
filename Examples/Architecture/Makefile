all: ActionDemo ControllerDemo

Action: Action.hpp Action.cpp
	g++ Action.cpp -c -std=c++11 -o Action.o

ActionHandler: ActionHandler.hpp ActionHandler.cpp
	g++ ActionHandler.cpp -c -std=c++11 -o ActionHandler.o

ControllerState: ControllerState.hpp ControllerState.cpp
	g++ ControllerState.cpp -c -std=c++11 -o ControllerState.o

CustomActions: CustomActions.hpp CustomActions.cpp
	g++ CustomActions.cpp -c -std=c++11 -o CustomActions.o

MockController: MockController.hpp MockController.cpp
	g++ MockController.cpp -c -std=c++11 -o MockController.o

ControllerDemo: ControllerDemo.cpp ActionHandler Action ControllerState CustomActions MockController
	g++ ControllerDemo.cpp ActionHandler.o Action.o ControllerState.o CustomActions.o MockController.o -std=c++11 -o ControllerDemo.exe

ActionDemo: ActionDemo.cpp ActionHandler Action
	g++ ActionDemo.cpp Action.o ActionHandler.o -std=c++11 -o ActionDemo.exe
