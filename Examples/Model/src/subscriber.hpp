
#ifndef SUBSCRIBER_HPP
#define SUBSCRIBER_HPP

#include "model_data.hpp"
#include "data_block.hpp"

#include <iostream>
#include <functional>

class Subscriber {
public:
    Subscriber();
    virtual ~Subscriber();

    // Interface functions
    template <typename T>
    T* getData();

    // Model access functions
    void setDataBlock(DataBlock* new_data_block);

    virtual void dataUpdated() = 0;
    virtual void dataDestroyed() = 0;

private:
    DataBlock* data_block = NULL;

    Subscriber(const Subscriber& that);
};

template <typename T>
T* Subscriber::getData() {
    if (data_block == NULL) {
        return NULL;
    }
    return data_block->getData<T>();
}


class ClosureSubscriber : public Subscriber {
public:
    ClosureSubscriber(std::function<void ((Subscriber*))> duc,
        std::function<void ((Subscriber*))> ddc);

    virtual void dataUpdated();
    virtual void dataDestroyed();

private:
    std::function<void ((Subscriber*))> dataUpdatedClosure;
    std::function<void ((Subscriber*))> dataDestroyedClosure;
};

class MockSubscriber : public Subscriber {
public:
    virtual void dataUpdated();
    virtual void dataDestroyed();

    int updatedCount = 0;
    bool wasDestroyed = false;
};

#endif // SUBSCRIBER_HPP
