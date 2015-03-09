#include "Action.hpp"
#include "ActionHandler.hpp"
#include "MockController.hpp"
#include "CustomActions.hpp"

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

static Controller controller;

void beginControllerProcessing() {
    controller.beginProcessing();
}

int main() {
    std::thread controller_process_thread(&beginControllerProcessing);

    std::cin.get();
    controller.addActionToQueue(new JoinGameAction("jfeuisfes"));
    std::cin.get();
    controller.addActionToQueue(new EnterLobbyAction("FJIOES"));
    while (true) {
        std::cin.get();
        controller.addActionToQueue(new AddPlayerAction("LJI"));
    }

    return 0;
}