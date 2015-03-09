#include "Action.hpp"
#include "ActionHandler.hpp"

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

static ActionHandler action_queue;

void action_spawner(std::string action_message, int delay) {
    int action_count = 0;
    while (true) {
        Action* new_action = new Action(action_message);
        action_queue.AddAction(new_action);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        action_count++;
    }
}


int main() {
    std::thread generating_thread_a(&action_spawner, "a", 1000);
    std::thread generating_thread_b(&action_spawner, "b", 500);

    while (true) {
        std::cin.get();
        auto actions = action_queue.FreezeFrontActions();
        if (actions.first != 0) {
            auto action_iter = actions.second;
            for (int i = 0; i < actions.first; i++) {
                std::cout << (*actions.second)->GetMessage();
                actions.second++;
            }
            action_queue.ReleaseFrozenActions();
        }
    }

    return 0;
}