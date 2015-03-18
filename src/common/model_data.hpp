
#ifndef MODEL_DATA_HPP
#define MODEL_DATA_HPP

#include <stdlib.h>

/**
 * A class representing model data. It doesn't actually do anything, but is
 * to be subclassed for actual use.
 *
 * @class ModelData
 * @author Matthew Hoffman
 * @date 2015-03-18
 */
class ModelData {
public:
    /**
     * Public constructor
    */
    ModelData( ) = default;

    /**
     * Virtual public destructor for polymorphism.
     */
    virtual ~ModelData( ) = default;

private:
    /**
     * private copy constructor prevents copying
     */
    ModelData( const ModelData& that );
};


/**
 * A class subclassing ModelData. It stores a single payload of a given type
 * based on its template, and can return references to the payload.
 *
 * @class TypedModelData
 * @author Matthew Hoffman
 * @date 2015-03-18
 */
template < typename T >
class TypedModelData : public ModelData {
public:
    /**
     * Explicitly set no public empty constructor. There must be a payload.
     */
    TypedModelData( ) = delete;
    
    /**
     * Constructor that takes a reference to what the payload should be, then
     * copies it to store.
     * @param payload_in A reference to what will become the payload
     */
    explicit TypedModelData( const T& payload_in ) : payload( payload_in ) {}
    
    /**
     * Default destructor
     */
    virtual ~TypedModelData( ) = default;

    /**
     * Returns a reference of the appropriate type to whatever payload is stored
     * @return payload
    */
    T& getPayload( ) { return payload; };

    /**
     * Returns a pointer of the appropriate type to whatever payload is stored
     * @return payload
    */
    T* getPayloadReference( ) { return &payload; };

private:
    T payload;

    /**
     * private copy constructor prevents accidental copying
     */
    TypedModelData( const TypedModelData& that );
};

#endif // MODEL_DATA_HPP
