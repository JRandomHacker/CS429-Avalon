

#ifndef DATA_BLOCK_HPP
#define DATA_BLOCK_HPP

#include <vector>

#include "model_data.hpp"

class Subscriber;

/**
 * A class representing a block of data in the model. It both stores a
 * ModelData for its payload and has a list of subscribers that it updates when
 * the data changes.
 *
 * @class DataBlock
 * @author Matthew Hoffman
 * @date 2015-03-18
 */
class DataBlock{
public:
    DataBlock( );
    ~DataBlock( );
    
    /**
     * Public constructor
     * 
     * @param that DataBlock to initialize with
     */
    DataBlock( const DataBlock& that );

    /**
     * Adds a subscriber so that they can be notified of any changes
     * 
     * @param new_subscriber Pointer to the Subscriber object for our new subscriber
     * @return true on success.
     */
    bool addSubscriber( Subscriber* new_subscriber );
    
    /**
     * Releases an old subscriber
     * 
     * @param old_subscriber Pointer to the Subscriber object for the old subscriber to be released
     * @return true if subscriber successfully released, false if not found
     */
    bool detachSubscriber( Subscriber* old_subscriber );
    
    /**
     * Removes an old subscriber without destroying its data
     * 
     * @param old_subscriber Pointer to the Subscriber object for the old subscriber to be removed
     * @return true if subscriber successfully removed, false if not found
     */
    bool detachSubscriberQuietly( Subscriber* old_subscriber );
    
    /**
     * Release all subscribers
     */
    void detachAllSubscribers( );

    /**
     * Updates the DataBlcok with new data and notifies all subscribers.
     * 
     * @param new_data the new data to add to the DataBlock
     */
    template < typename T >
    void updateData( const T& new_data );

    /**
     * Returns the current data, if there is any.
     * 
     * @return current data or null if no data exists
     */
    template < typename T >
    T* getData( );

    /**
     * Notifies all subscribers that data was updated.
     */
    void alertAllSubscribersToUpdate( );

private:
    ModelData* data_payload = NULL;
    std::vector< Subscriber* > subscribers;


    /**
     * Deletes all current data
     */
    void releaseData( );
    
    /**
     * Deletes data of given subscriber
     * 
     * @param old_subscriber Pointer to Subscriber object whose data will be deleted
     **/
    void releaseSubscriber( Subscriber* old_subscriber );
};

template < typename T >
void DataBlock::updateData( const T& new_data ) {
    releaseData( );
    data_payload = new TypedModelData< T >( new_data );
    alertAllSubscribersToUpdate( );
}

template < typename T >
T* DataBlock::getData( ) {
    if ( data_payload != NULL ) {
        TypedModelData< T >* data_payload_typed = dynamic_cast< TypedModelData< T >* >( data_payload );
        if ( data_payload_typed != NULL ) {
            return data_payload_typed->getPayloadReference( );
        }
    }
    return NULL;
}

#endif // DATA_BLOCK_HPP
