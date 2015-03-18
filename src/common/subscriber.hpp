
#ifndef SUBSCRIBER_HPP
#define SUBSCRIBER_HPP

#include "model_data.hpp"
#include "data_block.hpp"

#include <iostream>
#include <functional>

class Subscriber {
public:
    /**
     * Public constructor
     */
    Subscriber();
    virtual ~Subscriber();

    // Interface functions
    
    /**
     * Gets data from Subscriber
     * 
     * @return Subscriber's data with type T
     */
    template <typename T>
    T* getData();

    // Model access functions
    
    /**
     * Sets subscriber's data block to given DataBlock
     * 
     * @param new_data_block Pointer to DataBlock to give to the subscriber
     */
    void setDataBlock(DataBlock* new_data_block);

    /**
     * If data was updated, sends subscriber pointer to proper method
     */
    virtual void dataUpdated() = 0;
    
    /**
     * If data was destroyed, sends subscriber pointer to proper method
     */
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
    /**
     * Public constructor
     */
    ClosureSubscriber(std::function<void ((Subscriber*))> duc,
        std::function<void ((Subscriber*))> ddc);

    /**
     * If data was updated, sends subscriber pointer to dataUpdatedClosure method
     */
    virtual void dataUpdated();
    
    
    /**
     * If data was destroyed, sends subscriber pointer to dataDestroyedClosure method
     */
    virtual void dataDestroyed();

private:
    /**
     * Tells Subscriber data was updated
     * @param Pointer to Subscriber
     */
    std::function<void ((Subscriber*))> dataUpdatedClosure;
    
    /**
     * Tells Subscriber data was destroyed
     * @param Pointer to Subscriber
     */
    std::function<void ((Subscriber*))> dataDestroyedClosure;
};

class MockSubscriber : public Subscriber {
public:

    /**
     * If data was updated, sends subscriber pointer to proper method
     */
    virtual void dataUpdated();
    
    
    /**
     * If data was updated, sends subscriber pointer to proper method
     */
    virtual void dataDestroyed();

    int updatedCount = 0;
    bool wasDestroyed = false;
};

#endif // SUBSCRIBER_HPP
