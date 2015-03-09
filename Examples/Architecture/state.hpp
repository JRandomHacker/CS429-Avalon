#ifndef STATE_HPP
#define STATE_HPP

#include <string>

class State {
public:
    State();

    void actionHandle(Action* newAct);
};

#endif //STATE_HPP
