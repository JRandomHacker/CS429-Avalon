
#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <unordered_map>

#include "model_data.hpp"
#include "data_block.hpp"
#include "subscriber.hpp"

/**
 * A class for storing key, value pairs where the values are of arbitrary
 * types. Also, the data is retrieved by setting up subscriptions with
 * Subscriber objects who have their update called whenever their subscribed
 * data changes.
 *
 * @class Model
 * @author Matthew Hoffman
 * @date 2015-03-18
 */
class Model {
public:
    Model( );
    virtual ~Model( );

    /**
     * If the given data_id is found, adds new subscriber.
     * 
     * @param data_id id to search for
     * @param new_subscriber Pointer to Subscriber object to add to subscribers
     * @return true if successfully added, false if data_id not found
     */
    virtual bool subscribe( std::string data_id, Subscriber* new_subscriber );
    
    /**
     * Remove the old_subscriber from the list of subscribers to data_id

     * @param data_id id to search for
     * @param old_subscriber Pointer to Subscriber object to remove from subscribers
     * @return true if successfully removed, false if data_id not found
     */
    virtual bool unsubscribe( std::string data_id, Subscriber* old_subscriber );

    /**
     * Returns a reference to the data in the model so it can be updated without replacement
     * 
     * @param data_id id to search for
     * @return pointer to the data stored at data_id, NULL if there is no block there with that type
     */
    template < typename T >
    virtual T* getDataForUpdate( std::string data_id );

    /**
     * Treats the data at data_id as if it is new and triggers updates for the subscribers.
     * This is for use after data gotten from getDataForUpdate was modified
     * 
     * @param data_id id to search for
     */
    virtual bool flagDataForUpdate( std::string data_id );

    /**
     * If the given data_id is found, updates current data with new_data.
     * @param data_id id to search for
     * @param new_data data to update with
     * @return true if data successfully updated, false if data_id not found
     */
    template < typename T >
    virtual bool updateData( std::string data_id, const T& new_data );
    
    /**
     * Adds the initial data.
     * @param data_id id to search for
     * @param initial_data data to initialize with
     * @return true if data successfully added, false if data_id not found
     */
    template < typename T >
    virtual bool addData( std::string data_id, const T& initial_data );
    
    /**
     * Adds data to new DataBlock.
     * @param data_id id to search for
     * @return true if data successfully added, false if data_id not found
     */
    virtual bool addData( std::string data_id );
    
    /**
     * Removes data for given data_id.
     * @param data_id id to search for
     * @return true if data successfully removed, false if data_id not found
     */
    virtual bool removeData( std::string data_id );

    /**
     * Gets a constant pointer to the data at data_id. Note that this pointer
     * WILL NOT be valid after an update, it must be used by the same thread
     * that does the updates.
     * @param data_id id to search for
     * @return pointer to the data, NULL if there is no data
     */
    template < typename T >
    virtual const T* referenceData( std::string data_id );

private:
    /**
     * Returns whether data exists for given data_id.
     * @param data_id to search for
     * @return true if data exists for data_id, false if data_id not found
     */
    bool doesDataExist( std::string data_id );

    std::unordered_map< std::string, DataBlock > flat_data;

    Model( const Model& that );
};

template < typename T >
bool Model::updateData( std::string data_id, const T& new_data ) {
    auto data_iter = flat_data.find( data_id );
    if ( data_iter == flat_data.end( ) ) {
        return false;
    }
    data_iter->second.updateData<T>( new_data );
    return true;
}

template < typename T >
bool Model::addData( std::string data_id, const T& initial_value ) {
    bool data_created = addData( data_id );
    if ( !data_created ) {
        return false;
    }
    return updateData<T>( data_id, initial_value );
}

template < typename T >
T* Model::getDataForUpdate( std::string data_id ) {
    auto data_iter = flat_data.find( data_id );
    if ( data_iter == flat_data.end( ) ) {
        return NULL;
    }
    return data_iter->second.getData<T>();
}

template < typename T >
const T* Model::referenceData( std::string data_id ) {
    auto data_iter = flat_data.find( data_id );
    if ( data_iter == flat_data.end( ) ) {
        return NULL;
    }
    return data_iter->second.getData<T>();
}

#endif // MODEL_HPP
