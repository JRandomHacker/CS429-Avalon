
#ifndef SUBSCRIBER_HPP
#define SUBSCRIBER_HPP

#include "model_data.hpp"
#include "data_block.hpp"

#include <iostream>
#include <functional>

/**
 * A virtual class for an object that can subscribe to a block of data in the
 * Model class
 *
 * @class Subscriber
 * @author Matthew Hoffman
 * @date 2015-03-18
 */
class Subscriber {
public:
    /**
     * Public constructor
     */
    Subscriber( );
    virtual ~Subscriber( );

    // Interface functions
    
    /**
     * Gets data from Subscriber
     * 
     * @return Subscriber's data with type T
     */
    template < typename T >
    T* getData( );

    // Model access functions
    
    /**
     * Sets subscriber's data block to given DataBlock
     * 
     * @param new_data_block Pointer to DataBlock to give to the subscriber
     */
    void setDataBlock( DataBlock* new_data_block );

    /**
     * If data was updated, sends subscriber pointer to proper method
     */
    virtual void dataUpdated( ) = 0;
    
    /**
     * If data was destroyed, sends subscriber pointer to proper method
     * @return Should this subscriber be automatically free'd by the model
     */
    virtual bool dataDestroyed( ) = 0;

private:
    DataBlock* data_block = NULL;

    Subscriber( const Subscriber& that );
};

template < typename T >
T* Subscriber::getData() {
    if (data_block == NULL) {
        return NULL;
    }
    return data_block->getData<T>( );
}

/**
 * A subclass of Subscriber that takes closures for its subscription callbacks
 * Model class
 *
 * @class ClosureSubscriber
 * @author Matthew Hoffman
 * @date 2015-03-18
 */
class ClosureSubscriber : public Subscriber {
public:
    /**
     * Public constructor
     */
    ClosureSubscriber( std::function<void ((Subscriber*))> duc,
        std::function<void ((Subscriber*))> ddc );

    /**
     * Public constructor
     */
    ClosureSubscriber( std::function<void ((Subscriber*))> duc,
        std::function<void ((Subscriber*))> ddc, bool freeSelfOnDestruction );

    /**
     * If data was updated, sends subscriber pointer to dataUpdatedClosure method
     */
    virtual void dataUpdated();
    
    
    /**
     * If data was destroyed, sends subscriber pointer to dataDestroyedClosure method
     * If this returns true, the model containing it is given ownership of
     * the subscriber, which basically means its destroyed automatically
     * @return Should this subscriber be automatically cleaned up
     */
    virtual bool dataDestroyed( );

private:
    bool freeOnDestruction;

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

/**
 * A mock subclass of Subscriber that counts how many times it was updated and
 * checks whether it was destroyed.
 *
 * @class MockSubscriber
 * @author Matthew Hoffman
 * @date 2015-03-18
 */
class MockSubscriber : public Subscriber {
public:

    /**
     * If data was updated, sends subscriber pointer to proper method
     */
    virtual void dataUpdated( );
    
    
    /**
     * If data was updated, sends subscriber pointer to proper method
     * @return Should this object be automatically destroyed
     */
    virtual bool dataDestroyed( );

    int updatedCount = 0;
    bool wasDestroyed = false;
};

#endif // SUBSCRIBER_HPP
